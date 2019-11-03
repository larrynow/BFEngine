#include "BFMesh.h"

void Mesh::mFunction_UpdateRotateMatrix()
{
	mRotateMatrix = Matrix_RotationXYZ(GetRadian(mRotationPitch), GetRadian(mRotationYaw), GetRadian(mRotationRoll));
}

void Mesh::mFunction_UpdateScaleMatrix()
{
	mScaleTranslateMatrix.m[0][0] = mScaleX;
	mScaleTranslateMatrix.m[1][0] = mScaleY;
	mScaleTranslateMatrix.m[2][0] = mScaleZ;
}

void Mesh::mFunction_UpdateTranslateMatrix()
{
	mScaleTranslateMatrix.m[0][3] = mPosition.x;
	mScaleTranslateMatrix.m[1][3] = mPosition.y;
	mScaleTranslateMatrix.m[2][3] = mPosition.z;
}

void Mesh::mFunction_UpdateWorldMatrix()
{
	mWorldMatrix = mScaleTranslateMatrix * mRotateMatrix;
}
