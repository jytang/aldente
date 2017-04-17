#include "assetLoader.h"

AssetLoader::AssetLoader()
{
	boost::filesystem::path path = boost::filesystem::path("assets/fbx");
	for (auto& entry : boost::make_iterator_range(boost::filesystem::directory_iterator(path), {})) {
		model = new SceneModel();
		std::string filepath = std::string("assets/fbx/");
		filepath += entry.path().filename().string();
		loadModel(filepath);
		assets[entry.path().filename().string().c_str()] = model;
		//std::cout << entry.path().filename() << "\n";
	}
		
	// Test
}

AssetLoader::~AssetLoader()
{
}

void AssetLoader::loadModel(std::string path)
{
	unsigned int processFlags =
		aiProcess_CalcTangentSpace | // calculate tangents and bitangents if possible
		aiProcess_JoinIdenticalVertices | // join identical vertices/ optimize indexing
										  //aiProcess_ValidateDataStructure  | // perform a full validation of the loader's output
		aiProcess_Triangulate | // Ensure all verticies are triangulated (each 3 vertices are triangle)
		//aiProcess_ConvertToLeftHanded | // convert everything to D3D left handed space (by default right-handed, for OpenGL)
		aiProcess_SortByPType | // ?
		aiProcess_ImproveCacheLocality | // improve the cache locality of the output vertices
		aiProcess_RemoveRedundantMaterials | // remove redundant materials
		aiProcess_FindDegenerates | // remove degenerated polygons from the import
		aiProcess_FindInvalidData | // detect invalid model data, such as invalid normal vectors
		aiProcess_GenUVCoords | // convert spherical, cylindrical, box and planar mapping to proper UVs
		aiProcess_TransformUVCoords | // preprocess UV transformations (scaling, translation ...)
		aiProcess_FindInstances | // search for instanced meshes and remove them by references to one master
		aiProcess_LimitBoneWeights | // limit bone weights to 4 per vertex		
		aiProcess_OptimizeMeshes | // join small meshes, if possible;
		aiProcess_SplitByBoneCount | // split meshes with too many bones. Necessary for our (limited) hardware skinning shader
		aiProcess_PreTransformVertices | //-- fixes the transformation issue.
		0;


	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, processFlags);

	if (!scene)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	//this->directory = path.substr(0, path.find_last_of('/'));
	
	this->processNode(scene->mRootNode, scene);
}

void AssetLoader::processNode(aiNode* node, const aiScene* scene)
{
	// Gets all meshes and adds it to the model

	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		model->add_mesh(this->processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene);
	}
}

Mesh* AssetLoader::processMesh(aiMesh* mesh, const aiScene* scene)
{
	// Geometry handles all the vertex buffers and stuff
	Geometry *geo = new Geometry();

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		glm::vec3 vec;
		vec.x = mesh->mVertices[i].x;
		vec.y = mesh->mVertices[i].y;
		vec.z = mesh->mVertices[i].z;
		geo->vertices.push_back(vec);

		vec.x = mesh->mNormals[i].x;
		vec.y = mesh->mNormals[i].y;
		vec.z = mesh->mNormals[i].z;
		geo->normals.push_back(vec);

		if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			geo->tex_coords.push_back(vec);
		}
		else
			geo->tex_coords.push_back(glm::vec2(0.0f, 0.0f));
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			geo->indices.push_back(face.mIndices[j]);
		}
	}

	//Textures and Materials not yet loaded
	geo->populate_buffers();

	Material test_mat;
	test_mat.diffuse = test_mat.ambient = color::white;
	Mesh* final_mesh = new Mesh{ geo, test_mat, ShaderManager::get_default(), glm::mat4(1.0f) };

	return final_mesh;
}

SceneModel* AssetLoader::getModel(std::string name) {
	return assets[name];
}
