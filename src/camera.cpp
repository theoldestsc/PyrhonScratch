#include "camera.h"

Camera3D::Camera3D(QVector3D position,
                   QVector3D direction, 
                   QVector3D worldUp):position(position), 
                                      front(direction), 
                                      worldUp(worldUp)
{
    this->movementSpeed = 0.25f;
    this->sensitivity = 0.1f;
    this->viewMatrix.setToIdentity();
    this->right = QVector3D(0.0f, 0.0f, 0.0f);
    this->pitch = -30.f;
    this->yaw = -45.f;
    this->roll = 0.f;
    
    this->updateCameraVectors();
}

void Camera3D::updateCameraVectors()
{
    this->front.setX(qCos(qDegreesToRadians(this->yaw)) * qCos(qDegreesToRadians(this->pitch)));
    this->front.setY(qSin(qDegreesToRadians(this->pitch)));
    this->front.setZ(qSin(qDegreesToRadians(this->yaw)) * qCos(qDegreesToRadians(this->pitch)));

    this->front = this->front.normalized();
    this->right = QVector3D::crossProduct(this->front, this->worldUp);
    this->right = this->right.normalized();
    this->up = QVector3D::crossProduct(this->right, this->front);
    this->up = this->up.normalized();
}

const QMatrix4x4 Camera3D::getViewMatrix()
{
    this->updateCameraVectors();
    this->viewMatrix.setToIdentity();
    this->viewMatrix.lookAt(this->position,
                            this->position + this->front,
                            this->worldUp);
    return this->viewMatrix;
}

const QVector3D Camera3D::getPosition() const 
{
    return this->position;
}

const QVector3D Camera3D::getDirection() const 
{
    return this->front;
}

void Camera3D::move(const float& dt, const int direction)
{
    //Update position vector
    switch (direction)
    {
    case FORWARD:
    	this->position += this->front * this->movementSpeed;
    	break;
    case BACKWARD:
    	this->position -= this->front * this->movementSpeed;
    	break;
    case LEFT:
    	this->position -= this->right * this->movementSpeed;
    	break;
    case RIGHT:
    	this->position += this->right * this->movementSpeed;
    	break;
    case UP:
    	this->position += this->up * this->movementSpeed;
    	break;
    default:
	break;
    }
}

void Camera3D::updateMouseInput(const float& dt, 
                                const double& offsetX, 
                                const double& offsetY)
{
    //Update pitch yaw and roll
    this->pitch += static_cast<float>(offsetY) * this->sensitivity;
    this->yaw += static_cast<float>(offsetX) * this->sensitivity;

    if (this->pitch > 80.f)
    	this->pitch = 80.f;
    else if (this->pitch < -80.f)
    	this->pitch = -80.f;
    if (this->yaw > 360.f || this->yaw < -360.f)
    	this->yaw = 0.f;

}