#pragma once

#include <QVector3D>
#include <QVector2D>
#define WEIGHTS_PER_VERTEX  4
struct Vertex
{
	QVector3D position;
	QVector2D texcoord;
	QVector3D normal;

	float weight[WEIGHTS_PER_VERTEX] = {0.0f, 0.0f, 0.0f, 0.0f};
    unsigned int id[WEIGHTS_PER_VERTEX] = {0, 0, 0, 0};

};