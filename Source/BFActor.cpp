#include"BFActor.h"

inline void ABFActor::AddMovement(const VEC3 & direction, float movement)
{
	// Move actor position and its camera.
	mPosition = mPosition + (direction * movement);
	m_pCamera->MoveTo(mPosition);
}

// Bind a input op.
//std::bind(&ABFActor::op, *this);

ABFActor::ABFActor()
{
	mPosition = { 0.f, 0.f, -50.f };
	m_pCamera = new IBFCamera(mPosition);
	mMoveSpeed = 1.f;
}

ABFActor::~ABFActor()
{
	delete m_pCamera;
}