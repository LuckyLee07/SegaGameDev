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

// 游戏本体
void Play::update(GameBase* parent, float dt)
{
	State* gState = parent->state();

	// 主循环通关检测
	bool needClear = false;
	if (gState->hasCleared())
		needClear = true;

	// 获取输入
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

			// 更新
			gState->update(dx, dy, dt);
		}

		// 绘制
		gState->draw();
	}

	if (needClear) // 清除后报告
	{
		parent->moveTo(GameBase::SEQ_CLEAR);
	}
}