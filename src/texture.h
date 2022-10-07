#include <string>
#include <QOpenGLTexture>

struct Texture {
    std::shared_ptr<QOpenGLTexture> texture;
    std::string type;
    std::string path;
};