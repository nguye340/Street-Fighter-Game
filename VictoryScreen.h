#pragma once
#include "ScreenState.h"
#include <vector>	
using namespace std;

class VictoryScreen : public ScreenState
{	
	public:
		VictoryScreen() {}
		void Update(); // Method prototype.
		void Render();
		void Enter();
		void Exit();
		void Resume() {}

};
