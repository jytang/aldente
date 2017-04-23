#pragma once

#include "scene/scene.h"
#include "model/model.h"

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

    std::map<std::string, Model *> assets;
    std::map<std::string, GLuint> textures;
    Model *model;
    Assimp::Importer import;
public:
    static AssetLoader *asset_loader;

    ~AssetLoader();

    void setup();

    void load(std::string path, bool isModel);

    void processNode(aiNode *node, const aiScene *scene, bool isModel);

    Mesh *processMesh(aiMesh *mesh, const aiScene *scene);

    Model *getModel(std::string name);
};


