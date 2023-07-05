#pragma once

#include "vertex.h"
#include "texture.h"
#include "bone.h"
#include <vector>

class QOpenGLShaderProgram;
class QOpenGLVertexArrayObject;
class QOpenGLBuffer;

class Mesh {
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        std::vector<Bone> bones;

        Mesh(std::vector<Vertex> vertices, 
             std::vector<unsigned int> indices, 
             std::vector<Texture> textures,
             std::vector<Bone> bones);
        void Draw(QOpenGLShaderProgram* shader);
        
    private:
        std::shared_ptr<QOpenGLVertexArrayObject> VAO;
        std::shared_ptr<QOpenGLBuffer> VBO, EBO;
        void setupMesh();
};