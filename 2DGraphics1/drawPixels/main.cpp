#include "GameLib/Framework.h"

namespace GameLib {
	void Framework::update() {
		unsigned* vram = videoMemory();
		static unsigned i;
		vram[i] += i * 100;
		i += 9973; //С��1�����������
		i %= (width() * height());
	}
}