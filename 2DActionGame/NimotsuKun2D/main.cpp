#include <iostream>
#include "State.h"
#include "File.h"
#include "GameLib/Framework.h"

using namespace GameLib;

void mainLoop();
State* gState = NULL;
const int gFrameInterval = 16; //16毫秒62.5fps。
const unsigned TimeCnt = 10;
unsigned gPrevTime[TimeCnt] = { 0 };
unsigned gPrevDiffTime = 0;
namespace GameLib {
	void Framework::update() {
		unsigned endTime = gPrevTime[TimeCnt - 1];
		// unsigned 考虑溢出情况
		while ((time() - endTime) < gFrameInterval)
		{
			sleep(1);
		}

		unsigned currTime = time();
		unsigned frameTime10 = currTime - gPrevTime[0];
		for (int i = 0; i < TimeCnt - 1; ++i)
		{
			gPrevTime[i] = gPrevTime[i + 1];
		}
		gPrevTime[TimeCnt - 1] = currTime;
		gPrevDiffTime = frameTime10;

		unsigned frameRate = 1000 * 10 / frameTime10; // 帧率计算
		static unsigned sCounter = 0;
		if (++sCounter % 60 == 0)
		{
			cout << "FPS: ===>>" << frameRate << endl;
		}
		
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
	if (gState->hasCleared())
		needClear = true;

	if (!needClear)
	{
		// 获取输入
		int dx = 0, dy = 0;
		static bool sPrevW = false;
		static bool sPrevA = false;
		static bool sPrevS = false;
		static bool sPrevD = false;
		bool inputW = f.isKeyOn('w');
		bool inputA = f.isKeyOn('a');
		bool inputS = f.isKeyOn('s');
		bool inputD = f.isKeyOn('d');
		if (inputA && !sPrevA)
			dx -= 1;
		else if (inputD && !sPrevD)
			dx += 1;
		else if (inputW && !sPrevW)
			dy -= 1;
		else if (inputS && !sPrevS)
			dy += 1;
		sPrevW = inputW;
		sPrevA = inputA;
		sPrevS = inputS;
		sPrevD = inputD;

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
