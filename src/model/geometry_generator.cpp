#include "geometry_generator.h"

std::vector<Geometry *> GeometryGenerator::geometries;

void GeometryGenerator::destroy() {
    for (auto it = geometries.begin(); it != geometries.end(); ++it)
        delete (*it);
}

Geometry *GeometryGenerator::generate_cube(GLfloat scale, int num_instances) {
    Geometry *cube = new Geometry(num_instances);

    glm::vec3 v0 = {scale / 2.f, scale / 2.f, scale / 2.f};
    glm::vec3 v1 = {-scale / 2.f, scale / 2.f, scale / 2.f};
    glm::vec3 v2 = {scale / 2.f, scale / 2.f, -scale / 2.f};
    glm::vec3 v3 = {-scale / 2.f, scale / 2.f, -scale / 2.f};
    glm::vec3 v4 = {scale / 2.f, -scale / 2.f, scale / 2.f};
    glm::vec3 v5 = {-scale / 2.f, -scale / 2.f, scale / 2.f};
    glm::vec3 v6 = {scale / 2.f, -scale / 2.f, -scale / 2.f};
    glm::vec3 v7 = {-scale / 2.f, -scale / 2.f, -scale / 2.f};

    // Top Face
    cube->vertices.push_back(v0);
    cube->vertices.push_back(v2);
    cube->vertices.push_back(v1);
    cube->vertices.push_back(v1);
    cube->vertices.push_back(v2);
    cube->vertices.push_back(v3);

    cube->tex_coords.push_back(glm::vec2(0, 0));
    cube->tex_coords.push_back(glm::vec2(0, 0));
    cube->tex_coords.push_back(glm::vec2(0, 0));
    cube->tex_coords.push_back(glm::vec2(0, 0));
    cube->tex_coords.push_back(glm::vec2(0, 0));
    cube->tex_coords.push_back(glm::vec2(0, 0));


    // Back Face
    cube->vertices.push_back(v2);
    cube->vertices.push_back(v6);
    cube->vertices.push_back(v3);
    cube->vertices.push_back(v3);
    cube->vertices.push_back(v6);
    cube->vertices.push_back(v7);

    cube->tex_coords.push_back(glm::vec2(1.f, 1.f));
    cube->tex_coords.push_back(glm::vec2(1.f, 0));
    cube->tex_coords.push_back(glm::vec2(0, 1.f));
    cube->tex_coords.push_back(glm::vec2(0, 1.f));
    cube->tex_coords.push_back(glm::vec2(1.f, 0));
    cube->tex_coords.push_back(glm::vec2(0, 0));

    // Left Face
    cube->vertices.push_back(v3);
    cube->vertices.push_back(v7);
    cube->vertices.push_back(v1);
    cube->vertices.push_back(v1);
    cube->vertices.push_back(v7);
    cube->vertices.push_back(v5);

    cube->tex_coords.push_back(glm::vec2(1.f, 1.f));
    cube->tex_coords.push_back(glm::vec2(1.f, 0));
    cube->tex_coords.push_back(glm::vec2(0, 1.f));
    cube->tex_coords.push_back(glm::vec2(0, 1.f));
    cube->tex_coords.push_back(glm::vec2(1.f, 0));
    cube->tex_coords.push_back(glm::vec2(0, 0));

    // Right Face
    cube->vertices.push_back(v0);
    cube->vertices.push_back(v4);
    cube->vertices.push_back(v2);
    cube->vertices.push_back(v2);
    cube->vertices.push_back(v4);
    cube->vertices.push_back(v6);

    cube->tex_coords.push_back(glm::vec2(1.f, 1.f));
    cube->tex_coords.push_back(glm::vec2(1.f, 0));
    cube->tex_coords.push_back(glm::vec2(0, 1.f));
    cube->tex_coords.push_back(glm::vec2(0, 1.f));
    cube->tex_coords.push_back(glm::vec2(1.f, 0));
    cube->tex_coords.push_back(glm::vec2(0, 0));

    // Front Face
    cube->vertices.push_back(v1);
    cube->vertices.push_back(v5);
    cube->vertices.push_back(v0);
    cube->vertices.push_back(v0);
    cube->vertices.push_back(v5);
    cube->vertices.push_back(v4);

    cube->tex_coords.push_back(glm::vec2(1.f, 1.f));
    cube->tex_coords.push_back(glm::vec2(1.f, 0));
    cube->tex_coords.push_back(glm::vec2(0, 1.f));
    cube->tex_coords.push_back(glm::vec2(0, 1.f));
    cube->tex_coords.push_back(glm::vec2(1.f, 0));
    cube->tex_coords.push_back(glm::vec2(0, 0));

    // Bottom Face
    cube->vertices.push_back(v5);
    cube->vertices.push_back(v7);
    cube->vertices.push_back(v4);
    cube->vertices.push_back(v4);
    cube->vertices.push_back(v7);
    cube->vertices.push_back(v6);

    // Let's assume Bottom Face has no Texture
    cube->tex_coords.push_back(glm::vec2(0, 0));
    cube->tex_coords.push_back(glm::vec2(0, 0));
    cube->tex_coords.push_back(glm::vec2(0, 0));
    cube->tex_coords.push_back(glm::vec2(0, 0));
    cube->tex_coords.push_back(glm::vec2(0, 0));
    cube->tex_coords.push_back(glm::vec2(0, 0));    

    for (int i = 0; i < 6; ++i)
        cube->normals.push_back(glm::vec3(0.f, 1.f, 0.f));
    for (int i = 0; i < 6; ++i)
        cube->normals.push_back(glm::vec3(0.f, 0.f, -1.f));
    for (int i = 0; i < 6; ++i)
        cube->normals.push_back(glm::vec3(-1.f, 0.f, 0.f));
    for (int i = 0; i < 6; ++i)
        cube->normals.push_back(glm::vec3(1.f, 0.f, 0.f));
    for (int i = 0; i < 6; ++i)
        cube->normals.push_back(glm::vec3(0.f, 0.f, 1.f));
    for (int i = 0; i < 6; ++i)
        cube->normals.push_back(glm::vec3(0.f, -1.f, 0.f));

    for (int i = 0; i < cube->vertices.size(); ++i)
        cube->indices.push_back(i);

    cube->populate_buffers();
    geometries.push_back(cube);
    return cube;
}

Geometry *GeometryGenerator::generate_sphere(GLfloat radius, GLuint divisions, int num_instances) {
    Geometry *sphere = new Geometry(num_instances);

    float fstacks = (float) divisions;
    float fslices = (float) divisions;
    float pi = glm::pi<float>();

    //From Piazza
    for (unsigned int i = 0; i < divisions; i++) {
        for (unsigned int j = 0; j < divisions; j++) {
            // Top left
            sphere->vertices.push_back(glm::vec3(
                    radius * -cos(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices),
                    radius * -cos(pi * (j + 1.0f) / fslices),
                    radius * sin(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices)));
            sphere->normals.push_back(glm::normalize(glm::vec3(
                    -cos(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices),
                    -cos(pi * (j + 1.0f) / fslices),
                    sin(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices))));
            sphere->vertices.push_back(glm::vec3(
                    radius * -cos(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices),
                    radius * -cos(pi * j / fslices),
                    radius * sin(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices)));
            sphere->normals.push_back(glm::normalize(glm::vec3(
                    -cos(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices),
                    -cos(pi * j / fslices),
                    sin(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices))));
            sphere->vertices.push_back(glm::vec3(
                    radius * -cos(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * (j + 1.0) / fslices),
                    radius * -cos(pi * (j + 1.0) / fslices),
                    radius * sin(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * (j + 1.0) / fslices)));
            sphere->normals.push_back(glm::normalize(glm::vec3(
                    -cos(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * (j + 1.0) / fslices),
                    -cos(pi * (j + 1.0) / fslices),
                    sin(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * (j + 1.0) / fslices))));

            // Need to repeat 2 of the vertices since we can only draw triangles. Eliminates the confusion
            // of array indices.
            // Top left
            sphere->vertices.push_back(glm::vec3(
                    radius * -cos(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices),
                    radius * -cos(pi * (j + 1.0f) / fslices),
                    radius * sin(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices)));
            sphere->normals.push_back(glm::normalize(glm::vec3(
                    -cos(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices),
                    -cos(pi * (j + 1.0f) / fslices),
                    sin(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices))));
            // Bottom left
            sphere->vertices.push_back(glm::vec3(
                    radius * -cos(2.0f * pi * i / fstacks) * sin(pi * j / fslices),
                    radius * -cos(pi * j / fslices),
                    radius * sin(2.0f * pi * i / fstacks) * sin(pi * j / fslices)));
            sphere->normals.push_back(glm::normalize(glm::vec3(
                    -cos(2.0f * pi * i / fstacks) * sin(pi * j / fslices),
                    -cos(pi * j / fslices),
                    sin(2.0f * pi * i / fstacks) * sin(pi * j / fslices))));
            //Bottom Right
            sphere->vertices.push_back(glm::vec3(
                    radius * -cos(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices),
                    radius * -cos(pi * j / fslices),
                    radius * sin(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices)));
            sphere->normals.push_back(glm::normalize(glm::vec3(
                    -cos(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices),
                    -cos(pi * j / fslices),
                    sin(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices))));

        }
    }

    for (int i = 0; i < sphere->vertices.size(); i++)
        sphere->indices.push_back(i);

    sphere->populate_buffers();
    geometries.push_back(sphere);

    return sphere;
}

Geometry *GeometryGenerator::generate_cylinder(GLfloat radius, GLfloat height, GLuint divisions, bool is_centered, int num_instances) {
    Geometry *cylinder = new Geometry(num_instances);

    glm::vec3 v_top, v_bot, v0, v1, v2, v3;

    //Make Top and Bottom
    if (is_centered) //Origin at center
    {
        v_top = {0, height / 2, 0};
        v_bot = {0, -height / 2, 0};
    } else //Origin at botoom
    {
        v_top = {0, height, 0};
        v_bot = {0, 0, 0};
    }


    float step = (2 * glm::pi<float>()) / divisions;

    //Goes Slice by Slice
    for (float i = 0; i <= divisions; i++) {
        float theta = step * i;

        if (is_centered) {
            v0 = {radius * cos(theta), height / 2, radius * sinf(theta)};
            v1 = {radius * cos(theta + step), height / 2, radius * sinf(theta + step)};
            v2 = {radius * cos(theta), -height / 2, radius * sinf(theta)};
            v3 = {radius * cos(theta + step), -height / 2, radius * sinf(theta + step)};
        } else {
            v0 = {radius * cos(theta), height, radius * sinf(theta)};
            v1 = {radius * cos(theta + step), height, radius * sinf(theta + step)};
            v2 = {radius * cos(theta), 0, radius * sinf(theta)};
            v3 = {radius * cos(theta + step), 0, radius * sinf(theta + step)};
        }

        //Top Portion
        cylinder->vertices.push_back(v_top);
        cylinder->vertices.push_back(v1);
        cylinder->vertices.push_back(v0);
        cylinder->normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        cylinder->normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        cylinder->normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

        //Middle Portion
        cylinder->vertices.push_back(v0);
        cylinder->vertices.push_back(v1);
        cylinder->vertices.push_back(v2);
        cylinder->vertices.push_back(v1);
        cylinder->vertices.push_back(v3);
        cylinder->vertices.push_back(v2);
        cylinder->normals.push_back(glm::normalize(v0));
        cylinder->normals.push_back(glm::normalize(v1));
        cylinder->normals.push_back(glm::normalize(v2));
        cylinder->normals.push_back(glm::normalize(v1));
        cylinder->normals.push_back(glm::normalize(v3));
        cylinder->normals.push_back(glm::normalize(v2));

        //Bottom Portion
        cylinder->vertices.push_back(v_bot);
        cylinder->vertices.push_back(v2);
        cylinder->vertices.push_back(v3);
        cylinder->normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
        cylinder->normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
        cylinder->normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
    }

    //Indices are just in order to make it easier
    for (int i = 0; i < cylinder->vertices.size(); i++)
        cylinder->indices.push_back(i);

    cylinder->populate_buffers();
    geometries.push_back(cylinder);

    return cylinder;

}

Geometry *GeometryGenerator::generate_plane(GLfloat scale, bool has_texture, int num_instances) {
    Geometry *plane = new Geometry(num_instances);
    
        // Done square by square for repeatable texture

    glm::vec3 v0 = { -scale / 2.f, 0, -scale / 2.f }; //Top Left
    glm::vec3 v1 = { scale / 2.f , 0, -scale / 2.f }; //Top Right
    glm::vec3 v2 = { -scale / 2.f, 0, scale / 2.f }; //Bottom Left
    glm::vec3 v3 = { scale / 2.f, 0, scale / 2.f }; //Bottom Right

    plane->vertices.push_back(v0);
    plane->vertices.push_back(v2);
    plane->vertices.push_back(v1);
    plane->vertices.push_back(v1);
    plane->vertices.push_back(v2);
    plane->vertices.push_back(v3);

    plane->tex_coords.push_back(glm::vec2(0, 0));
    plane->tex_coords.push_back(glm::vec2(0, 1.f));
    plane->tex_coords.push_back(glm::vec2(1.f, 0));
    plane->tex_coords.push_back(glm::vec2(1.f, 0));
    plane->tex_coords.push_back(glm::vec2(0, 1.f));
    plane->tex_coords.push_back(glm::vec2(1.f, 1.f));

        /*
        float divisions = 10.f;
        float step = (scale * 2) / divisions;

        for (float i = -scale; i <= scale - step; i += step) {
            for (float j = -scale; j <= scale - step; j += step) {
                glm::vec3 v0 = {i, 0, j}; //Top Left
                glm::vec3 v1 = {i + step, 0, j}; //Top Right
                glm::vec3 v2 = {i, 0, j + step}; //Bottom Left
                glm::vec3 v3 = {i + step, 0, j + step}; //Bottom Right

                plane->vertices.push_back(v0);
                plane->vertices.push_back(v2);
                plane->vertices.push_back(v1);
                plane->vertices.push_back(v1);
                plane->vertices.push_back(v2);
                plane->vertices.push_back(v3);

                plane->tex_coords.push_back(glm::vec2(0, 0));
                plane->tex_coords.push_back(glm::vec2(0, 1.f));
                plane->tex_coords.push_back(glm::vec2(1.f, 0));
                plane->tex_coords.push_back(glm::vec2(1.f, 0));
                plane->tex_coords.push_back(glm::vec2(0, 1.f));
                plane->tex_coords.push_back(glm::vec2(1.f, 1.f));
            }
        }
    } else {
        // Rendered as two simple triangles

        //Setting Y-Offset as 0, change by Rotating World
        glm::vec3 v0 = {scale, 0, scale};
        glm::vec3 v1 = {-scale, 0, scale};
        glm::vec3 v2 = {scale, 0, -scale};
        glm::vec3 v3 = {-scale, 0, -scale};

        plane->vertices.push_back(v0);
        plane->vertices.push_back(v2);
        plane->vertices.push_back(v1);
        plane->vertices.push_back(v1);
        plane->vertices.push_back(v2);
        plane->vertices.push_back(v3);
    }

    */

    glm::vec3 n = {0, 1.0f, 0};

    for (int i = 0; i < plane->vertices.size(); i++)
        plane->normals.push_back(n);

    for (int i = 0; i < plane->vertices.size(); i++)
        plane->indices.push_back(i);

    plane->populate_buffers();
    geometries.push_back(plane);

    return plane;
}
