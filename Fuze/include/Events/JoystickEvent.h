#pragma once

#include "Core.h"
#include "Event.h"

namespace Fuze {
class FUZE_API JoystickEvent : public Event {
  public:
    int GetJoystickID() const { return m_JoystickID; }
    EVENT_CLASS_CATEGORY(EventCategoryJoystick | EventCategoryInput)

  protected:
    JoystickEvent(int joystickID): m_JoystickID(joystickID) {}

    int m_JoystickID;
};

class FUZE_API JoystickConnectedEvent : public JoystickEvent {
  public:
    JoystickConnectedEvent(int joystickID): JoystickEvent(joystickID) {}

    inline std::string ToString() const override {
        std::stringstream ss;
        ss << "JoystickConnected: " << m_JoystickID;
        return ss.str();
    }

    EVENT_CLASS_TYPE(JoystickConnected)
};

class FUZE_API JoystickDisconnectedEvent : public JoystickEvent {
  public:
    JoystickDisconnectedEvent(int joystickID): JoystickEvent(joystickID) {}

    inline std::string ToString() const override {
        std::stringstream ss;
        ss << "JoystickDisconnected: " << m_JoystickID;
        return ss.str();
    }

    EVENT_CLASS_TYPE(JoystickDisconnected)
};

class FUZE_API JoystickButtonPressedEvent : public JoystickEvent {
  public:
    JoystickButtonPressedEvent(int joystickID, int button): JoystickEvent(joystickID), m_Button(button) {}

    int GetButton() const { return m_Button; }

    inline std::string ToString() const override {
        std::stringstream ss;
        ss << "JoystickButtonPressed: " << m_JoystickID << " " << m_Button;
        return ss.str();
    }

    EVENT_CLASS_TYPE(JoystickButtonPressed)

  private:
    int m_Button;
};

class FUZE_API JoystickButtonReleasedEvent : public JoystickEvent {
  public:
    JoystickButtonReleasedEvent(int joystickID, int button): JoystickEvent(joystickID), m_Button(button) {}

    int GetButton() const { return m_Button; }

    inline std::string ToString() const override {
        std::stringstream ss;
        ss << "JoystickButtonReleased: " << m_JoystickID << " " << m_Button;
        return ss.str();
    }

    EVENT_CLASS_TYPE(JoystickButtonReleased)

  private:
    int m_Button;
};

class FUZE_API JoystickAxisMovedEvent : public JoystickEvent {
  public:
    JoystickAxisMovedEvent(int joystickID, int axis, float value)
        : JoystickEvent(joystickID), m_Axis(axis), m_Value(value) {}

    int GetAxis() const { return m_Axis; }
    float GetValue() const { return m_Value; }

    inline std::string ToString() const override {
        std::stringstream ss;
        ss << "JoystickAxisMoved: " << m_JoystickID << " " << m_Axis << " " << m_Value;
        return ss.str();
    }

    EVENT_CLASS_TYPE(JoystickMoved)

  private:
    int m_Axis;
    float m_Value;
};
}
