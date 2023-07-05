#include "mesh.h"
#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

Mesh::Mesh(std::vector<Vertex> vertices, 
           std::vector<unsigned int> indices, 
           std::vector<Texture> textures,
           std::vector<Bone> bones)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->bones = bones;
    setupMesh();

    /*qDebug() << "Mesh loaded: " << "V: " << vertices.size() 
    << " I: " << indices.size() << " Textures " << textures.size();*/
}


void Mesh::setupMesh()
{
    auto functions = QOpenGLContext::currentContext()->functions();
    auto extraFunctions = QOpenGLContext::currentContext()->extraFunctions();
    
    VAO = std::shared_ptr<QOpenGLVertexArrayObject>(new QOpenGLVertexArrayObject());
    VBO = std::shared_ptr<QOpenGLBuffer>(new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer));
    EBO = std::shared_ptr<QOpenGLBuffer>(new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer));

    VAO->create();
    VBO->create();
    EBO->create();

    VAO->bind();

    VBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
    EBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
    VBO->bind();
    VBO->allocate(vertices.data(), vertices.size() * sizeof(Vertex));
    EBO->bind();
    EBO->allocate(indices.data(), indices.size() * sizeof(unsigned int));
    
    functions->glEnableVertexAttribArray(0);
    functions->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    
    functions->glEnableVertexAttribArray(1);
    functions->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                    (void*)offsetof(Vertex, normal));

    functions->glEnableVertexAttribArray(2);
    functions->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
                                    (void*)offsetof(Vertex, texcoord));
    
    functions->glEnableVertexAttribArray(3);
    functions->glVertexAttribPointer(3, WEIGHTS_PER_VERTEX, GL_FLOAT, GL_TRUE, sizeof(Vertex), 
                                    (void*)offsetof(Vertex, weight));

    functions->glEnableVertexAttribArray(4);
    functions->glVertexAttribPointer(4, WEIGHTS_PER_VERTEX, GL_UNSIGNED_INT, GL_FALSE, sizeof(Vertex), 
                                    (void*)offsetof(Vertex, id));
                                    
    extraFunctions->glBindVertexArray(0);
}

void Mesh::Draw(QOpenGLShaderProgram* shader)
{
    auto functions = QOpenGLContext::currentContext()->functions();
    auto extraFunctions = QOpenGLContext::currentContext()->extraFunctions();

    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    std::vector<QMatrix4x4> vec;
    for(unsigned int i = 0; i < textures.size(); i++)
    {
        functions->glActiveTexture(GL_TEXTURE0 + i); // activate texture unit first
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures[i].type;
        if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "texture_specular")
            number = std::to_string(specularNr++);

        shader->setUniformValue((/*"material." +*/ name + number).c_str(), i);

        textures[i].texture->bind();
    }
    
    for(auto b : bones) 
    {
        vec.push_back(b.offsetMatrix);
    }
    //std::vector<float> boneTransformsArray;
    //boneTransformsArray.reserve(vec.size() * 16); // 16 - размер матрицы 4x4
    //for (const QMatrix4x4& matrix : vec) {
    //    const float* data = matrix.constData();
    //    boneTransformsArray.insert(boneTransformsArray.end(), data, data + 16);
    //}
    shader->setUniformValueArray("gBones", vec.data(), vec.size());
    // draw mesh
    VAO->bind();
    functions->glEnable(GL_SMOOTH);
    //functions->glShadeModel(GL_SMOOTH);
    functions->glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    extraFunctions->glBindVertexArray(0);
    functions->glActiveTexture(GL_TEXTURE0);
}

