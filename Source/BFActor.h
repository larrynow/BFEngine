#pragma once
#ifndef BFACTOR
#define BFACTOR

#include"BFEngine.h"

class ABFActor
{
public:
	ABFActor();
	~ABFActor();

	// Add movement.
	virtual void AddMovement(const VEC3& direction, float movement);
	inline virtual void MoveForward() { AddMovement(mCamera.GetFront(), mMoveSpeed); };
	inline virtual void MoveBack() { AddMovement(-mCamera.GetFront(), mMoveSpeed); };
	inline virtual void MoveLeft() { AddMovement(-mCamera.GetRight(), mMoveSpeed); };
	inline virtual void MoveRight() { AddMovement(mCamera.GetRight(), mMoveSpeed); };
	inline virtual void MoveUp() { AddMovement(mCamera.GetUp(), mMoveSpeed); };
	inline virtual void MoveDown() { AddMovement(-mCamera.GetUp(), mMoveSpeed); };

	// Turning movement.
	//inline virtual void Turn() {};

	// Jump.
	//virtual void jump();

private:

	/////////////////////////////////////
	// Actor status.

	VEC3 mPosition;

	float mMoveSpeed;

	/////////////////////////////////////
	// Actor components.

	IBFCamera mCamera;

	//Mesh.
};





#endif // !BFACTOR
