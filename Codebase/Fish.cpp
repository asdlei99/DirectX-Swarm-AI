#include "Fish.h"
#include "Shoal.h"
#include <algorithm>

using namespace DirectX;

Fish::Fish(Swarm* swarm, bool rotatingClockwise) : Agent(swarm)
{
	m_maxSpeed = 5.0f;
	m_minSqDistFromTarget = 500.0f;
	m_isRotatingClockwise = rotatingClockwise;
}

Fish::~Fish()
{
}

void Fish::Tick(float dt)
{
	//get the direction to move to
	//m_move_dir = XMFLOAT3(target.x - m_pos.x, target.y - m_pos.y, 0.0f);
	//XMVECTOR moveVec = XMVector3Normalize(XMLoadFloat3(&m_move_dir));
	//XMStoreFloat3(&m_move_dir, moveVec);

	XMFLOAT3 target = m_parentSwarm->GetTarget();

	//move the agent in their move direction
	Steer(target);

	float velMultiplier = std::clamp((m_minSqDistFromTarget/m_sqDistFromTarget), 0.5f, 1.0f);

	m_vel.x = m_acc.x * dt * m_maxSpeed * velMultiplier;
	m_vel.y = m_acc.y * dt * m_maxSpeed * velMultiplier;

	//apply drag
	m_vel.x *= (1 - dt * m_drag);
	m_vel.y *= (1 - dt * m_drag);

	m_pos.x += m_vel.x;
	m_pos.y += m_vel.y;
}

void Fish::SetIsRotatingClockwise(bool isClockwise)
{
	m_isRotatingClockwise = isClockwise;
}

void Fish::Steer(XMFLOAT3 target)
{
	//original code
	/*
	float accMultiplier = 1;  //increases or decreases the acceleration towards the target

							  //calculate the desired velocity, i.e. straight to the target
	XMVECTOR desiredVelVec = XMVectorSubtract(XMLoadFloat3(&target), XMLoadFloat3(&m_pos));

	//get the angle to rotate towards
	XMFLOAT3 desiredVelFloat;
	XMStoreFloat3(&desiredVelFloat, desiredVelVec);
	m_rot = std::atan2f(desiredVelFloat.y, desiredVelFloat.x);

	//calculate the vector needed to steer towards the desired velocity
	desiredVelVec = XMVector3Normalize(desiredVelVec);
	desiredVelVec *= m_maxSpeed;
	XMVECTOR steeringVec = XMVectorSubtract(desiredVelVec, XMLoadFloat3(&m_vel));
	steeringVec = XMVector3ClampLength(steeringVec, 0.0f, m_maxForce);
	XMStoreFloat3(&m_moveDir, steeringVec);

	m_acc.x += m_moveDir.x * accMultiplier;
	m_acc.y += m_moveDir.y * accMultiplier;
	*/

	//calculate the desired velocity, i.e. swim around the target
	XMVECTOR desiredVelVec = XMVectorSubtract(XMLoadFloat3(&target), XMLoadFloat3(&m_pos));
	XMFLOAT3 sqDistanceFromCentre;	//store the distance from the centre of the swarm
	XMStoreFloat3(&sqDistanceFromCentre, XMVector3LengthSq(desiredVelVec));
	m_sqDistFromTarget = sqDistanceFromCentre.x;

	//get the normal acceleration
	XMVECTOR desiredAccVec = XMVector3Normalize(desiredVelVec);

	//get the tangential acceleration (this is just the current acceleration)
	XMVECTOR tangentialVec = XMLoadFloat3(&m_acc);

	//get the angle to rotate towards
	tangentialVec = desiredVelVec;

	//combine to get the desired acceleration
	XMVECTOR finalAcc = XMVectorAdd(desiredVelVec, tangentialVec);

	//calculate the vector needed to steer towards the desired velocity
	finalAcc = XMVector3Normalize(finalAcc); //XMVector3Normalize(finalAcc);
	finalAcc *= m_maxSpeed;

	XMFLOAT3 desiredVelFloat;
	XMStoreFloat3(&desiredVelFloat, finalAcc);
	
	float rotationAngle = 0;
	if (m_isRotatingClockwise)
	{
		rotationAngle = std::clamp(90.0f - (1 / (m_minSqDistFromTarget / m_sqDistFromTarget)), 0.0f, 90.0f);
	}
	else
	{
		rotationAngle = std::clamp(-90.0f + (1 / (m_minSqDistFromTarget / m_sqDistFromTarget)), -90.0f, 0.0f);
	}

	float rad = XMConvertToRadians(rotationAngle);
	float x = desiredVelFloat.x;
	float y = desiredVelFloat.y;

	desiredVelFloat.x = x * cos(rad) - y * sin(rad);
	desiredVelFloat.y = y * cos(rad) + x * sin(rad);
	m_rot = std::atan2f(desiredVelFloat.y, desiredVelFloat.x);
	finalAcc = XMLoadFloat3(&desiredVelFloat);

	XMVECTOR steeringVec = XMVectorSubtract(finalAcc, XMLoadFloat3(&m_acc));
	steeringVec = XMVector3ClampLength(steeringVec, 0.0f, m_maxForce);
	XMStoreFloat3(&m_moveDir, steeringVec);

	m_acc.x += m_moveDir.x;
	m_acc.y += m_moveDir.y;
}