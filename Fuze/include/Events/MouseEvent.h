#pragma once

#include "Event.h"

namespace Fuze {
class FUZE_API MouseMovedEvent : public Event {
  public:
    MouseMovedEvent(float mouseX, float mouseY): m_MouseX(mouseX), m_MouseY(mouseY) {}

    float GetX() const { return m_MouseX; }
    float GetY() const { return m_MouseY; }

    inline std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseMoved: " << m_MouseX << " " << m_MouseY;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseMoved)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

  private:
    float m_MouseX, m_MouseY;
};

class FUZE_API MouseScrolledEvent : public Event {
  public:
    MouseScrolledEvent(float xOffset, float yOffset): m_XOffset(xOffset), m_YOffset(yOffset) {}

    float GetXOffset() const { return m_XOffset; }
    float GetYOffset() const { return m_YOffset; }

    inline std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseScrolled: " << m_XOffset << " " << m_YOffset;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseScrolled)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

  private:
    float m_XOffset, m_YOffset;
};

class FUZE_API MouseButtonEvent : public Event {
  public:
    int GetMouseButton() const { return m_Button; }

    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

  protected:
    MouseButtonEvent(int button): m_Button(button) {}
    int m_Button;
};

class FUZE_API MouseButtonPressedEvent : public MouseButtonEvent {
  public:
    MouseButtonPressedEvent(int button): MouseButtonEvent(button) {}

    inline std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseButtonPressed: " << GetMouseButton();
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonPressed)
};

class FUZE_API MouseButtonReleasedEvent : public MouseButtonEvent {
  public:
    MouseButtonReleasedEvent(int button): MouseButtonEvent(button) {}

    inline std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseButtonReleased: " << GetMouseButton();
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonReleased)
};
}
