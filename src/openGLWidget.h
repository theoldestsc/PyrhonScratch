#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QVector3D>


class MyGLWidget : public QOpenGLWidget
{
public:
    MyGLWidget(QWidget *parent = nullptr) : QOpenGLWidget(parent) { };
    ~MyGLWidget();
private:
    //QOpenGLFunctions_4_5_Core* f = nullptr;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

};

#endif // WIDGET_H
