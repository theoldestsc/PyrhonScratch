#include "mesh.h"
#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    setupMesh();

    qDebug() << "Mesh loaded: " << "V: " << vertices.size() 
    << " I: " << indices.size() << " Textures " << textures.size();
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
    extraFunctions->glBindVertexArray(0);
}

void Mesh::Draw(QOpenGLShaderProgram* shader)
{
    auto functions = QOpenGLContext::currentContext()->functions();
    auto extraFunctions = QOpenGLContext::currentContext()->extraFunctions();

    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
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
        functions->glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    
    // draw mesh
    VAO->bind();
    functions->glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    extraFunctions->glBindVertexArray(0);
    functions->glActiveTexture(GL_TEXTURE0);
}

