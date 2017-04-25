#pragma once

#include "scene/scene.h"
#include "model/model.h"
#include "soil.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

/*
	Class for loading all assets on startup.
*/

class AssetLoader {
private:
    AssetLoader();

    void load(std::string path, bool isModel);

    void process_node(aiNode *node, const aiScene *scene, bool isModel);

    Mesh *process_mesh(aiMesh *mesh, const aiScene *scene);

    void load_texture(std::string path);

    std::map<std::string, Model *> assets;
    std::map<std::string, GLuint> textures;
    Model *model;
    Assimp::Importer import;
public:
    static AssetLoader *asset_loader;

    ~AssetLoader();

    void setup();

    Model *get_model(std::string name);

    GLuint get_texture(std::string name);
};


