#pragma once
#include "../Time/TimeManager.h"
#include <stdio.h>

class GraphicManager;

namespace tge
{
	class FPSCounter
	{
	public:
		void Update();
		void SetActive(bool active);
	private:
		float _sec = 0;
		unsigned _frames = 0;

		bool is_active = false;
	};
}

