#pragma once


#include "scene/scene.h"
#include "scene/scene_model.h"
#include "util/colors.h"
#include "shaders/shader_manager.h"

//#include <filesystem>
//#include <boost/filesystem.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

//using namespace boost::filesystem;

class AssetLoader {
private:
	map<string, Mesh*> assets;
public:
	AssetLoader();
	~AssetLoader();

	SceneModel* model;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);

};


