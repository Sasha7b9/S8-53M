#pragma once


struct Point
{
    float x;
    float y;
    float z;
};

struct Edge
{
    int numPoint0;
    int numPoint1;
};

struct Mesh
{
    int numEdges;
    Edge* edges;
    int numPoints;
    Point* points;
};

struct Mat4x4
{
    float n[4][4];
};


void FillMatriceRotation(Mat4x4 *mat, float angleX, float angleY, float angleZ);
void FillMatriceScale(Mat4x4 *mat, float scaleX, float scaleY, float scaleZ);
void FillMatriceMov(Mat4x4 *mat, float movX, float movY, float movZ);
void SetMatriceTransform(Mat4x4 *matTransform, Mat4x4 *matScale, Mat4x4 *matRotate, Mat4x4 *matMove);
void TransformPoint(Mat4x4 *mat, Point *pointIn, Point *pointOut);
void PresentPointToScreen(Point *point, float *x, float *y);
void MatrixMul(Mat4x4 *mat0, Mat4x4 *mat1, Mat4x4 *matOut);
