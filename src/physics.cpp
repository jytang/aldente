#include "physics.h"
#include "scene/scene.h"
#include "aldente_client.h"

Physics::Physics() {
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
    scene = nullptr;
}

Physics::~Physics() {}

void Physics::set_scene(Scene *s) {
    scene = s;

    currentRigidSignal.disconnect();

    //Make a new dynamicsWorld if scene was not previously used
    if (scene_worlds.count(s) == 0) {
        dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
        dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
        for (btRigidBody* rigid : s->rigids) {
            if(rigid)
                dynamicsWorld->addRigidBody(rigid);
        }
        scene_worlds[s] = dynamicsWorld;
    }
    else {
        dynamicsWorld = scene_worlds[s];
    }

    currentRigidSignal = s->rigidSignal.connect([&](std::pair<bool, btRigidBody*> p) {
        if (p.first) {
            dynamicsWorld->addRigidBody(p.second);
        } else {
            dynamicsWorld->removeRigidBody(p.second);
        }
    });
}

void Physics::raycast_mouse(double xpos, double ypos, int width, int height) {
    // Constructing the ray for picking
    // The ray Start and End positions, in Normalized Device Coordinates (Have you read Tutorial 4 ?)
    ypos = height - ypos;
    glm::vec4 lRayStart_NDC(
            ((float) xpos / (float) width - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
            ((float) ypos / (float) height - 0.5f) * 2.0f, // [0, 768] -> [-1,1]
            -1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
            1.0f
    );
    glm::vec4 lRayEnd_NDC(
            ((float) xpos / (float) width - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
            ((float) ypos / (float) height - 0.5f) * 2.0f, // [0, 768] -> [-1,1]
            0.0,
            1.0f
    );

    glm::mat4 M = glm::inverse(scene->camera.P * scene->camera.V);
    glm::vec4 lRayStart_world = M * lRayStart_NDC;
    lRayStart_world /= lRayStart_world.w;
    glm::vec4 lRayEnd_world = M * lRayEnd_NDC;
    lRayEnd_world /= lRayEnd_world.w;

    glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
    lRayDir_world = glm::normalize(lRayDir_world);

    glm::vec3 out_end = glm::vec3(lRayStart_world) + lRayDir_world * 1000.0f;

    btCollisionWorld::ClosestRayResultCallback RayCallback(
            btVector3(lRayStart_world.x, lRayStart_world.y, lRayStart_world.z),
            btVector3(out_end.x, out_end.y, out_end.z)
    );
    dynamicsWorld->rayTest(
            btVector3(lRayStart_world.x, lRayStart_world.y, lRayStart_world.z),
            btVector3(out_end.x, out_end.y, out_end.z),
            RayCallback
    );

    // TODO: Change this such that it uses the event system to pass which tile
    // is currently being hovered over.

    if (RayCallback.hasHit()) {
        hover = (Tile *) RayCallback.m_collisionObject->getUserPointer();
    } else {
        hover = NULL;
    }

    //fprintf(stderr, "CAM:%f,%f,%f\n", scene->camera->cam_pos.x, scene->camera->cam_pos.y, scene->camera->cam_pos.z);
    //fprintf(stderr, "RAY:%f,%f,%f\n", scene->camera->cam_pos.x, scene->camera->cam_pos.y, scene->camera->cam_pos.z);
    //btCollisionShapegrid->getGrid()[0];
    //fprintf(stderr, "POS:%f,%f,%f\n", scene->camera->cam_pos.x, scene->camera->cam_pos.y, scene->camera->cam_pos.z);

}

void Physics::update() {
    //Step in simulation
    dynamicsWorld->stepSimulation(1.f / 60.f, 10);
}
