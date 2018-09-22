#include "MathUtility.h"


enum DeviceType{
	DT_OpenGL,
	DT_Directx9,
	DT_Directx10,
};
DeviceType getGraphicsDeviceType(){
	return DT_OpenGL;
}

// left hand coord system
void makeMatrixLookAtLH( Matrix4x4& outMat,const Vector4 &eye,const Vector4 &lookat,const Vector4 &up )
{
	Vector4 up_normalized = VectorNormalize(up);
	Vector4 zaxis = (lookat - eye); zaxis.Normalize();
	Vector4 xaxis = Vector3CrossProduct(up_normalized, zaxis);
	Vector4 yaxis = Vector3CrossProduct(zaxis, xaxis);

	outMat.Identity();

	outMat.SetColumn(0, xaxis);
	outMat.SetColumn(1, yaxis);
	outMat.SetColumn(2, zaxis);
	outMat[3][0] = -Vector3Dot(xaxis, eye)[0];
	outMat[3][1] = -Vector3Dot(yaxis, eye)[0];
	outMat[3][2] = -Vector3Dot(zaxis, eye)[0];
}

// right hand coord system
// eye = `���Y��m`
// lookat = `���Y��Ǫ���m`
// up = `���Y���W�誺��V`
void makeMatrixLookAtRH(Matrix4x4& outMat,const Vector4 &eye,const Vector4 &lookat,const Vector4 &up)
{
	Vector4 up_normalized = VectorNormalize(up);
	Vector4 zaxis = (eye - lookat); zaxis.Normalize();
	Vector4 xaxis = Vector3CrossProduct(up_normalized, zaxis);
	Vector4 yaxis = Vector3CrossProduct(zaxis, xaxis);

	//Matrix4x4 matrix; 
	outMat.Identity();

	outMat.SetColumn(0, xaxis);
	outMat.SetColumn(1, yaxis);
	outMat.SetColumn(2, zaxis);
	outMat[3][0] = -Vector3Dot(xaxis, eye)[0];
	outMat[3][1] = -Vector3Dot(yaxis, eye)[0];
	outMat[3][2] = -Vector3Dot(zaxis, eye)[0];

	//return outMat;
}

void makeMatrixOrthoRH(Matrix4x4& outMat,float w, float h, float z_near, float z_far)
{
	switch(getGraphicsDeviceType())
	{
	case DT_OpenGL:
		return makeMatrixOrthoRH_OpenGL(outMat,w, h, z_near, z_far);
		break;
	case DT_Directx9:
	case DT_Directx10:
	default:
		return makeMatrixOrthoRH_DirectX(outMat,w, h, z_near, z_far);
		break;
	}
}

void makeMatrixPerspectiveRH( Matrix4x4& outMat,float fovy, float aspect, float z_near, float z_far )
{
	switch(getGraphicsDeviceType())
	{
	case DT_OpenGL:
		return makeMatrixPerspectiveRH_OpenGL(outMat,fovy, aspect, z_near, z_far);
		break;
	case DT_Directx9:
	case DT_Directx10:
	default:
		return makeMatrixPerspectiveRH_DirectX(outMat,fovy, aspect, z_near, z_far);
		break;
	}
}


// Direct3D native left hand system
// fovy = ������V������
// aspect = ������V�����ﭫ����V���������
// z_hear = ���Y�i�H�ݨ쪺�̪�Z��
// z_far = ���Y�i�H�ݨ쪺�̻��Z��
Matrix4x4 makeMatrixPerspective_DirectX(float fovy, float aspect, float z_near, float z_far)
{
	Matrix4x4 matrix;
	matrix.Identity();

	float fovy_radian = FastMath::DegreeToRadian(fovy);
	float yscale = FastMath::Cot(fovy_radian/2.0f);
	float xscale = yscale * aspect;

	matrix[0][0] = xscale;
	matrix[1][1] = yscale;
	matrix[2][2] = z_far / (z_far - z_near);
	matrix[2][3] = 1.0f;
	matrix[3][2] = -(z_near * z_far) /(z_far - z_near);
	matrix[3][3] = 0.0f;

	return matrix;
}

// w = `���Y������V�i�H�ݨ쪺�d��`
// h = `���Y������V�i�H�ݨ쪺�d��`
// z_hear = `���Y�i�H�ݨ쪺�̪�Z��`
// z_far  = `���Y�i�H�ݨ쪺�̻��Z��`
void makeMatrixOrthoRH_DirectX( Matrix4x4& outMat,float w, float h, float z_near, float z_far )
{
	//Matrix4x4 matrix;
	outMat.Identity();

	outMat[0][0] = 2.0f/w;
	outMat[1][1] = 2.0f/h;
	outMat[2][2] = 1.0f/(z_near - z_far);
	outMat[3][2] = z_near / (z_near - z_far);

}

// fovy = ������V������
// aspect = `������V�����ﭫ����V���������`
// z_hear = `���Y�i�H�ݨ쪺�̪�Z��`
// z_far = `���Y�i�H�ݨ쪺�̻��Z��`
void makeMatrixPerspectiveRH_DirectX( Matrix4x4& outMat,float fovy, float aspect, float z_near, float z_far )
{
	//Matrix4x4 matrix;
	outMat.Identity();

	float fovy_radian = FastMath::DegreeToRadian(fovy);
	float yscale = FastMath::Cot(fovy_radian/2.0f);
	float xscale = yscale * aspect;

	outMat[0][0] = xscale;
	outMat[1][1] = yscale;
	outMat[2][2] = z_far / (z_near - z_far);
	outMat[2][3] = -1.0f;
	outMat[3][2] = (z_near * z_far) /(z_near - z_far);
	outMat[3][3] = 0.0f;

	//return matrix;
}

// OpenGL native right hand system
// fovy = `������V������`
// aspect = `������V�����ﭫ����V���������`
// z_hear = `���Y�i�H�ݨ쪺�̪�Z��`
// z_far = `���Y�i�H�ݨ쪺�̻��Z��`
void makeMatrixOrthoRH_OpenGL( Matrix4x4& outMat,float w, float h, float z_near, float z_far )
{
	//Matrix4x4 matrix;
	outMat.Identity();

	outMat[0][0] = 2.0f/w;
	outMat[1][1] = 2.0f/h;
	outMat[2][2] = 2.0f/(z_near - z_far);
	outMat[3][2] = (z_far + z_near)/(z_near - z_far);
	outMat[3][3] = 1.0f;

	//return matrix;
}


void makeMatrixOrtho_OpenGL( Matrix4x4& outMat,float w, float h, float z_near, float z_far )
{

	return makeMatrixOrthoRH_OpenGL(outMat,w, h, z_near, z_far);
}

// OpenGL native right hand system
// fovy = `������V������`
// aspect = `������V�����ﭫ����V���������`
// z_hear = `���Y�i�H�ݨ쪺�̪�Z��`
// z_far = `���Y�i�H�ݨ쪺�̻��Z��`
void makeMatrixPerspectiveRH_OpenGL( Matrix4x4& outMat,float fovy, float aspect, float z_near, float z_far )
{
	//Matrix4x4 matrix;
	outMat.Identity();

	float fovy_radian = FastMath::DegreeToRadian(fovy);
	float yscale =  FastMath::Cot(fovy_radian/2.0f);
	float xscale = yscale * aspect;

	outMat[0][0] = xscale;
	outMat[1][1] = yscale;
	outMat[2][2] = (z_far + z_near)/(z_near - z_far);
	outMat[2][3] = -1.0f;
	outMat[3][2] = 2.0f * z_far * z_near / (z_near - z_far);
	outMat[3][3] = 0.0f;

	//return matrix;
}

void makeMatrixPerspectiveLH_OpenGL( Matrix4x4& outMat,float fovy, float aspect, float z_near, float z_far )
{
	return makeMatrixPerspectiveRH_OpenGL(outMat,fovy, aspect, z_near, z_far);
}
