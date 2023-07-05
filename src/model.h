#pragma once

#include "mesh.h"
#include "bone.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <map>
#include <string>
#include <QMatrix4x4>

// Structure for storing animation data
struct Animation {
    std::string name;
    float duration;
    float ticksPerSecond;
    std::map<std::string, std::vector<std::pair<float, QMatrix4x4>>> nodeAnimations;
};

class QOpenGLShaderProgram;

class Model
{
public:
    Model(std::string path);
    void Draw(QOpenGLShaderProgram* shader);
private:
// model data
    std::vector<Mesh> meshes;
    
    std::map<std::string, int> boneIndexMap;
    std::vector<Bone> bones;
    std::vector<Texture> textures_loaded;
    
    std::string directory;
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, 
                                              aiTextureType type, 
                                              std::string typeName);

};


