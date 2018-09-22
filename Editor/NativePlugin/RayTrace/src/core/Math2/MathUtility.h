#include "Vector4.h"
#include "Matrix4x4.h"

void makeMatrixLookAtRH(Matrix4x4& outMat,const Vector4 &eye,const Vector4 &lookat,const Vector4 &up);
void makeMatrixLookAtLH(Matrix4x4& outMat,const Vector4 &eye,const Vector4 &lookat,const Vector4 &up);

void makeMatrixOrthoLH(Matrix4x4& outMat,float w, float h, float z_near, float z_far);
void makeMatrixOrthoRH(Matrix4x4& outMat,float w, float h, float z_near, float z_far);
void makeMatrixPerspectiveRH(Matrix4x4& outMat,float fovy, float aspect, float z_near, float z_far);
void makeMatrixPerspectiveLH(Matrix4x4& outMat,float fovy, float aspect, float z_near, float z_far);

void makeMatrixOrtho_DirectX(Matrix4x4& outMat,float w, float h, float z_near, float z_far);
void makeMatrixOrthoRH_DirectX(Matrix4x4& outMat,float w, float h, float z_near, float z_far);
void makeMatrixOrthoLH_DirectX(Matrix4x4& outMat,float w, float h, float z_near, float z_far);
void makeMatrixPerspectiveRH_DirectX(Matrix4x4& outMat,float fovy, float aspect, float z_near, float z_far);
//void makeMatrixPerspectiveRH_DirectX(Matrix4x4& outMat,float fovy, float aspect, float z_near, float z_far);
void makeMatrixPerspectiveLH_DirectX(Matrix4x4& outMat,float fovy, float aspect, float z_near, float z_far);

void makeMatrixOrtho_OpenGL(Matrix4x4& outMat,float w, float h, float z_near, float z_far);
void makeMatrixOrthoRH_OpenGL(Matrix4x4& outMat,float w, float h, float z_near, float z_far);
void makeMatrixOrthoLH_OpenGL(Matrix4x4& outMat,float w, float h, float z_near, float z_far);
void makeMatrixPerspectiveRH_OpenGL(Matrix4x4& outMat,float fovy, float aspect, float z_near, float z_far);
//void makeMatrixPerspectiveRH_OpenGL(Matrix4x4& outMat,float fovy, float aspect, float z_near, float z_far);
void makeMatrixPerspectiveLH_OpenGL(Matrix4x4& outMat,float fovy, float aspect, float z_near, float z_far);
