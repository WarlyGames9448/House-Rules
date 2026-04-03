#pragma once

#include "Core.h"
#include "Event.h"

namespace Fuze {
class FUZE_API WindowResizedEvent : public Event {
  public:
    WindowResizedEvent(float width, float height): m_Width(width), m_Height(height) {}

    float GetWidth() const { return m_Width; }
    float GetHeight() const { return m_Height; }

    inline std::string ToString() const override {
        std::stringstream ss;
        ss << "WindowResized: " << m_Width << " " << m_Height;
        return ss.str();
    }

    EVENT_CLASS_TYPE(WindowResize)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)

  private:
    float m_Width, m_Height;
};

class FUZE_API WindowCloseEvent : public Event {
  public:
    EVENT_CLASS_TYPE(WindowClose)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class FUZE_API WindowLostFocusEvent : public Event {
  public:
    EVENT_CLASS_TYPE(WindowLostFocus)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class FUZE_API WindowFocusEvent : public Event {
  public:
    EVENT_CLASS_TYPE(WindowFocus)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class FUZE_API AppUpdateEvent : public Event {
  public:
    EVENT_CLASS_TYPE(AppUpdate)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class FUZE_API AppTickEvent : public Event {
  public:
    EVENT_CLASS_TYPE(AppTick)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class FUZE_API AppRendeEvent : public Event {
  public:
    EVENT_CLASS_TYPE(AppRender)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};
}
