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

    events::dungeon::player_request_raycast_event.connect([&](glm::vec3 position, glm::vec3 dir, std::function<void(GameObject *bt_hit)> callback) {
        callback(raycast(position, dir));
    });

    events::add_rigidbody_event.connect([&](events::RigidBodyData d) {
        add_rigid(d);
    });

    events::remove_rigidbody_event.connect([&](GameObject *obj) {
        remove_rigid(obj);
    });
}

Physics::~Physics() {}

void Physics::set_scene(Scene *s) {
    scene = s;

    //Make a new dynamicsWorld if scene was not previously used
    if (scene_worlds.count(s) == 0) {
        dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
        dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
        scene_worlds[s] = dynamicsWorld;
    }
    else {
        dynamicsWorld = scene_worlds[s];
    }
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
    collision_detection();
}

void Physics::collision_detection() {
    int num_manifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
    for (int i = 0; i < num_manifolds; i++)
    {
        btPersistentManifold* contact_manifold =  dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
        const btCollisionObject *obj_a = contact_manifold->getBody0();
        const btCollisionObject *obj_b = contact_manifold->getBody1();

        // Send callbacks to both objects' on_collide.
        GameObject *go_a = static_cast<GameObject *>(obj_a->getUserPointer());
        GameObject *go_b = static_cast<GameObject *>(obj_b->getUserPointer());
        go_a->on_collision(go_b);
        go_b->on_collision(go_a); // ORDER OF THESE ON_COLLISION CALLS MAY MATTER HERE. CONSIDER BUFFERING.

        // Determine points at which the objects collide.
        // Unused so far.
        int num_contacts = contact_manifold->getNumContacts();
        for (int j = 0; j < num_contacts; j++)
        {
            btManifoldPoint& pt = contact_manifold->getContactPoint(j);
            if (pt.getDistance() < 0.f)
            {
                const btVector3& ptA = pt.getPositionWorldOnA();
                const btVector3& ptB = pt.getPositionWorldOnB();
                const btVector3& normalOnB = pt.m_normalWorldOnB;
            }
        }
    }
}

GameObject* Physics::raycast(glm::vec3 position, glm::vec3 dir) {
    glm::vec3 out_end = glm::normalize(dir) * 0.6f;
    position.y = 0.5f;
    out_end += position;

    btCollisionWorld::ClosestRayResultCallback RayCallback(
        btVector3(position.x, position.y, position.z),
        btVector3(out_end.x, out_end.y, out_end.z)
    );
    dynamicsWorld->rayTest(
        btVector3(position.x, position.y, position.z),
        btVector3(out_end.x, out_end.y, out_end.z),
        RayCallback
    );

    if (RayCallback.hasHit()) {
        GameObject *bt_hit = static_cast<GameObject*>(RayCallback.m_collisionObject->getUserPointer());
        return bt_hit;
    }
    else {
        return nullptr;
    }
}

void Physics::add_rigid(events::RigidBodyData d) {

    btVector3 pos = btVector3((btScalar)d.position.x, (btScalar)d.position.y, (btScalar)d.position.z);
    btDefaultMotionState *motionstate = new btDefaultMotionState(btTransform(
        btQuaternion().getIdentity(), pos));

    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
        d.mass,                  // mass, in kg. 0 -> Static object, will never move.
        motionstate,
        d.shape,  // collision shape of body
        btVector3(d.inertia.x, d.inertia.y, d.inertia.z)    // local inertia
    );

    btRigidBody *rigidbody = new btRigidBody(rigidBodyCI);

    // Will be used to know which object is picked.
    rigidbody->setUserPointer(d.object);
    d.object->set_rigid(rigidbody);

    dynamicsWorld->addRigidBody(rigidbody);
}

void Physics::remove_rigid(GameObject *obj) {
    dynamicsWorld->removeRigidBody(obj->get_rigid());
    obj->set_rigid(nullptr);
}