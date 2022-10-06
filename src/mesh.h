#include "vertex.h"
#include "texture.h"
#include <vector>
#include <QOpenGLShaderProgram>

class QOpenGLVertexArrayObject;
class QOpenGLBuffer;

class Mesh {
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        void Draw(QOpenGLShaderProgram* shader);
    private:
        std::shared_ptr<QOpenGLVertexArrayObject> VAO;
        std::shared_ptr<QOpenGLBuffer> VBO, EBO;
        void setupMesh();
};