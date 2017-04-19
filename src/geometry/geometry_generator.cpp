#include "geometry_generator.h"

std::vector<Geometry *> GeometryGenerator::geometries;

void GeometryGenerator::clean_up()
{
    for (auto it = geometries.begin(); it != geometries.end(); ++it)
        delete(*it);
}

Geometry * GeometryGenerator::generate_cube(GLfloat scale, bool has_normals)
{
    Geometry *cube = new Geometry();

    glm::vec3 v0 = { scale / 2.f, scale / 2.f, scale / 2.f };
    glm::vec3 v1 = { -scale / 2.f, scale / 2.f, scale / 2.f };
    glm::vec3 v2 = { scale / 2.f, scale / 2.f, -scale / 2.f };
    glm::vec3 v3 = { -scale / 2.f, scale / 2.f, -scale / 2.f };
    glm::vec3 v4 = { scale / 2.f, -scale / 2.f, scale / 2.f };
    glm::vec3 v5 = { -scale / 2.f, -scale / 2.f, scale / 2.f };
    glm::vec3 v6 = { scale / 2.f, -scale / 2.f, -scale / 2.f };
    glm::vec3 v7 = { -scale / 2.f, -scale / 2.f, -scale / 2.f };

    cube->vertices.push_back(v0);
    cube->vertices.push_back(v2);
    cube->vertices.push_back(v1);
    cube->vertices.push_back(v1);
    cube->vertices.push_back(v2);
    cube->vertices.push_back(v3);

    cube->vertices.push_back(v2);
    cube->vertices.push_back(v6);
    cube->vertices.push_back(v3);
    cube->vertices.push_back(v3);
    cube->vertices.push_back(v6);
    cube->vertices.push_back(v7);

    cube->vertices.push_back(v1);
    cube->vertices.push_back(v3);
    cube->vertices.push_back(v5);
    cube->vertices.push_back(v3);
    cube->vertices.push_back(v7);
    cube->vertices.push_back(v5);

    cube->vertices.push_back(v0);
    cube->vertices.push_back(v4);
    cube->vertices.push_back(v2);
    cube->vertices.push_back(v2);
    cube->vertices.push_back(v4);
    cube->vertices.push_back(v6);

    cube->vertices.push_back(v1);
    cube->vertices.push_back(v5);
    cube->vertices.push_back(v0);
    cube->vertices.push_back(v0);
    cube->vertices.push_back(v5);
    cube->vertices.push_back(v4);

    cube->vertices.push_back(v5);
    cube->vertices.push_back(v7);
    cube->vertices.push_back(v4);
    cube->vertices.push_back(v4);
    cube->vertices.push_back(v7);
    cube->vertices.push_back(v6);

    if (has_normals)
    {
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
    }

    for (int i = 0; i < cube->vertices.size(); ++i)
        cube->indices.push_back(i);

    cube->has_normals = has_normals;
    cube->populate_buffers();
    geometries.push_back(cube);
    return cube;
}

Geometry * GeometryGenerator::generate_sphere(GLfloat radius, GLuint divisions)
{

    Geometry *sphere = new Geometry();

    float fstacks = (float) divisions;
    float fslices = (float) divisions;
    float pi = glm::pi<float>();

    //From Piazza
    for (unsigned int i = 0; i < divisions; i++)
    {
        for (unsigned int j = 0; j < divisions; j++)
        {
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

Geometry * GeometryGenerator::generate_cylinder(GLfloat radius, GLfloat height, GLuint divisions, bool is_centered)
{
    Geometry *cylinder = new Geometry();

    glm::vec3 v_top, v_bot, v0, v1, v2, v3;

    //Make Top and Bottom
    if (is_centered) //Origin at center
    {
        v_top = { 0, height / 2, 0 };
        v_bot = { 0, -height / 2, 0 };
    }
    else //Origin at botoom
    {
        v_top = { 0, height, 0 };
        v_bot = { 0, 0, 0 };
    }


    float step = (2 * glm::pi<float>()) / divisions;

    //Goes Slice by Slice
    for (float i = 0; i <= divisions; i++)
    {
        float theta = step * i;

        if (is_centered)
        {
            v0 = { radius * cos(theta), height / 2, radius * sinf(theta) };
            v1 = { radius * cos(theta + step), height / 2, radius * sinf(theta + step) };
            v2 = { radius * cos(theta), -height / 2, radius * sinf(theta) };
            v3 = { radius * cos(theta + step), -height / 2, radius * sinf(theta + step) };
        }
        else
        {
            v0 = { radius * cos(theta), height, radius * sinf(theta) };
            v1 = { radius * cos(theta + step), height, radius * sinf(theta + step) };
            v2 = { radius * cos(theta), 0, radius * sinf(theta) };
            v3 = { radius * cos(theta + step), 0, radius * sinf(theta + step) };
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

Geometry * GeometryGenerator::generate_plane(GLfloat scale, int texture_type)
{
    Geometry *plane = new Geometry();

    if (texture_type == WATER)
    {
        //Gotta go Square by Square for a texture

        float divisions = 10.f;
        float step = (scale * 2) / divisions;

        for (float i = -scale; i <= scale - step; i += step)
        {
            for (float j = -scale; j <= scale - step; j += step)
            {
                glm::vec3 v0 = { i, 0, j }; //Top Left
                glm::vec3 v1 = { i + step, 0, j }; //Top Right
                glm::vec3 v2 = { i, 0, j + step }; //Bottom Left
                glm::vec3 v3 = { i + step, 0, j + step }; //Bottom Right

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

        plane->attachNewTexture("assets/textures/WaterWW.png");
        plane->add_texture_noise = true;
    }
    else
    {
        //Setting Y-Offset as 0, change by Rotating World
        glm::vec3 v0 = { scale, 0, scale };
        glm::vec3 v1 = { -scale, 0, scale };
        glm::vec3 v2 = { scale, 0, -scale };
        glm::vec3 v3 = { -scale, 0, -scale };

        plane->vertices.push_back(v0);
        plane->vertices.push_back(v2);
        plane->vertices.push_back(v1);
        plane->vertices.push_back(v1);
        plane->vertices.push_back(v2);
        plane->vertices.push_back(v3);
    }

    glm::vec3 n = { 0, 1.0f, 0 };

    for (int i = 0; i < plane->vertices.size(); i++)
        plane->normals.push_back(n);

    for (int i = 0; i < plane->vertices.size(); i++)
        plane->indices.push_back(i);

    plane->populate_buffers();
    geometries.push_back(plane);

    return plane;
}

Geometry * GeometryGenerator::generate_bezier_plane(GLfloat radius, GLuint num_curves, GLuint segmentation, GLfloat waviness, int texture_type, unsigned int seed = 0)
{
    Geometry *bez_plane = new Geometry();
    bez_plane->draw_type = GL_TRIANGLE_FAN;

    // Make bezier curves
    if (seed != 0)
        srand(seed);
    int num_points = num_curves * 3;
    std::vector<glm::vec3> control_points(num_points);
    for (int i = 0; i < num_points; ++i)
    {
        if (i % 3 == 0) continue; // do interpolated points later
        float offset = ((float)rand()/(float)RAND_MAX) * (radius / (1/waviness)) - (radius / (2/waviness));
        float x = radius * glm::cos(glm::radians(i * 360.f / num_points)) + offset;
        offset = ((float)rand() / (float)RAND_MAX) * (radius / (1/waviness)) - (radius / (2/waviness));
        float z = radius * glm::sin(glm::radians(i * 360.f / num_points)) + offset;
        float y = 0;
        control_points[i] = glm::vec3(x, y, -z);
    }
    // Interpolated points as midpoints
    for (int i = 0; i < num_points; i += 3)
        control_points[i] = 0.5f * (control_points[i - 1 > 0 ? i - 1 : num_points - 1] + control_points[i + 1]);

    // Calculate vertices
    bez_plane->vertices.push_back(glm::vec3(0.f)); // centered at origin
    for (unsigned int i = 0; i < num_curves; ++i)
    {
        int off = 3 * i;
        glm::mat4 bezier_mat = Util::calc_bezier_mat(control_points[off],
                control_points[off + 1],
                control_points[off + 2],
                control_points[(off + 3) % num_points]);

        for (unsigned int j = 0; j <= segmentation; ++j)
        {
            float t = (float) j / (float) segmentation;
            bez_plane->vertices.push_back(glm::vec3(bezier_mat * glm::vec4(t*t*t, t*t, t, 1.f)));
        }
    }

    // Normals
    for (int i = 0; i < bez_plane->vertices.size(); ++i)
        bez_plane->normals.push_back(glm::vec3(0.f, 1.f, 0.f));

    // Indices
    for (int i = 0; i < bez_plane->vertices.size(); ++i)
        bez_plane->indices.push_back(i);

    bez_plane->tex_coords.push_back(glm::vec2(0.5f, 0.f));
    for (int i = 1; i < bez_plane->vertices.size(); i += 2)
    {
        bez_plane->tex_coords.push_back(glm::vec2(0.f, 1.f));
        bez_plane->tex_coords.push_back(glm::vec2(1.f, 1.f));
    }

    if (texture_type == SAND)
        bez_plane->attachNewTexture("assets/textures/SandWW2.dds");
    else if (texture_type == OBSIDIAN)
        bez_plane->attachNewTexture("assets/textures/Obsidian.png");

    bez_plane->populate_buffers();
    geometries.push_back(bez_plane);
    return bez_plane;
}

