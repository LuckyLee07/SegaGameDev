#include "Parent.h"
#include "Title.h"
#include "StateSelect.h"
#include "Game/GameBase.h"
#include "GameLib/Framework.h"

Parent::Parent() : m_pTitle(nullptr), 
	m_pStateSelect(nullptr), m_pMainGame(nullptr),
	m_next(SEQ_NONE), m_stateId(0)
{
	m_pTitle = new Title();
}

Parent::~Parent()
{
	if (m_pTitle) SAFE_DELETE(m_pTitle);
	if (m_pStateSelect) SAFE_DELETE(m_pStateSelect);
	if (m_pMainGame) SAFE_DELETE(m_pMainGame);
}

void Parent::update(float dt)
{
	if (m_pTitle)
	{
		m_pTitle->update(this);
	}
	else if (m_pStateSelect)
	{
		m_pStateSelect->update(this);
	}
	else if (m_pMainGame)
	{
		m_pMainGame->update(this, dt);
	}
	else
	{
		HALT("bakana!");
	}

	switch (m_next)
	{
	case Parent::SEQ_STATE_SELECT:
		SAFE_DELETE(m_pTitle);
		SAFE_DELETE(m_pMainGame);
		m_pStateSelect = new StateSelect();
		break;
	case Parent::SEQ_TITLE:
		SAFE_DELETE(m_pMainGame);
		m_pTitle = new Title();
		break;
	case Parent::SEQ_GAME:
		SAFE_DELETE(m_pStateSelect);
		m_pMainGame = new GameBase(m_stateId);
		break;
	}
	m_next = SEQ_NONE;
}

void Parent::moveTo(SeqID seqId)
{
	m_next = seqId;
}

void Parent::setStateId(int stateId)
{
	m_stateId = stateId;
}
