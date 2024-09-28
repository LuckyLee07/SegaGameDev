#include <iostream>
#include "State.h"
#include "File.h"
#include "GameLib/Framework.h"

using namespace GameLib;

void mainLoop(float dt);

State* gState = NULL;
const unsigned MAX_FRAME = 10;
const int gFrameInterval = 16; //16毫秒62.5fps。
unsigned gPrevTime[MAX_FRAME] = { 0 };
namespace GameLib {
	void Framework::update() {
		unsigned endTime = gPrevTime[MAX_FRAME - 1];
		// unsigned 考虑溢出情况
		while ((time() - endTime) < gFrameInterval)
		{
			sleep(1);
		}

		unsigned currTime = time();
		unsigned frameTime10 = currTime - gPrevTime[0];
		for (int i = 0; i < MAX_FRAME - 1; ++i)
		{
			gPrevTime[i] = gPrevTime[i + 1];
		}
		gPrevTime[MAX_FRAME - 1] = currTime;

		unsigned frameRate = 1000 * 10 / frameTime10; // 帧率计算
		static unsigned sCounter = 0;
		if (++sCounter % 60 == 0)
		{
			cout << "FPS: ===>>" << frameRate << endl;
		}
		
		mainLoop(frameTime10*0.1f);
	}
}

int main()
{
	while (true)
		mainLoop(0.0f);

	return 0;
}

void mainLoop(float dt)
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
		if (inputA)
			dx -= 1;
		else if (inputD)
			dx += 1;
		else if (inputW)
			dy -= 1;
		else if (inputS)
			dy += 1;
		sPrevW = inputW;
		sPrevA = inputA;
		sPrevS = inputS;
		sPrevD = inputD;

		// 更新
		gState->update(dx, dy, dt);

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
