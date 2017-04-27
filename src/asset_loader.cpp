#include "asset_loader.h"
#include "aldente_client.h"
#include "util/colors.h"

#include <boost/range.hpp>
#include <boost/filesystem.hpp>

std::map<std::string, Model *> AssetLoader::models;
std::map<std::string, GLuint> AssetLoader::textures;

/*
    Loads all fbx models located in assets/fbx
    (animations and textures attached to file)
*/
void AssetLoader::setup() {
    boost::filesystem::path path = boost::filesystem::path("assets/fbx");

    for (auto &entry : boost::make_iterator_range(boost::filesystem::directory_iterator(path), {})) {

        std::string filepath = std::string("assets/fbx/");
        filepath += entry.path().filename().string();

        Model *model = new Model();
        load(model, filepath);
        models[entry.path().filename().string().c_str()] = model;
    }
}

/*
    Loads a model (assuming .fbx) from the file location,
    along with any textures, animations, and bones that
    is connected to the file.
*/
void AssetLoader::load(Model *model, std::string path) {

    Assimp::Importer import;

    unsigned int processFlags =
        aiProcess_Triangulate // Makes sure we use triangle primitives
        ;

    const aiScene *scene = import.ReadFile(path, processFlags);

    if (!scene) {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }

    std::cerr << "Reading from : " << path << std::endl;

    process_node(model, scene, scene->mRootNode, glm::mat4(1.0f));
}

void AssetLoader::process_node(Model *model, const aiScene *scene, aiNode *node, glm::mat4 model_mat) {

    std::cerr << "Processing Node : " << node->mName.data << std::endl;
    std::cerr << "Number of Animations : " << scene->mNumAnimations << std::endl;

    model_mat = model_mat * convert_matrix(node->mTransformation);

    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *aimesh = scene->mMeshes[node->mMeshes[i]];
        Mesh *mesh = process_mesh(aimesh, scene);
        mesh->to_world = model_mat;
        model->add_mesh(mesh);

        process_bones(model, mesh, aimesh);

        mesh->geometry->populate_buffers();
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        process_node(model, scene, node->mChildren[i], model_mat);
    }
}

Mesh *AssetLoader::process_mesh(aiMesh *mesh, const aiScene *scene) {

    std::string test = mesh->mName.data;
    std::cerr << "Mesh : " << test << std::endl;

    // Geometry handles all the vertex buffers and stuff
    Geometry *geo = new Geometry();

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
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
        } else
            geo->tex_coords.push_back(glm::vec2(0.0f, 0.0f));
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            geo->indices.push_back(face.mIndices[j]);
        }
    }

    //Textures and Materials not yet loaded
    Mesh *final_mesh;
    Material *loadMat = new Material();
    aiMaterial *assimpMat = scene->mMaterials[mesh->mMaterialIndex];
    aiColor3D diffuse(0, 0, 0);
    assimpMat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
    aiColor3D ambient(0, 0, 0);
    assimpMat->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
    aiColor3D specular(0, 0, 0);
    assimpMat->Get(AI_MATKEY_COLOR_SPECULAR, specular);
    float shiny = 0.0f;
    assimpMat->Get(AI_MATKEY_SHININESS, shiny);
    loadMat->ambient = glm::vec3(ambient.r, ambient.g, ambient.b);
    loadMat->diffuse = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
    loadMat->specular = glm::vec3(specular.r, specular.g, specular.b);
    loadMat->shininess = shiny;
    final_mesh = new Mesh(geo, loadMat);
    final_mesh->local_transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    //If textures exist for this mesh
    for (GLuint i = 0; i < assimpMat->GetTextureCount(aiTextureType_DIFFUSE); i++) {
        aiString str;
        assimpMat->GetTexture(aiTextureType_DIFFUSE, i, &str);

        std::string temp(str.C_Str());
        std::size_t found = temp.find_last_of("/\\");
        std::string fileName = temp.substr(found + 1);
        std::string to_pass("assets/textures/");
        to_pass += fileName;

        geo->has_texture = true;
        geo->texture = textures[fileName];
    }

    geo->populate_buffers();
    return final_mesh;
}

void AssetLoader::process_bones (Model *model, Mesh *mesh, aiMesh *aimesh) {
    // Loading in BONES for Rigging

    std::cerr << "Number of Bones : " << aimesh->mNumBones << std::endl;

    Geometry *geo = mesh->geometry;
    geo->has_bones = true;
    geo->bone_ids.resize(geo->vertices.size);
    geo->weights.resize(geo->vertices.size);

    for (int i = 0; i < aimesh->mNumBones; i++) {

        std::cerr << "Bone : " << aimesh->mBones[i]->mName.data << std::endl;

        int bone_index = 0;
        std::string bone_name(aimesh->mBones[i]->mName.data);

        if (model->bone_mapping.find(bone_name) == model->bone_mapping.end()) {
            bone_index = model->bones.size;
            model->bones.push_back(glm::mat4(1.0f));
        }
        else {
            bone_index = model->bone_mapping[bone_name];
        }

        model->bone_mapping[bone_name] = bone_index;
        model->bones[bone_index] = convert_matrix(aimesh->mBones[i]->mOffsetMatrix);

        for (unsigned int j = 0; j < aimesh->mBones[i]->mNumWeights; j++) {
            unsigned int vertex_id = aimesh->mBones[i]->mWeights[j].mVertexId;
            float weight = aimesh->mBones[i]->mWeights[j].mWeight;
            add_bone_to_geo(geo, vertex_id, bone_index, weight);
        }
    }
}


//Use this function to access a model, pass in a path in the form of
//"assets/fbx/the_model_you_want_here.fbx"
Model *AssetLoader::get_model(std::string name) {
    if (models.find(name) == models.end()) {
        std::string error("ERROR: Asset ");
        error += name;
        error += " was not loaded. Check for fbx file and double check filename.\n";
        fprintf(stderr, "%s", error.c_str());
        return nullptr;
    }
    return models[name];
}

GLuint AssetLoader::get_texture(std::string name) {
    if (textures.find(name) == textures.end()) {
        std::string error("ERROR: Texture ");
        error += name;
        error += " was not loaded.\n";
        fprintf(stderr, "%s", error.c_str());
        return 0;
    }
    return textures[name];
}

void AssetLoader::load_texture(std::string path) {
    GLuint texture;

    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_TRIANGLES);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_TRIANGLES);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_REPEAT);

    int width, height, channels;
    unsigned char *image = SOIL_load_image(path.c_str(), &width, &height, &channels, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    std::size_t found = path.find_last_of("/\\");
    std::string fileName = path.substr(found + 1);
    textures[fileName] = texture;
}

glm::mat4 AssetLoader::convert_matrix(aiMatrix4x4 ai_mat) {
    return{ ai_mat.a1, ai_mat.a2, ai_mat.a3, ai_mat.a4,
            ai_mat.b1, ai_mat.b2, ai_mat.b3, ai_mat.b4,
            ai_mat.c1, ai_mat.c2, ai_mat.c3, ai_mat.c4,
            ai_mat.d1, ai_mat.d2, ai_mat.d3, ai_mat.d4 };
}

void AssetLoader::add_bone_to_geo(Geometry *geo, unsigned int vertex_id,
                                unsigned int bone_index, float weight) {

    // Finds spot for bone, only possible to have up to four bones attached
    // to a single vertex
    for (int i = 0; i < 4; i++) {
        if (geo->weights[vertex_id][i] == 0.0f) {
            geo->bone_ids[vertex_id][i] = bone_index;
            geo->weights[vertex_id][i] == weight;
            return;
        }
    }
}
