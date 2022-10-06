#include "openGLWidget.h"
#include <QOpenGlContext>
#include <QOpenGlFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>

void MyGLWidget::initializeGL()
{
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
    auto functions = this->context()->functions();
    
    functions->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    functions->glViewport(this->geometry().x(),
                          this->geometry().y(),
                          this->geometry().width(),
                          this->geometry().height());
    functions->glEnable(GL_DEPTH_TEST);

    QOpenGLShader vertexShader(QOpenGLShader::Vertex);
    vertexShader.compileSourceFile(":/shaders/commonShader.vs");
    QOpenGLShader fragmentShader(QOpenGLShader::Fragment);
    fragmentShader.compileSourceFile(":/shaders/commonShader.fs");
    QOpenGLShaderProgram shaderProgram;
    shaderProgram.addShader(&vertexShader);
    shaderProgram.addShader(&fragmentShader);
    shaderProgram.link();

};
    
void MyGLWidget::resizeGL(int w, int h)
{    

};

void MyGLWidget::paintGL()
{

};

MyGLWidget::~MyGLWidget()
{
    
}
