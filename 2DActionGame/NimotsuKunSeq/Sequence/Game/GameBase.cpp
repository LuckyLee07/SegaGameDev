#include "GameBase.h"
#include "State.h"
#include "Base/File.h"
#include "../Parent.h"
#include "GameLib/Framework.h"
#include "Clear.h"
#include "Loading.h"
#include "Menu.h"
#include "Play.h"
#include <stdio.h>

GameBase::GameBase(int stateId) : m_stateId(stateId), m_next(SEQ_NONE),
	m_pState(nullptr), m_pClear(nullptr), m_pLoading(nullptr), m_pMenu(nullptr), m_pPlay(nullptr)
{
	m_pLoading = new Loading();
}

GameBase::~GameBase()
{
	SAFE_DELETE(m_pState);
	SAFE_DELETE(m_pClear);
	SAFE_DELETE(m_pLoading);
	SAFE_DELETE(m_pMenu);
	SAFE_DELETE(m_pPlay);
}

State* GameBase::state()
{
	return m_pState;
}

void GameBase::update(Parent* parent, float dt)
{
	if (m_pClear)
	{
		m_pClear->update(this);
	}
	else if (m_pLoading)
	{
		m_pLoading->update(this);
	}
	else if (m_pMenu)
	{
		m_pMenu->update(this);
	}
	else if (m_pPlay)
	{
		m_pPlay->update(this, dt);
	}
	else
	{
		HALT("game bakana!");
	}

	switch (m_next)
	{
	case GameBase::SEQ_STATE_SELECT:
		parent->moveTo(Parent::SEQ_STATE_SELECT);
		break;
	case GameBase::SEQ_TITLE:
		parent->moveTo(Parent::SEQ_TITLE);
		break;
	case GameBase::SEQ_CLEAR:
		SAFE_DELETE(m_pPlay);
		m_pClear = new Clear();
		break;
	case GameBase::SEQ_MENU:
		SAFE_DELETE(m_pPlay);
		m_pMenu = new Menu();
		break;
	case GameBase::SEQ_PLAY:
		SAFE_DELETE(m_pMenu);
		SAFE_DELETE(m_pLoading);
		m_pPlay = new Play();
		break;
	}

	m_next = SEQ_NONE;
}

void GameBase::moveTo(SeqID seqId)
{
	m_next = seqId;
}

void GameBase::startLoding()
{
	char filePath[128];
	sprintf(filePath, "assets/stageData/%d.txt", m_stateId);

	File file(filePath);
	m_pState = new State(file.data(), file.size());
}