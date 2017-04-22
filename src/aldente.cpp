#include "aldente.h"

#include "window.h"
#include "setup.h"
#include "asset_loader.h"
#include "physics.h"
#include "shaders/shader_manager.h"
#include "shadows.h"

#include "keyboard.h"

#include "events/input.h"
#include "input/process.h"
#include "util/util.h"

namespace input = kuuhaku::input;
namespace events = kuuhaku::events;

Aldente* Aldente::aldente = new Aldente();

Aldente::Aldente() {}
Aldente::~Aldente() {}

void Aldente::setup_scenes()
{
    // Initial scene and cam.
    main_scene = new MainScene();
	main_scene->setup();
	scene = main_scene;
    scenes.push_back(scene);
    camera = scene->camera;
}

void Aldente::go()
{
    window = Window::window->create_window();
	Setup::setup_callbacks();
    Setup::setup_opengl();
    Setup::setup_shaders();
	Physics::physics->setup_bullet();
	AssetLoader::asset_loader->setup();
    Util::seed(0); // Seed PRNG.
    setup_scenes();

    while (!Window::window->should_close(window))
    {
		Window::window->poll_events();

        input::process();
		Keyboard::keyboard->handle_movement();

		Window::window->update_size(window);
		Physics::physics->update();

		scene->update();

		// First pass: shadowmap.
		Shadows::shadows->shadow_pass(scene);

		// Second pass: usual rendering.
		Window::window->clear_window();
		scene->draw();

        if (debug_shadows)
			Shadows::shadows->debug_shadows();

		Window::window->swap_buffers(window);
    }
    destroy();
}

void Aldente::destroy()
{
	// Free memory here.
	ShaderManager::destroy();
	GeometryGenerator::clean_up();
	Window::window->destroy(window);
}

std::vector<Scene*> Aldente::get_scenes()
{
	return scenes;
}

Scene *Aldente::get_scene()
{
	return scene;
}

SceneCamera *Aldente::get_camera()
{
	return camera;
}

GLFWwindow *Aldente::get_window()
{
	return window;
}

