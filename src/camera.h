
#pragma once

#include <QVector3D>
#include <QMatrix4x4>

enum direction {FORWARD = 0, BACKWARD, LEFT, RIGHT, UP};

class Camera3D
{
private:
    QVector3D position;
    QVector3D front;
    QVector3D worldUp;
    QVector3D right;
    QVector3D up;

    QMatrix4x4 viewMatrix;
    
    float yaw = -45.0f;
    float pitch = -30.0f;
    float roll = 0.0f;
    float sensitivity;
    float movementSpeed;
    
    void updateCameraVectors();
public:
    Camera3D(QVector3D position, 
             QVector3D direction, 
             QVector3D worldUp);
    
    const QMatrix4x4 getViewMatrix();

    const QVector3D getPosition() const;
    const QVector3D getDirection() const;
    
    void updateMouseInput(const float& dt, 
                          const double& offsetX, 
                          const double& offsetY);
    void move(const float& dt, const int direction);
};

