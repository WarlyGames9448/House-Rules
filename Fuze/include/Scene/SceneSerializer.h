#pragma once

#include "Scene.h"
#include <yaml-cpp/yaml.h>

namespace Fuze {
class SceneSerializer {
  public:
    SceneSerializer(const Ref<Scene>& scene);

    void SerializeText(const std::string& filepath);
    void SerializeBinary(const std::string& filepath);

    void DeserializeText(const std::string& filepath);
    void DeserializeBinary(const std::string& filepath);

    void SerializeEntity(YAML::Emitter& out, Entity entity);

  private:
    Ref<Scene> m_Scene;
};
}
