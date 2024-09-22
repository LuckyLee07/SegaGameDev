#include <iostream>
#include "State.h"
#include "File.h"
#include "GameLib/Framework.h"

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
		// 获取输入并刷新
		std::cout << "a:left  s:right w:up z:down. command?" << std::endl;
		char input;
		std::cin >> input;
		gState->update(input);

		// 绘制
		gState->draw();
	}

	if (needClear)
	{
		std::cout << "Congraturation's! you win." << std::endl;
		delete gState;
		gState = NULL;
	}
}
