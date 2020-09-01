#include "GameInputManager.h"
#include <profileapi.h>

using namespace Windows::Foundation;
using namespace Windows::System;

GameInputManager::GameInputManager()
{
    QueryPerformanceFrequency(&m_timerFrequency);
}

GameInputManager* GameInputManager::InitPlatformManager(CoreWindow^ window)
{
    GameInputManager* inputManager = new GameInputManager();
    GameInputManager::PlatformInputForwarder^ inputForwarder = ref new GameInputManager::PlatformInputForwarder(inputManager);
    inputForwarder->Setup(window);
    inputManager->m_inputForwarder = inputForwarder;
    return inputManager;
}

void GameInputManager::PlatformInputForwarder::OnKeyDown(CoreWindow^ sender, KeyEventArgs^ args)
{
    std::string keyValue = "";
    auto inputKey = args->VirtualKey;
    switch (inputKey) {
    case VirtualKey::Up:
        keyValue = "<UP>";
        break;
    }

    // only proceed of we have a value we are interested in
    if (keyValue.empty())
        return;

    LARGE_INTEGER currentTimestamp;
    QueryPerformanceCounter(&currentTimestamp);
    GameInputEvent keyEvent;
    keyEvent.timestamp = currentTimestamp.QuadPart;
    keyEvent.etype = InputEventType::Down;
    keyEvent.value = keyValue;
    m_inputManager->m_inputQueue.push(keyEvent);
}

void GameInputManager::PlatformInputForwarder::OnKeyUp(CoreWindow^ sender, KeyEventArgs^ args)
{
    LARGE_INTEGER currentTimestamp;
    QueryPerformanceCounter(&currentTimestamp);
}

void GameInputManager::PlatformInputForwarder::Setup(CoreWindow^ window)
{
    window->KeyDown +=
        ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &GameInputManager::PlatformInputForwarder::OnKeyDown);

    window->KeyUp +=
        ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &GameInputManager::PlatformInputForwarder::OnKeyUp);
}

GameInputManager::PlatformInputForwarder::PlatformInputForwarder(GameInputManager* inputManager)
{
    m_inputManager = inputManager;
}