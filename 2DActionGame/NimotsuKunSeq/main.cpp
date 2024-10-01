#include <iostream>
#include "Base/File.h"
#include "GameLib/Framework.h"
#include "Sequence/Parent.h"

using namespace GameLib;

void mainLoop(float dt);

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
	static Parent* gParentPtr = nullptr;
	if (gParentPtr == nullptr)
	{
		gParentPtr = new Parent;
	}
	gParentPtr->update(dt);

	Framework f = Framework::instance();
	// 结束判断
	if (f.isKeyOn('q'))
	{
		f.requestEnd();
	}
	if (f.isEndRequested())
	{
		SAFE_DELETE(gParentPtr);
	}
}
