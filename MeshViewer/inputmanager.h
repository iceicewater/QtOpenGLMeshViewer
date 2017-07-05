#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <Qt>
#include <QPoint>

class InputManager
{
public:

    // Possible key states
    enum InputState
    {
        InputInvalid,
        InputRegistered,
        InputUnregistered,
        InputTriggered,
        InputPressed,
        InputReleased
    };

    // State checking
    static InputState keyState(Qt::Key key);
    static bool keyTriggered(Qt::Key key);
    static bool keyPressed(Qt::Key key);
    static bool keyReleased(Qt::Key key);
    static InputState modifierState(Qt::KeyboardModifier modifier);
    static bool modifierTriggered(Qt::KeyboardModifier modifier);
    static bool modifierPressed(Qt::KeyboardModifier modifier);
    static bool modifierReleased(Qt::KeyboardModifier modifier);
    static InputState buttonState(Qt::MouseButton button);
    static bool buttonTriggered(Qt::MouseButton button);
    static bool buttonPressed(Qt::MouseButton button);
    static bool buttonReleased(Qt::MouseButton button);
    static QPoint mousePosition();
    static QPoint mouseDelta();

private:

    // State updating
    static void update();
    static void registerKeyPress(int key);
    static void registerKeyRelease(int key);
    static void registerModifiers(Qt::KeyboardModifiers modifiers);
    static void registerModifierPress(Qt::KeyboardModifier modifier);
    static void registerModifierRelease(Qt::KeyboardModifier modifier);
    static void registerMousePress(Qt::MouseButton button);
    static void registerMouseRelease(Qt::MouseButton button);
    static void reset();
    friend class GLWidget;
};

inline bool InputManager::keyTriggered(Qt::Key key)
{
    return keyState(key) == InputTriggered;
}

inline bool InputManager::keyPressed(Qt::Key key)
{
    return keyState(key) == InputPressed;
}

inline bool InputManager::keyReleased(Qt::Key key)
{
    return keyState(key) == InputReleased;
}

inline bool InputManager::modifierTriggered(Qt::KeyboardModifier modifier)
{
    return modifierState(modifier) == InputTriggered;
}

inline bool InputManager::modifierPressed(Qt::KeyboardModifier modifier)
{
    return modifierState(modifier) == InputPressed;
}

inline bool InputManager::modifierReleased(Qt::KeyboardModifier modifier)
{
    return modifierState(modifier) == InputReleased;
}

inline bool InputManager::buttonTriggered(Qt::MouseButton button)
{
    return buttonState(button) == InputTriggered;
}

inline bool InputManager::buttonPressed(Qt::MouseButton button)
{
    return buttonState(button) == InputPressed;
}

inline bool InputManager::buttonReleased(Qt::MouseButton button)
{
    return buttonState(button) == InputReleased;
}

#endif // INPUTMANAGER_H
