#include "Fuze.h"
#include "EntryPoint.h"

#include "EditorLayer.h"

class Editor : public Fuze::Application {
  public:
    Editor() {
        PushLayer(new Fuze::EditorLayer());

        this->Get().GetWindow().SetTitle("FuzeEditor");
    }

    ~Editor() {
    }
};

Fuze::Application* Fuze::CreateApplication() {
    return new Editor();
}
