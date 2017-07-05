#include "inputmanager.h"
#include <QCursor>
#include <vector>
#include <algorithm>

/*******************************************************************************
 * Static Helper Structs
 ******************************************************************************/

template <typename T>
struct InputInstance : std::pair<T, InputManager::InputState>
{
    typedef std::pair<T, InputManager::InputState> base_class;
    inline InputInstance(T value) : base_class(value, InputManager::InputInvalid) {}
    inline InputInstance(T value, InputManager::InputState state) : base_class(value, state) {}
    inline bool operator==(const InputInstance &rhs) const
    {
        return this->first == rhs.first;
    }
};

// Instance types
typedef InputInstance<Qt::Key> KeyInstance;
typedef InputInstance<Qt::KeyboardModifier> ModifierInstance;
typedef InputInstance<Qt::MouseButton> ButtonInstance;

// Container types
typedef std::vector<KeyInstance> KeyContainer;
typedef std::vector<ModifierInstance> ModifierContainer;
typedef std::vector<ButtonInstance> ButtonContainer;

// Globals
static KeyContainer sg_keyInstances;
static ModifierContainer sg_modifierInstances;
static ButtonContainer sg_buttonInstances;
static QPoint sg_mouseCurrPosition;
static QPoint sg_mousePrevPosition;
static QPoint sg_mouseDelta;

/*******************************************************************************
 * Static Helper Fucntions
 ******************************************************************************/

static inline KeyContainer::iterator FindKey(Qt::Key value)
{
    return std::find(sg_keyInstances.begin(), sg_keyInstances.end(), value);
}

static inline ModifierContainer::iterator FindModifier(Qt::KeyboardModifier value)
{
    return std::find(sg_modifierInstances.begin(), sg_modifierInstances.end(), value);
}

static inline ButtonContainer::iterator FindButton(Qt::MouseButton value)
{
    return std::find(sg_buttonInstances.begin(), sg_buttonInstances.end(), value);
}

template <typename TPair>
static inline void UpdateStates(TPair &instance)
{
    switch (instance.second)
    {
        case InputManager::InputRegistered:
            instance.second = InputManager::InputTriggered; break;
        case InputManager::InputTriggered:
            instance.second = InputManager::InputPressed; break;
        case InputManager::InputUnregistered:
            instance.second = InputManager::InputReleased; break;
        default: break;
    }
}

template <typename TPair>
static inline bool CheckReleased(const TPair &instance)
{
    return instance.second == InputManager::InputReleased;
}

template <typename Container>
static inline void Update(Container &container)
{
    typedef typename Container::iterator Iter;
    typedef typename Container::value_type TPair;

    // Remove old data
    Iter remove = std::remove_if(container.begin(), container.end(), &CheckReleased<TPair>);
    container.erase(remove, container.end());

    // Update existing data
    std::for_each(container.begin(), container.end(), &UpdateStates<TPair>);
}

/*******************************************************************************
 * Input Implementation
 ******************************************************************************/

InputManager::InputState InputManager::keyState(Qt::Key k)
{
    KeyContainer::iterator it = FindKey(k);
    return (it != sg_keyInstances.end()) ? it->second : InputInvalid;
}

InputManager::InputState InputManager::modifierState(Qt::KeyboardModifier k)
{
    ModifierContainer::iterator it = FindModifier(k);
    return (it != sg_modifierInstances.end()) ? it->second : InputInvalid;
}

InputManager::InputState InputManager::buttonState(Qt::MouseButton k)
{
    ButtonContainer::iterator it = FindButton(k);
    return (it != sg_buttonInstances.end()) ? it->second : InputInvalid;
}

QPoint InputManager::mousePosition()
{
    return QCursor::pos();
}

QPoint InputManager::mouseDelta()
{
    return sg_mouseDelta;
}

void InputManager::update()
{
    // Update Mouse Delta
    sg_mousePrevPosition = sg_mouseCurrPosition;
    sg_mouseCurrPosition = QCursor::pos();
    sg_mouseDelta = sg_mouseCurrPosition - sg_mousePrevPosition;

    // Update KeyState values
    Update(sg_buttonInstances);
    Update(sg_keyInstances);
    Update(sg_modifierInstances);
}

void InputManager::registerKeyPress(int k)
{
    KeyContainer::iterator it = FindKey((Qt::Key)k);
    if (it == sg_keyInstances.end())
    {
        sg_keyInstances.push_back(KeyInstance((Qt::Key)k, InputRegistered));
    }
}

void InputManager::registerKeyRelease(int k)
{
    KeyContainer::iterator it = FindKey((Qt::Key)k);
    if (it != sg_keyInstances.end())
    {
        it->second = InputUnregistered;
    }
}

void InputManager::registerModifiers(Qt::KeyboardModifiers mods)
{
    if (mods & Qt::AltModifier) registerModifierPress(Qt::AltModifier);
    if (!(mods & Qt::AltModifier)) registerModifierRelease(Qt::AltModifier);
}

void InputManager::registerModifierPress(Qt::KeyboardModifier mod)
{
    ModifierContainer::iterator it = FindModifier((Qt::KeyboardModifier)mod);
    if (it == sg_modifierInstances.end())
    {
        sg_modifierInstances.push_back(ModifierInstance((Qt::KeyboardModifier)mod, InputRegistered));
    }
}

void InputManager::registerModifierRelease(Qt::KeyboardModifier mod)
{
    ModifierContainer::iterator it = FindModifier((Qt::KeyboardModifier)mod);
    if (it != sg_modifierInstances.end())
    {
        it->second = InputUnregistered;
    }
}

void InputManager::registerMousePress(Qt::MouseButton btn)
{
    ButtonContainer::iterator it = FindButton(btn);
    if (it == sg_buttonInstances.end())
    {
        sg_buttonInstances.push_back(ButtonInstance(btn, InputRegistered));
    }
}

void InputManager::registerMouseRelease(Qt::MouseButton btn)
{
    ButtonContainer::iterator it = FindButton(btn);
    if (it != sg_buttonInstances.end())
    {
        it->second = InputUnregistered;
    }
}

void InputManager::reset()
{
    sg_keyInstances.clear();
    sg_modifierInstances.clear();
    sg_buttonInstances.clear();
}
