#include "model.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <QOpenGLTexture>
#include <QImage>
#include <QOpenGLShaderProgram>
#include <iostream>

QMatrix4x4 aiToQt(const aiMatrix4x4& aiMat) {
    QMatrix4x4 qtMat;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            qtMat(i, j) = aiMat[j][i]; // Transpose the matrix
        }
    }
    return qtMat;
}

QOpenGLTexture* TextureFromFile(const char *path, const std::string &directory, bool gamma)
{
    auto functions = QOpenGLContext::currentContext()->functions();
    auto extraFunctions = QOpenGLContext::currentContext()->extraFunctions();
    std::string filename = std::string(path);

    filename = directory + '\\' + filename;
    QOpenGLTexture* glTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    glTexture->setData(QImage(filename.c_str()));  
    glTexture->bind();
    if(!glTexture->isBound())
    {
        std::cout << "glTexture wasn't bounded" << std::endl;
    }

    glTexture->generateMipMaps();

    return glTexture;
}

Model::Model(std::string path)
{
    this->loadModel(path);
}

void Model::Draw(QOpenGLShaderProgram* shader)
{
    for(unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for(unsigned int vIdx = 0; vIdx < mesh->mNumVertices; vIdx++)
    {
        Vertex vertex;
        QVector3D vector;
        
        vector.setX(mesh->mVertices[vIdx].x);
        vector.setY(mesh->mVertices[vIdx].y);
        vector.setZ(mesh->mVertices[vIdx].z);
        vertex.position = vector;
        
        if (mesh->HasNormals())
        {
            vector.setX(mesh->mNormals[vIdx].x);
            vector.setY(mesh->mNormals[vIdx].y);
            vector.setZ(mesh->mNormals[vIdx].z);
            vertex.normal = vector;
        }
        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            QVector2D vec;
            vec.setX(mesh->mTextureCoords[0][vIdx].x);
            vec.setY(mesh->mTextureCoords[0][vIdx].y);
            vertex.texcoord = vec;
        }
        else
            vertex.texcoord = QVector2D(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    for(unsigned int fIdx = 0; fIdx < mesh->mNumFaces; fIdx++)
    {
        aiFace face = mesh->mFaces[fIdx];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    //TODO:Print status of mesh loading(Which mesh, path, number, sizes, bones)
    if(mesh->HasBones())
    {

        qDebug() << "Bones processing ...";
        for(int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
        {
            auto bone = mesh->mBones[boneIndex];

            std::string name = bone->mName.C_Str();
            int index = -1;
            if (boneIndexMap.find(name) == boneIndexMap.end()) {
                index = bones.size();
                Bone newBone;
                newBone.name = name;
                newBone.offsetMatrix = aiToQt(bone->mOffsetMatrix);
                bones.push_back(newBone);
                boneIndexMap[name] = index;
            } else {
                index = boneIndexMap[name];
            }

            for(int i = 0; i < bone->mNumWeights; ++i)
            {
                auto weightInfo = bone->mWeights[i];
                auto vertexId = weightInfo.mVertexId;
                auto weight = weightInfo.mWeight;
                
                for(int k = 0; k < WEIGHTS_PER_VERTEX; ++k)
                {
                    if(vertices.at(vertexId).weight[k] == 0.0)
                    {
                        vertices.at(vertexId).id[k] = index;
                        vertices.at(vertexId).weight[k] = weight;
                        break;
                    }
                }

            }
        }

    }
    //std::cout << std::endl;
    //for(int a = 0; a < vertices.size(); ++a)
    //{
    //    std::cout << "vID: " << a << std::endl;
    //    
    //    std::cout << "\t[ " << vertices[a].weight[0]  << ", "
    //    << vertices[a].weight[1]  << ", " 
    //    << vertices[a].weight[2]  << ", "
    //    << vertices[a].weight[3]  << " ]" << std::endl;
    //    
    //}
    qDebug() << vertices.size() << " " << mesh->mNumFaces << " Bones data: " << boneIndexMap.size() << " " << bones.size() << "\n";
    return Mesh(vertices, indices, textures, bones);
}


std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    
    std::vector<Texture> textures;
    qDebug() << mat->GetName().C_Str() << " Textures(" << typeName.c_str() << "): " << mat->GetTextureCount(type);
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
    
        if(!skip)
        { 
            Texture texture;
            texture.texture = std::shared_ptr<QOpenGLTexture>(TextureFromFile(str.C_Str(), directory, 0));
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture); 
        }
    }
    return textures;
}


void Model::processNode(aiNode *node, const aiScene *scene)
{
// process all the node’s meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
    
}


void Model::loadModel(std::string path)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of("\\"));
    processNode(scene->mRootNode, scene);
}
