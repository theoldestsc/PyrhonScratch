#include "openGLWidget.h"
#include "model.h"
#include <QOpenGlContext>
#include <QOpenGlFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLExtraFunctions>
#include <QMatrix4x4>
#include <QVector3D>
#include <QKeyEvent>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

void MyGLWidget::initializeGL()
{
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
    auto functions = this->context()->functions();
    auto extraFunctions = this->context()->extraFunctions();
    this->installEventFilter(this);
    camera = std::shared_ptr<Camera3D>(new Camera3D({-3.5f, 3.0f, 3.0f}, 
                                       {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}));

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

    qDebug() << "Loading mesh";
    //model = new Model("C:\\Users\\andreyp\\Downloads\\backpack\\backpack.obj");
    //model = new Model("C:\\Users\\andreyp\\Downloads\\Animation Tutorial Download\\WorkingOBJ\\Character Running.obj");
    model = new Model("C:\\Users\\andreyp\\Downloads\\Animation Tutorial Download\\modelCUBE2.dae");
    

};
    
void MyGLWidget::resizeGL(int w, int h)
{    
    auto functions = this->context()->functions();
    functions->glViewport(0, 0, w, h);
};

void MyGLWidget::paintGL()
{
    
    auto functions = this->context()->functions();
    functions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    QMatrix4x4 projection;
    float aspectRatio = (float)this->width()/(float)this->height();
    projection.perspective(45.0f, aspectRatio, 0.1f, 100.0f);
    QMatrix4x4 translationMatrix;
    translationMatrix.rotate(45.0f, QVector3D(0.0f,1.0f,0.0f));
    
    currentShaderProgram->bind();
    currentShaderProgram->setUniformValue("ModelMatrix", translationMatrix);
    currentShaderProgram->setUniformValue("ViewMatrix", camera->getViewMatrix());
    currentShaderProgram->setUniformValue("ProjectionMatrix", projection);
    model->Draw(currentShaderProgram);
    currentShaderProgram->release();
};

void MyGLWidget::keyPressEvent(QKeyEvent *ev)
{
    const float cameraSpeed = 0.25f;

    if(ev->key() == Qt::Key_W)
        camera->move(cameraSpeed, 0);
    else if(ev->key() == Qt::Key_S)
        camera->move(cameraSpeed, 1);
    else if(ev->key() == Qt::Key_A)
        camera->move(cameraSpeed, 2); 
    else if(ev->key() == Qt::Key_D)
        camera->move(cameraSpeed, 3);
    else if(ev->key() == Qt::Key_E)
        camera->move(cameraSpeed, 4);
}

bool MyGLWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
      QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
      lastX = mouseEvent->pos().x();
      lastY = mouseEvent->pos().y();
    }
    if (event->type() == QEvent::MouseMove)
    {
      QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); 
      float xoffset = mouseEvent->pos().x() - lastX;
      float yoffset = lastY - mouseEvent->pos().y();
      lastX = mouseEvent->pos().x();
      lastY = mouseEvent->pos().y();
      float angleX = 1.0 * xoffset/2;
      float angleY = 1.0 * yoffset/2;
      camera->updateMouseInput(1, xoffset, yoffset);
    }
    return false;
}

MyGLWidget::~MyGLWidget()
{
    
}
