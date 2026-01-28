//
// Created by tayler on 12/14/25.
//

#ifndef SIMULATION_INPUT_H
#define SIMULATION_INPUT_H


#include "inputmanager.h"

#include <functional>
#include <map>

class InputManager;

enum KeyEventType {
    KEY_DOWN,
    KEY_UP,
    KEY_PRESSED
};

enum MouseEventType {
    MOUSE_DOWN,
    MOUSE_UP,
    MOUSE_PRESSED
};

class Input {
public:
    Input(InputManager *inputManager);

    void BindKey(int key, KeyEventType eventType, std::function<void()> action);
    void BindMouseButton(int button, MouseEventType eventType, std::function<void()> action);
    void BindScroll(std::function<void(double, double)> action);

    void UnbindKey(int key, KeyEventType eventType);
    void UnbindMouseButton(int button, MouseEventType eventType);
    void UnbindScroll();

    void ClearAllInputEvents();

    void Update();
private:
    InputManager *inputManager;
    std::map<std::pair<int, KeyEventType>, std::function<void()>> keyBinds;
    std::map<std::pair<int, MouseEventType>, std::function<void()>> mouseButtonBinds;
    std::function<void(double, double)> scrollAction;
};

#endif //SIMULATION_INPUT_H