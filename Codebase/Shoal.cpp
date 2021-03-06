#include "Shoal.h"
#include "Fish.h"

Shoal::Shoal() : Swarm()
{
}

Shoal::~Shoal()
{
}

void Shoal::InitialiseSwarm()
{
	m_instances = m_model->GetInstances();

	for (int i = 0; i < m_swarmHeight; i++)
	{
		for (int j = 0; j < m_swarmWidth; j++)
		{
			Fish* agent = new Fish(this, m_rotatingClockwise);
			agent->SetPosition(DirectX::XMFLOAT3(j * 2.0f * m_agentScale + m_pos.x + (j - m_swarmWidth * m_agentScale), i * 2.0f * m_agentScale + m_pos.y + (i - m_swarmHeight * m_agentScale), 0.0f));
			agent->SetScale(m_agentScale);
			m_agents.push_back(agent);
		}
	}
}

void Shoal::InitialiseSwarm(CollisionGrid* grid)
{
	m_instances = m_model->GetInstances();

	for (int i = 0; i < m_swarmHeight; i++)
	{
		for (int j = 0; j < m_swarmWidth; j++)
		{
			Fish* agent = new Fish(this, m_rotatingClockwise);
			agent->SetPosition(DirectX::XMFLOAT3(j * m_agentScale + m_pos.x + (j - m_swarmWidth * m_agentScale), i* m_agentScale + m_pos.y + (i - m_swarmHeight * m_agentScale), 0.0f));
			agent->SetScale(m_agentScale);
			m_agents.push_back(agent);

			//add the new agent to the collision grid
			grid->GetCell(agent->GetPosition())->agentList.push_back(agent);
		}
	}
}

void Shoal::SetIsRotatingClockwise(bool isClockwise)
{
	m_rotatingClockwise = isClockwise;
}