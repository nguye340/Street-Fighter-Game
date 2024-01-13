#pragma once
#include "StateMachine.h"
#include <iostream>
// Begin StateMachine
void StateMachine::Update()
{
	if (!m_vStates.empty()) // empty() and back() are methods of the vector type.
		m_vStates.back()->Update();
}

void StateMachine::Render()
{
	if (!m_vStates.empty())
		m_vStates.back()->Render();
}

void StateMachine::PushState(ScreenState* pState)
{
	m_vStates.push_back(pState); // push_back() is a method of the vector type.
	m_vStates.back()->Enter();
}

void StateMachine::ChangeState(ScreenState* pState)
{
	if (!m_vStates.empty())
	{
		m_vStates.back()->Exit();
		delete m_vStates.back(); // De-allocating the state in the heap.
		m_vStates.back() = nullptr; // Nullifying pointer to the de-allocated state.
		m_vStates.pop_back(); // Removes the now-null pointer from the vector.
	}
	pState->Enter();
	m_vStates.push_back(pState);
}

void StateMachine::PopState()
{
	if (!m_vStates.empty())
	{
		m_vStates.back()->Exit();
		delete m_vStates.back();
		m_vStates.back() = nullptr;
		m_vStates.pop_back();
	}
	m_vStates.back()->Resume();
}

void StateMachine::Clean()
{
	while (!m_vStates.empty()) // Ensures that ALL states left in the vector are cleaned up.
	{
		m_vStates.back()->Exit();
		delete m_vStates.back();
		m_vStates.back() = nullptr;
		m_vStates.pop_back();
	}
}

StateMachine::~StateMachine()
{
	std::cout << "Destroying FSM...\n";
	Clean();
}
// End StateMachine