#include <iostream>
#include "State.h"
#include "File.h"
#include "GameLib/Framework.h"

using namespace GameLib;

void mainLoop();
State* gState = NULL;

namespace GameLib {
	void Framework::update() {
		mainLoop();
	}
}

int main()
{
	while (true)
		mainLoop();

	return 0;
}

void mainLoop()
{
	Framework f = Framework::instance();
	if (gState == NULL)
	{
		File file("assets/stageData.txt");
		gState = new State(file.data(), file.size());

		gState->draw();
	}

	// 主循环通关检测
	bool needClear = false;
	if (gState->checkClear())
		needClear = true;

	if (!needClear)
	{
		// 获取输入
		int dx = 0, dy = 0;
		static bool sPrevA = false;
		static bool sPrevS = false;
		static bool sPrevW = false;
		static bool sPrevZ = false;
		bool inputA = f.isKeyOn('a');
		bool inputS = f.isKeyOn('s');
		bool inputW = f.isKeyOn('w');
		bool inputZ = f.isKeyOn('z');
		if (inputA && !sPrevA)
			dx -= 1;
		else if (inputS && !sPrevS)
			dx += 1;
		else if (inputW && !sPrevW)
			dy -= 1;
		else if (inputZ && !sPrevZ)
			dy += 1;
		sPrevA = inputA;
		sPrevS = inputS;
		sPrevW = inputW;
		sPrevZ = inputZ;

		// 更新
		gState->update(dx, dy);

		// 绘制
		gState->draw();
	}

	if (needClear) //庆祝通关
	{
		std::cout << "Congraturation's! you win." << std::endl;
		delete gState;
		gState = NULL;
	}
	// 结束判断
	if (f.isKeyOn('q'))
	{
		f.requestEnd();
	}
	if (f.isEndRequested())
	{
		if (gState)
		{
			delete gState;
			gState = NULL;
		}
	}
}
