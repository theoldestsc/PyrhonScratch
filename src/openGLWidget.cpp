#include "openGLWidget.h"
#include <QOpenGlContext>
#include <QOpenGlFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLExtraFunctions>
#include <QMatrix4x4>
#include <QVector3D>


void MyGLWidget::initializeGL()
{
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
    auto functions = this->context()->functions();
    auto extraFunctions = this->context()->extraFunctions();
    camera = std::shared_ptr<Camera3D>(
            new Camera3D({-3.5f, 3.0f, 3.0f}, 
            {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}));

    currentShaderProgram = new QOpenGLShaderProgram;
    
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
    currentShaderProgram->addShader(&vertexShader);
    currentShaderProgram->addShader(&fragmentShader);
    currentShaderProgram->link();

    float vertices[] = {
                    -0.5f, -0.5f, -0.5f,
                    0.5f, -0.5f, -0.5f,
                    0.5f, 0.5f, -0.5f,
                    0.5f, 0.5f, -0.5f,
                    -0.5f, 0.5f, -0.5f,
                    -0.5f, -0.5f, -0.5f,
                    -0.5f, -0.5f, 0.5f,
                    0.5f, -0.5f, 0.5f,
                    0.5f, 0.5f, 0.5f,
                    0.5f, 0.5f, 0.5f,
                    -0.5f, 0.5f, 0.5f,
                    -0.5f, -0.5f, 0.5f,
                    -0.5f, 0.5f, 0.5f,
                    -0.5f, 0.5f, -0.5f,
                    -0.5f, -0.5f, -0.5f,
                    -0.5f, -0.5f, -0.5f,
                    -0.5f, -0.5f, 0.5f,
                    -0.5f, 0.5f, 0.5f,
                    0.5f, 0.5f, 0.5f,
                    0.5f, 0.5f, -0.5f,
                    0.5f, -0.5f, -0.5f,
                    0.5f, -0.5f, -0.5f,
                    0.5f, -0.5f, 0.5f,
                    0.5f, 0.5f, 0.5f,
                    -0.5f, -0.5f, -0.5f, 
                    0.5f, -0.5f, -0.5f, 
                    0.5f, -0.5f, 0.5f, 
                    0.5f, -0.5f, 0.5f, 
                    -0.5f, -0.5f, 0.5f, 
                    -0.5f, -0.5f, -0.5f, 
                    -0.5f, 0.5f, -0.5f, 
                    0.5f, 0.5f, -0.5f,
                    0.5f, 0.5f, 0.5f,
                    0.5f, 0.5f, 0.5f, 
                    -0.5f, 0.5f, 0.5f, 
                    -0.5f, 0.5f, -0.5f, 
};


    unsigned int VBO;
    extraFunctions->glGenVertexArrays(1, &VAO);
    extraFunctions->glBindVertexArray(VAO);


    functions->glGenBuffers(1, &VBO);
    functions->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    functions->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    functions->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),(void*)0);
    functions->glEnableVertexAttribArray(0);


};
    
void MyGLWidget::resizeGL(int w, int h)
{    

};

void MyGLWidget::paintGL()
{
    
    auto functions = this->context()->functions();
    functions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int size;

    QMatrix4x4 projection;
    projection.perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
    QMatrix4x4 translationMatrix;
    translationMatrix.translate(QVector3D(0.5f, -0.5f, 0.0f));
    
    
    currentShaderProgram->setUniformValue("model", translationMatrix);
    currentShaderProgram->setUniformValue("projection", projection);
    currentShaderProgram->setUniformValue("view", camera->getViewMatrix());
    
    currentShaderProgram->bind();
    functions->glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    functions->glDrawArrays(GL_TRIANGLES, 0, size);
    
};

MyGLWidget::~MyGLWidget()
{
    
}
