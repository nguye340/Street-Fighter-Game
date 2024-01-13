#pragma once
#include <vector>
#include "ScreenState.h"
using namespace std;

class StateMachine
{
private:
	vector<ScreenState*> m_vStates;

public:
	~StateMachine();
	void Update();
	void Render();
	void PushState(ScreenState* pState);
	void ChangeState(ScreenState* pState);
	void PopState();
	void Clean();
	vector<ScreenState*>& GetStates() { return m_vStates; }
};