#pragma once
#include <QMatrix4x4>
#define NUM_BONES_PER_VERTEX 4


struct Bone {
    std::string name;
    int parentIndex;

    QMatrix4x4 offsetMatrix;
    QMatrix4x4 finalTransform;
};

struct VertexBoneData
{
    uint IDs[NUM_BONES_PER_VERTEX];
    float Weights[NUM_BONES_PER_VERTEX];
};