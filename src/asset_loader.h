#pragma once

#include "scene/scene.h"
#include "model/model.h"
#include "SOIL.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

/*
    Class for loading all assets on startup.
*/

class AssetLoader {
private:
    static Assimp::Importer import;

    static std::map<std::string, Model *> models;
    static std::map<std::string, GLuint> textures;

    static void load(std::string file_loc, std::string file_name);

    static void load_texture(std::string path);

    static void process_node(Model *model, Skeleton *skel, const aiScene *scene, aiNode *node, glm::mat4 model_mat);
    static Mesh *process_mesh(aiMesh *mesh, const aiScene *scene);
    static void process_bones(Skeleton *skel, Mesh *mesh, aiMesh *aimesh);

    static void add_bone_to_geo(Geometry *geo, unsigned int vertex_id, unsigned int bone_index, float weight);
    static glm::mat4 convert_ai_matrix(aiMatrix4x4 ai_mat);
public:
    static void setup();
    static void destroy();

    static Model *get_model(std::string name);
    static GLuint get_texture(std::string name);
};