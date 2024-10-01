#include "Play.h"
#include "GameBase.h"
#include "State.h"
#include "Base/Image.h"
#include "GameLib/Framework.h"

using namespace GameLib;

Play::Play()
{
}

Play::~Play()
{
}

// ��Ϸ����
void Play::update(GameBase* parent, float dt)
{
	State* gState = parent->state();

	// ��ѭ��ͨ�ؼ��
	bool needClear = false;
	if (gState->hasCleared())
		needClear = true;

	// ��ȡ����
	Framework f = Framework::instance();
	if (!needClear)
	{
		if (f.isKeyTriggered(' '))
		{
			parent->moveTo(GameBase::SEQ_MENU);
		}
		else
		{
			int dx = 0, dy = 0;
			if (f.isKeyOn('w')) 
				dy -= 1;
			else if (f.isKeyOn('a'))
				dx -= 1;
			else if (f.isKeyOn('s'))
				dy += 1;
			else if (f.isKeyOn('d')) 
				dx += 1;

			// ����
			gState->update(dx, dy, dt);
		}

		// ����
		gState->draw();
	}

	if (needClear) // ����󱨸�
	{
		parent->moveTo(GameBase::SEQ_CLEAR);
	}
}