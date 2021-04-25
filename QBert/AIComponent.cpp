#include "AIComponent.h"

#include "GameTime.h"
#include "MoveCommand.h"
#include "TextureComponent.h"

QBert::AIComponent::AIComponent(dae::GameObject* pOwner, EnemyType type, dae::GameObject* pPlayer)
	: Component(pOwner)
	, m_Type(type)
	, m_pPlayer(pPlayer)
	, m_movementTime(0.f)
	, m_pUpCommand(nullptr)
	, m_pDownCommand(nullptr)
	, m_pLeftCommand(nullptr)
	, m_pRightCommand(nullptr)
{
}

QBert::AIComponent::~AIComponent()
{
	delete m_pUpCommand;
	delete m_pDownCommand;
	delete m_pLeftCommand;
	delete m_pRightCommand;
}

void QBert::AIComponent::Initialize()
{
	auto pMovementComponent = m_pOwner->GetComponentOfType<MovementComponent>();
	if (pMovementComponent == nullptr)
		throw std::exception("AIComponent Initialized without MovementComponent");
	
	bool revertsTiles = m_Type == EnemyType::SlickSam;
	m_pUpCommand = new MoveCommand(pMovementComponent, MovementComponent::Direction::UP, false, revertsTiles);
	m_pDownCommand = new MoveCommand(pMovementComponent, MovementComponent::Direction::DOWN, false, revertsTiles);
	m_pLeftCommand = new MoveCommand(pMovementComponent, MovementComponent::Direction::LEFT, false, revertsTiles);
	m_pRightCommand = new MoveCommand(pMovementComponent, MovementComponent::Direction::RIGHT, false, revertsTiles);
}

void QBert::AIComponent::Update()
{
	m_movementTime += dae::GameTime::GetInstance().GetDeltaTime();
	if (m_movementTime < m_movementInterval) 
		return;

	m_movementTime = 0.f;

	
	//MovementComponent::Direction toGo{ rand() % 2 == 0 ? MovementComponent::Direction::DOWN : MovementComponent::Direction::RIGHT };
	auto toGo = GetNextDirectionToGo();
	
	switch (toGo)
	{
	case MovementComponent::Direction::UP: 
		m_pUpCommand->Execute();
		break;
	case MovementComponent::Direction::DOWN: 
		m_pDownCommand->Execute();
		break;
	case MovementComponent::Direction::LEFT: 
		m_pLeftCommand->Execute();
		break;
	case MovementComponent::Direction::RIGHT: 
		m_pRightCommand->Execute();
		break;
	}
}

QBert::AIComponent::EnemyType QBert::AIComponent::GetType() const
{
	return m_Type;
}
