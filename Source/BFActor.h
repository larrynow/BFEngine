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
	inline void MoveForward() { AddMovement(m_pCamera->GetFront(), mMoveSpeed); };
	inline virtual void MoveBack() { AddMovement(-m_pCamera->GetFront(), mMoveSpeed); };
	inline virtual void MoveLeft() { AddMovement(-m_pCamera->GetRight(), mMoveSpeed); };
	inline virtual void MoveRight() { AddMovement(m_pCamera->GetRight(), mMoveSpeed); };
	inline virtual void MoveUp() { AddMovement(m_pCamera->GetUp(), mMoveSpeed); };
	inline virtual void MoveDown() { AddMovement(-m_pCamera->GetUp(), mMoveSpeed); };

	inline IBFCamera* GetCamera() { return m_pCamera; };
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

	IBFCamera* m_pCamera;

	//Mesh.
};





#endif // !BFACTOR
