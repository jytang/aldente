#include "skybox_shader.h"

#include "model/geometry_generator.h"

#define SKYBOX_DIRECTORY_PATH "assets/skybox/"

void SkyboxShader::set_skybox(std::string skybox_name) {
    if (texture_ids.find(skybox_name) != texture_ids.end())
        current_texture_id = texture_ids[skybox_name];
}

void SkyboxShader::init() {
    // Use cube geometry.
    cube_geometry = GeometryGenerator::generate_cube(2.f);

    // List of skyboxes. Hard-coded for now.
    std::vector<std::string> skyboxes {
            "space"
    };

    // Name of each file, in ppm format. Hard-coded for now.
    std::vector<std::string> faces {
            "/right.ppm", "/left.ppm",
            "/top.ppm", "/bottom.ppm",
            "/back.ppm", "/front.ppm"
    };

    int tex_width, tex_height;
    unsigned char *tex_image;

    // Load and create textures for all skyboxes.
    for (std::string s : skyboxes) {
        GLuint id;
        glGenTextures(1, &id);
        texture_ids[s] = id;
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);

        // Load each face's texture.
        for (unsigned int i = 0; i < faces.size(); ++i) {
            std::string path = SKYBOX_DIRECTORY_PATH + s + faces[i];
            tex_image = Util::loadPPM(path.c_str(), tex_width, tex_height);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB,
                         tex_width, tex_height,
                         0, GL_RGB, GL_UNSIGNED_BYTE,
                         tex_image);
            delete tex_image;
        }

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    // Set default skybox to be the first one.
    set_skybox(skyboxes[0]);
}

void SkyboxShader::pre_draw(SceneInfo &scene_info) {}
void SkyboxShader::post_draw() {}

void SkyboxShader::draw(Mesh *mesh, SceneInfo &scene_info, glm::mat4 to_world) {
    // Turn off depth mask.
    glDepthMask(GL_FALSE);

    // Strip translation from view matrix to make skybox appear infinitely large.
    glm::mat4 view = glm::mat4(glm::mat3(scene_info.camera->V));

    // Send view and projection matrices
    glUniformMatrix4fv(get_uni("view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(get_uni("projection"), 1, GL_FALSE, &scene_info.camera->P[0][0]);

    // Bind active texture.
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(get_uni("skybox"), 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, current_texture_id);

    cube_geometry->bind();
    cube_geometry->draw();
    glBindVertexArray(0);

    // Reset depth mask (on).
    glDepthMask(GL_TRUE);
}
