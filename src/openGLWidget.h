#ifndef WIDGET_H
#define WIDGET_H
#include "camera.h"
#include <QOpenGLWidget>

class QVector3D;
class QOpenGLShaderProgram;
class QKeyEvent;
class Model;

class MyGLWidget : public QOpenGLWidget
{
public:
    MyGLWidget(QWidget *parent = nullptr) : QOpenGLWidget(parent) { };
    ~MyGLWidget();
private:
    QOpenGLShaderProgram* currentShaderProgram = nullptr;
    
    std::shared_ptr<Camera3D> camera;
    float lastX;
    float lastY;
    Model* model;
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    bool eventFilter(QObject *obj, QEvent *event);
    void keyPressEvent(QKeyEvent *ev);

};

#endif // WIDGET_H
