#include "Agent.h"

Agent::Agent()
{
}

Agent::~Agent()
{

}

XMMATRIX Agent::Tick(XMFLOAT3 target, float dt)
{
	//start with an identity matrix
	XMMATRIX worldMat = XMMatrixIdentity();

	//get the direction to move to
	//m_move_dir = XMFLOAT3(target.x - m_pos.x, target.y - m_pos.y, 0.0f);
	//XMVECTOR moveVec = XMVector3Normalize(XMLoadFloat3(&m_move_dir));
	//XMStoreFloat3(&m_move_dir, moveVec);

	//move the agent in their move direction
	Steer(target);
	m_vel.x = m_acc.x * dt * m_maxSpeed;
	m_vel.y = m_acc.y * dt * m_maxSpeed;
	m_pos.x += m_vel.x;
	m_pos.y += m_vel.y;

	//rotate the agent around its local coords
	XMMATRIX rotMat = XMMatrixRotationZ(m_rot - XMConvertToRadians(90));

	//translate the agent based on its new position
	XMMATRIX transMat = XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);

	//combine the matrices to get the final world matrix
	worldMat = rotMat * transMat;
	worldMat = XMMatrixTranspose(worldMat);
	return worldMat;
}

//used to separate the agents, hopefully avoiding collisions
void Agent::Separate(std::list<Agent*> agentList)
{
	float desiredSeparation = 5;
	int count = 0;
	XMVECTOR sum = XMVectorZero();

	XMFLOAT3 distance;
	int i = 0;
	//for (int i = 0; i < instanceCount; i++)
	for(std::list<Agent*>::iterator agent = agentList.begin(); agent != agentList.end(); agent++)
	{
		if ((*agent) != this)
		{
			XMVECTOR distanceVec = XMVector3Length(XMLoadFloat3(&(*agent)->m_pos) - XMLoadFloat3(&m_pos));
			XMStoreFloat3(&distance, distanceVec);
			if (distance.x < desiredSeparation)
			{
				XMVECTOR diff = XMVectorSubtract(XMLoadFloat3(&m_pos), XMLoadFloat3(&(*agent)->m_pos));
				diff = XMVector3Normalize(diff);
				//diff *= distance.x;
				//diff /= distance.x;
				sum = XMVectorAdd(sum, diff);
				count++;
			}
		}
	}

	if (count > 0)
	{
		sum /= count;
		sum *= m_maxSpeed;
		XMFLOAT3 steer;
		XMStoreFloat3(&steer, XMVectorSubtract(sum, XMLoadFloat3(&m_vel)));
		m_acc.x += steer.x;
		m_acc.y += steer.y;
	}
}

void Agent::SetPosition(XMFLOAT3 pos)
{
	m_pos = pos;
}

void Agent::SetRotation(float rot)
{
	m_rot = rot;
}

void Agent::Steer(XMFLOAT3 target)
{
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
}

void Agent::Arrive(XMFLOAT3 target)
{
	//For future use
}