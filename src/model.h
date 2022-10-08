#pragma once

#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class QOpenGLShaderProgram;

class Model
{
public:
    Model(std::string path);
void Draw(QOpenGLShaderProgram* shader);
private:
// model data
    std::vector<Mesh> meshes;
    std::vector<Texture> textures_loaded;
    std::string directory;
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, 
                                              aiTextureType type, 
                                              std::string typeName);
};