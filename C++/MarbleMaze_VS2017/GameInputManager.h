///
/// Based on https://gamedev.stackexchange.com/questions/59582/input-management-techniques-in-large-games.
/// 
/// 


#pragma once
#include <GameInputManager.h>
#include <wrl.h>
#include <queue>
#include <string>

using namespace Windows::UI::Core;

enum class InputEventType
{
	None,
	Down,
	Up
};

struct GameInputEvent
{
	LONGLONG timestamp;
	std::string value;
	InputEventType etype;
};


/// <summary>
/// Manager for game input related functionality.
/// </summary>
class GameInputManager
{
public:
	static GameInputManager* InitPlatformManager(CoreWindow^ window);

private:
	/// <summary>
	/// This class is needed by CoreWindow to be the event
	/// handler delegate. It is used to forward the events
	/// to the input manager.
	/// </summary>
	ref class PlatformInputForwarder sealed
	{
	public:
		void Setup(CoreWindow^ window);
		void OnKeyDown(
			CoreWindow^ sender,
			KeyEventArgs^ args
		);
		void OnKeyUp(
			CoreWindow^ sender,
			KeyEventArgs^ args
		);

	private:
		GameInputManager* m_inputManager;

	internal:
		PlatformInputForwarder(GameInputManager* inputManager);

	};

	GameInputManager::PlatformInputForwarder^ m_inputForwarder;
	std::queue<GameInputEvent> m_inputQueue;
	LARGE_INTEGER m_timerFrequency;

	GameInputManager();


};
