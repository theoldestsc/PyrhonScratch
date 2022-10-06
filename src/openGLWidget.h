#ifndef WIDGET_H
#define WIDGET_H

#include "camera.h"
#include <QOpenGLWidget>
#include <QVector3D>

class QOpenGLBuffer;
class QOpenGLVertexArrayObject;
class QOpenGLShaderProgram;
class QKeyEvent;

class MyGLWidget : public QOpenGLWidget
{
public:
    MyGLWidget(QWidget *parent = nullptr) : QOpenGLWidget(parent) { };
    ~MyGLWidget();
private:
    QOpenGLShaderProgram* currentShaderProgram = nullptr;
    std::shared_ptr<QOpenGLVertexArrayObject> VAO;
    std::shared_ptr<QOpenGLBuffer> VBO;
    std::shared_ptr<Camera3D> camera;
    float lastX;
    float lastY;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    bool eventFilter(QObject *obj, QEvent *event);
    void keyPressEvent(QKeyEvent *ev);

};

#endif // WIDGET_H
