#include "aldente.h"
#include "window.h"
#include "shaders/skybox_shader.h"
#include "shaders/shadow_shader.h"
#include "geometry/geometry_generator.h"
#include "scene/scene_model.h"
#include "scene/scene_transform.h"
#include "scene/scene_animation.h"
#include "bounding_sphere.h"
#include <cfloat>
#include <string>

#include "events/input.h"
#include "input/process.h"
#include "util/util.h"
#include "util/colors.h"
#include "global.h"
#include "util/config.h"
#include "btBulletDynamicsCommon.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace input = kuuhaku::input;
namespace events = kuuhaku::events;

/* global vars */
Scene* scene;
std::vector<Scene*> scenes;
SceneCamera* camera;

bool keys[1024];
bool lmb_down = false;
bool rmb_down = false;
bool debug_shadows = false;
bool mouse_moved = false;
bool shadows_on = true;
bool god_mode = false;
bool helicopter_mode = false;
glm::vec3 last_cursor_pos;

const GLfloat PLAYER_HEIGHT = Global::PLAYER_HEIGHT;

const GLfloat FAR_PLANE = 50.f * PLAYER_HEIGHT;
const GLfloat FOV = 45.f;

const GLfloat   BASE_CAM_SPEED = PLAYER_HEIGHT / 10.f;
const GLfloat   EDGE_PAN_THRESH = 5.f;
const GLfloat   EDGE_PAN_SPEED = 0.5f;

//Bullet varaibles
btBroadphaseInterface* broadphase;
btDefaultCollisionConfiguration* collisionConfiguration;
btCollisionDispatcher* dispatcher;
btSequentialImpulseConstraintSolver* solver;
btDiscreteDynamicsWorld* dynamicsWorld;
vector<btRigidBody*> rigidBodies;

Grid* grid;
Tile* hover;

Aldente::Aldente() {}

Aldente::~Aldente() {}

void Aldente::destroy()
{
    // Free memory here.
    ShaderManager::destroy();
    GeometryGenerator::clean_up();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Aldente::setup_shaders()
{
    // Load shaders via a shader manager.
    ShaderManager::create_shader_program("basic");
    ShaderManager::create_shader_program("skybox");
    ShaderManager::create_shader_program("shadow");
    ShaderManager::create_shader_program("debug_shadow");
    ShaderManager::set_default("basic");
}

void Aldente::setup_scenes()
{
    // Initial scene and cam.
    scene = new Scene();
    scenes.push_back(scene);
    camera = scene->camera;
    scene->light_pos = glm::vec3(0.f, 2.f, 1.f);

    // Skybox
    Material default_material;
    Mesh* skybox_mesh = new Mesh{ nullptr, default_material, ShaderManager::get_shader_program("skybox"), glm::mat4(1.f) };
    SceneModel *skybox_model = new SceneModel(scene);
    skybox_model->add_mesh(skybox_mesh);
    scene->root->add_child(skybox_model);

    // Cube
    Geometry *cube_geo = GeometryGenerator::generate_cube(1.f, true);
    Material cube_mat;
    cube_mat.diffuse = cube_mat.ambient = color::ocean_blue;
    Mesh* cube_mesh = new Mesh{ cube_geo, cube_mat, ShaderManager::get_default(), glm::mat4(1.f) };
    SceneModel *cube_model = new SceneModel(scene);
    cube_model->add_mesh(cube_mesh);
    scene->root->add_child(cube_model);

	//Setting up scene graph for Grid

	grid = new Grid(10,10);
	vector<vector<Tile*>> toAdd = grid->getGrid();
	for (int i = 0; i < toAdd.size(); i++) {
		vector<Tile*> currRow = toAdd[i];
		for (int j = 0; j < currRow.size(); j++) {
			SceneModel *currTile = new SceneModel(scene);
			currTile->add_mesh(currRow[j]->getMesh());
			/*if (currRow[j]->getRigid() == NULL) {
				SceneTransform *tileTranslate = new SceneTransform(scene,
				  glm::translate(glm::mat4(1.f), glm::vec3(currRow[j]->getX(), 0, currRow[j]->getZ())));
				tileTranslate->add_child(currTile);
				scene->root->add_child(tileTranslate);
			}
			else {*/
				scene->root->add_child(currTile);
			//}

			if (currRow[j]->getRigid() != NULL) {
				dynamicsWorld->addRigidBody(currRow[j]->getRigid());
				rigidBodies.push_back(currRow[j]->getRigid());
			}

		}
	}

	/*
    // Plane
    Geometry *plane_geo = GeometryGenerator::generate_plane(1.f, 0);
    Material plane_mat;
    plane_mat.diffuse = plane_mat.ambient = color::indian_red;
    Mesh plane_mesh = { plane_geo, plane_mat, ShaderManager::get_default(), glm::mat4(1.f) };
        SceneModel *plane_model = new SceneModel(scene);
        plane_model->add_mesh(plane_mesh);
        SceneTransform *plane_scale = new SceneTransform(scene, glm::scale(glm::mat4(1.f), glm::vec3(50.f, 1.0f, 50.f)));
        SceneTransform *plane_translate = new SceneTransform(scene, glm::translate(glm::mat4(1.f), glm::vec3(0.0f, -0.5f * PLAYER_HEIGHT, 0.0f)));
        plane_scale->add_child(plane_model);
        plane_translate->add_child(plane_scale);
        scene->root->add_child(plane_translate);
	*/
}

void Aldente::go()
{
    // Get window width and height from config
    int width, height;
    Config::config->get_value(Config::str_screen_width, width);
    Config::config->get_value(Config::str_screen_height, height);
    std::string game_name;
    Config::config->get_value(Config::str_game_name, game_name);

    window = Window::create_window(width, height, game_name.c_str());
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); // Don't show cursor
    setup_callbacks();
    setup_opengl();

    setup_shaders();

	// Initialize Bullet. This strictly follows http://bulletphysics.org/mediawiki-1.5.8/index.php/Hello_World,
	// even though we won't use most of this stuff.

	// Build the broadphase
	broadphase = new btDbvtBroadphase();

	// Set up the collision configuration and dispatcher
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	// The actual physics solver
	solver = new btSequentialImpulseConstraintSolver;

	// The world.
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));

	// Seed PRNG.
    Util::seed(0);
    setup_scenes();

    // Send height/width of window
    glfwGetFramebufferSize(window, &width, &height);
    resize_callback(window, width, height);

    GLuint frame = 0;
    double prev_ticks = glfwGetTime();
    double move_prev_ticks = prev_ticks;

	AssetLoader* test = new AssetLoader();
	SceneModel* tmodel = test->getModel(std::string("textured.fbx"));
	tmodel->setScene(scene);
	scene->root->add_child(tmodel);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        input::process();

        frame++;
        double curr_time = glfwGetTime();
        if (curr_time - prev_ticks > 1.f)
        {
            /* std::cerr << "FPS: " << frame << std::endl; */
            frame = 0;
            prev_ticks = curr_time;
        }
        if (curr_time - move_prev_ticks > 1.f / 60.f)
        {
            handle_movement();
            move_prev_ticks = curr_time;
        }

		//Step in simulation
		dynamicsWorld->stepSimulation(1.f / 60.f, 10);

		vector<vector<Tile*>> toAdd = grid->getGrid();
		for (int i = 0; i < toAdd.size(); i++) {
			vector<Tile*> currRow = toAdd[i];
			for (int j = 0; j < currRow.size(); j++) {
				currRow[j]->update(hover);
			}
		}

        glfwGetFramebufferSize(window, &width, &height);
        scene->update_frustum_planes();
        scene->update_frustum_corners(width, height, FAR_PLANE);

        // First pass: shadowmap.
        shadow_pass();

        // Second pass: usual rendering.
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene->render();
        // Debug shadows.
        if (debug_shadows)
        {
            glViewport(0, 0, width / 3, height / 3);
            ShaderManager::get_shader_program("debug_shadow")->use();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, ((ShadowShader *)ShaderManager::get_shader_program("shadow"))->shadow_map_tex);
            Util::render_quad();
        }

        glfwSwapBuffers(window);
    }
    destroy();
}

void Aldente::shadow_pass()
{
    ShadowShader * ss = (ShadowShader *) ShaderManager::get_shader_program("shadow");
    glViewport(0, 0, ss->size, ss->size);
    glBindFramebuffer(GL_FRAMEBUFFER, ss->FBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    ss->use();
    ss->light_pos = scene->light_pos;
    if (shadows_on) {
        ss->light_proj = scene->frustum_ortho();
    }
    else {
        ss->light_proj = glm::ortho(-1.f, 1.f, -1.f, 1.f, 0.f, 0.1f);
    }
    // Render using scene graph.
    glDisable(GL_CULL_FACE);
    scene->pass(ss);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Aldente::handle_movement()
{
    GLfloat cam_step = keys[GLFW_KEY_LEFT_SHIFT] ? 3*BASE_CAM_SPEED : BASE_CAM_SPEED;
    glm::vec3 displacement(0.f);

    int btn_c;
    const unsigned char* btns =
        glfwGetJoystickButtons(GLFW_JOYSTICK_1, &btn_c);

    if (keys[GLFW_KEY_W] || (btns && btns[13]))
        displacement += cam_step * camera->cam_front;
    if (keys[GLFW_KEY_S] || (btns && btns[15]))
        displacement -= cam_step * camera->cam_front;
    if (keys[GLFW_KEY_A] || (btns && btns[16]))
        displacement -= glm::normalize(glm::cross(camera->cam_front, camera->cam_up)) * cam_step;
    if (keys[GLFW_KEY_D] || (btns && btns[14]))
        displacement += glm::normalize(glm::cross(camera->cam_front, camera->cam_up)) * cam_step;
    if (keys[GLFW_KEY_SPACE])
        displacement += cam_step * camera->cam_up;

    camera->cam_pos += displacement;
    camera->recalculate();
}

void Aldente::setup_callbacks()
{
    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetFramebufferSizeCallback(window, resize_callback);

    events::JoystickEvent::subscribe([](events::JoystickData d) {
        fprintf(stderr,
                "JoystickEvent:\n"
                "  id: %d\n"
                "  is_button: %d\n"
                "  input: %d\n"
                "  state: %d\n",
                d.id, d.is_button, d.input, d.state);
    });
}

void Aldente::setup_opengl()
{
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        glfwTerminate();
    }
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Aldente::error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

void Aldente::resize_callback(GLFWwindow* window, int width, int height)
{
    // Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
    glViewport(0, 0, width, height);

    if (height > 0)
    {
        for (Scene * s : scenes)
            s->P = glm::perspective(FOV, (float)width / (float)height, 0.1f, FAR_PLANE);
    }
}

void Aldente::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Check for a key press
    if (action == GLFW_PRESS)
    {
        keys[key] = true;
        switch (key) {
            // Check if escape was pressed
            case GLFW_KEY_ESCAPE:
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_Q:
                debug_shadows = !debug_shadows;
                break;
            case GLFW_KEY_X:
                shadows_on = !shadows_on;
                break;
            case GLFW_KEY_G:
                god_mode = !god_mode;
                break;
            default:
                break;
        }
    }
    else if (action == GLFW_RELEASE)
    {
        keys[key] = false;
    }
}

void Aldente::cursor_position_callback(GLFWwindow* window, double x_pos, double y_pos)
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glm::vec3 current_cursor_pos(x_pos, y_pos, 0);

    // First movement detected.
    if (!mouse_moved)
    {
        mouse_moved = true;
        last_cursor_pos = current_cursor_pos;
        return;
    }

    glm::vec3 cursor_delta = current_cursor_pos - last_cursor_pos;
    if (lmb_down && keys[GLFW_KEY_LEFT_CONTROL]) {
        int dir = cursor_delta.x > 0 ? 1 : -1;
        float rot_angle = dir * glm::length(cursor_delta) * 0.001f;
        scene->light_pos = glm::vec3(glm::rotate(glm::mat4(1.0f), rot_angle, glm::vec3(0.f, 0.f, 1.f)) * glm::vec4(scene->light_pos, 1.0f));

        /*
           float angle;
        // Horizontal rotation
        angle = (float)(cursor_delta.x) / 100.f;
        camera->cam_pos = glm::vec3(glm::rotate(glm::mat4(1.f), angle, glm::vec3(0.f, 1.f, 0.f)) * glm::vec4(camera->cam_pos, 1.f));
        camera->cam_up = glm::vec3(glm::rotate(glm::mat4(1.f), angle, glm::vec3(0.f, 1.f, 0.f)) * glm::vec4(camera->cam_up, 1.f));

        // Vertical rotation
        angle = (float)(-cursor_delta.y) / 100.f;
        glm::vec3 axis = glm::cross((camera->cam_pos - (camera->cam_pos + camera->cam_front)), camera->cam_up);
        camera->cam_pos = glm::vec3(glm::rotate(glm::mat4(1.f), angle, axis) * glm::vec4(camera->cam_pos, 1.f));
        camera->cam_up = glm::vec3(glm::rotate(glm::mat4(1.f), angle, axis) * glm::vec4(camera->cam_up, 1.f));
        camera->cam_front = glm::normalize(-camera->cam_pos);

        camera->recalculate();
        */
    }
    else if (!keys[GLFW_KEY_LEFT_CONTROL]) {
        // Look around.
        GLfloat xoffset = cursor_delta.x;
        GLfloat yoffset = cursor_delta.y;
        GLfloat sensitivity = 0.5;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        if (current_cursor_pos.x > width - EDGE_PAN_THRESH)
            xoffset = EDGE_PAN_SPEED;
        else if (current_cursor_pos.x < EDGE_PAN_THRESH)
            xoffset = -EDGE_PAN_SPEED;

        camera->yaw += xoffset;
        camera->pitch += yoffset;

        if (camera->pitch > 89.0f)
            camera->pitch = 89.0f;
        if (camera->pitch < -89.0f)
            camera->pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
        front.y = -sin(glm::radians(camera->pitch));
        front.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
        camera->cam_front = glm::normalize(front);
        camera->recalculate();
    }

	// Constructing the ray for picking
	// The ray Start and End positions, in Normalized Device Coordinates (Have you read Tutorial 4 ?)
	float ypos = height - (float)y_pos;
	glm::vec4 lRayStart_NDC(
		((float)x_pos / (float)width - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
		((float)ypos / (float)height - 0.5f) * 2.0f, // [0, 768] -> [-1,1]
		-1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
		1.0f
	);
	glm::vec4 lRayEnd_NDC(
		((float)x_pos / (float)width - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
		((float)ypos / (float)height - 0.5f) * 2.0f, // [0, 768] -> [-1,1]
		0.0,
		1.0f
	);

	glm::mat4 M = glm::inverse(scene->P * scene->camera->V);
	glm::vec4 lRayStart_world = M * lRayStart_NDC;
	lRayStart_world/=lRayStart_world.w;
	glm::vec4 lRayEnd_world   = M * lRayEnd_NDC  ;
	lRayEnd_world  /=lRayEnd_world.w;

	glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
	lRayDir_world = glm::normalize(lRayDir_world);

	glm::vec3 out_end = glm::vec3(lRayStart_world) + lRayDir_world*1000.0f;

	btCollisionWorld::ClosestRayResultCallback RayCallback(
		btVector3(lRayStart_world.x, lRayStart_world.y, lRayStart_world.z),
		btVector3(out_end.x, out_end.y, out_end.z)
	);
	dynamicsWorld->rayTest(
		btVector3(lRayStart_world.x, lRayStart_world.y, lRayStart_world.z),
		btVector3(out_end.x, out_end.y, out_end.z),
		RayCallback
	);

	if (RayCallback.hasHit()) {
		hover = (Tile*)RayCallback.m_collisionObject->getUserPointer();
	}
	else {
		hover = NULL;
	}

	//fprintf(stderr, "CAM:%f,%f,%f\n", scene->camera->cam_pos.x, scene->camera->cam_pos.y, scene->camera->cam_pos.z);
	//fprintf(stderr, "RAY:%f,%f,%f\n", scene->camera->cam_pos.x, scene->camera->cam_pos.y, scene->camera->cam_pos.z);
	//btCollisionShapegrid->getGrid()[0];
	//fprintf(stderr, "POS:%f,%f,%f\n", scene->camera->cam_pos.x, scene->camera->cam_pos.y, scene->camera->cam_pos.z);
    last_cursor_pos = current_cursor_pos;
}

void Aldente::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double x_pos, y_pos;
    glfwGetCursorPos(window, &x_pos, &y_pos);

    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            if (action == GLFW_PRESS) {
                lmb_down = true;
                last_cursor_pos = glm::vec3(x_pos, y_pos, 0.f);
            }
            else if (action == GLFW_RELEASE) {
                lmb_down = false;
				/*if (hover != NULL) {
					int x = hover->getX();
					int z = hover->getZ();
					if (dynamic_cast<FloorTile*>(hover) == NULL) {
						hover = new FloorTile(x, z);
					}
					else {
						hover = new WallTile(x, z);
					}
				}*/
            }
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            if (action == GLFW_PRESS) {
                rmb_down = true;
                last_cursor_pos = glm::vec3(x_pos, y_pos, 0.f);
            }
            else if (action == GLFW_RELEASE) {
                rmb_down = false;
            }
            break;
        default:
            break;
    }
}

void Aldente::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    glm::vec3 trans_vec = (float)yoffset * glm::normalize(camera->cam_front);
    // Only y is relevant here, -1 is down, +1 is up
    camera->cam_pos = glm::vec3(glm::translate(glm::mat4(1.0f), trans_vec) * glm::vec4(camera->cam_pos, 1.0f));
    camera->recalculate();
}
