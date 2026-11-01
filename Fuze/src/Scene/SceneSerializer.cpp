#include "Scene/SceneSerializer.h"

#include <yaml-cpp/yaml.h>

#include "Utils/FileUtils.h"

namespace YAML {
template <> struct convert<glm::vec3> {
    static Node encode(const glm::vec3& rhs) {
        Node node;
        node.push_back(rhs.x);
        node.push_back(rhs.y);
        node.push_back(rhs.z);
        return node;
    }

    static bool decode(const Node& node, glm::vec3& rhs) {
        if (!node.IsSequence() || node.size() != 3) return false;

        rhs.x = node[0].as<float>();
        rhs.y = node[1].as<float>();
        rhs.z = node[2].as<float>();
        return true;
    }
};
template <> struct convert<glm::vec4> {
    static Node encode(const glm::vec4& rhs) {
        Node node;
        node.push_back(rhs.x);
        node.push_back(rhs.y);
        node.push_back(rhs.z);
        node.push_back(rhs.w);
        return node;
    }

    static bool decode(const Node& node, glm::vec4& rhs) {
        if (!node.IsSequence() || node.size() != 4) return false;

        rhs.x = node[0].as<float>();
        rhs.y = node[1].as<float>();
        rhs.z = node[2].as<float>();
        rhs.w = node[3].as<float>();
        return true;
    }
};
}

namespace Fuze {
SceneSerializer::SceneSerializer(const Ref<Scene>& scene): m_Scene(scene) {
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) {
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
    return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
    return out;
}

void SerializeComponent(YAML::Emitter& out, TagComponent& component) {
    out << YAML::Key << "TagComponent";
    out << YAML::BeginMap;

    out << YAML::Key << "Tag" << YAML::Value << component.Tag;

    out << YAML::EndMap;
}

void SerializeComponent(YAML::Emitter& out, TransformComponent& component) {
    out << YAML::Key << "TransformComponent";
    out << YAML::BeginMap;

    out << YAML::Key << "Translation" << YAML::Value << component.Translation;
    out << YAML::Key << "Rotation" << YAML::Value << component.Rotation;
    out << YAML::Key << "Scale" << YAML::Value << component.Scale;

    out << YAML::EndMap;
}

void SerializeComponent(YAML::Emitter& out, SpriteRendererComponent& component) {
    out << YAML::Key << "SpriteRendererComponent";
    out << YAML::BeginMap;

    out << YAML::Key << "Color" << YAML::Value << component.Color;

    out << YAML::EndMap;
}

void SerializeComponent(YAML::Emitter& out, CameraComponent& component) {
    out << YAML::Key << "CameraComponent";
    out << YAML::BeginMap;

    out << YAML::Key << "Camera";
    out << YAML::BeginMap;

    out << YAML::Key << "Position" << YAML::Value << component.Camera->GetPosition();
    out << YAML::Key << "Rotation" << YAML::Value << component.Camera->GetRotation();
    out << YAML::Key << "AspectRatio" << YAML::Value << component.AspectRatio;
    out << YAML::Key << "Size" << YAML::Value << component.Size;
    out << YAML::Key << "NearClip" << YAML::Value << component.FarClip;
    out << YAML::Key << "FarClip" << YAML::Value << component.NearClip;

    out << YAML::EndMap;

    out << YAML::Key << "ControllerActive" << YAML::Value << component.ControllerActive;
    out << YAML::Key << "Primary" << YAML::Value << component.Primary;
    out << YAML::Key << "FixedAspectRatio" << YAML::Value << component.FixedAspectRatio;
    out << YAML::Key << "SizeChanged" << YAML::Value << component.SizeChanged;

    out << YAML::EndMap;
}

void SceneSerializer::SerializeEntity(YAML::Emitter& out, Entity entity) {
    if (!m_Scene->GetRegistry()->HasComponent<IDComponent>(entity)) {
        FUZE_CORE_ASSERT(0, "Entity {0} need IDComponent", entity);
        return;
    }
    auto& ID = m_Scene->GetRegistry()->GetComponent<IDComponent>(entity).ID;

    out << YAML::BeginMap;
    out << YAML::Key << "Entity" << YAML::Value << ID.ToString();

    if (m_Scene->GetRegistry()->HasComponent<TagComponent>(entity)) {
        auto& component = m_Scene->GetRegistry()->GetComponent<TagComponent>(entity);
        SerializeComponent(out, component);
    }

    if (m_Scene->GetRegistry()->HasComponent<TransformComponent>(entity)) {
        auto& component = m_Scene->GetRegistry()->GetComponent<TransformComponent>(entity);
        SerializeComponent(out, component);
    }

    if (m_Scene->GetRegistry()->HasComponent<SpriteRendererComponent>(entity)) {
        auto& component = m_Scene->GetRegistry()->GetComponent<SpriteRendererComponent>(entity);
        SerializeComponent(out, component);
    }

    if (m_Scene->GetRegistry()->HasComponent<CameraComponent>(entity)) {
        auto& component = m_Scene->GetRegistry()->GetComponent<CameraComponent>(entity);
        SerializeComponent(out, component);
    }

    out << YAML::EndMap;
}

bool SceneSerializer::SerializeText(const std::string& filepath) {
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Scene" << YAML::Value << m_Scene->GetName();
    out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

    for (Entity entity : m_Scene->GetRegistry()->GetEntities()) {
        if (entity == NULL_ENTITY) return false;

        SerializeEntity(out, entity);
    }

    out << YAML::EndMap;

    bool status = FileUtils::WriteTextFile(filepath, out.c_str());

    return status;
}

bool SceneSerializer::SerializeBinary(const std::string& filepath) {
    (void)filepath;
    return false;
}

bool SceneSerializer::DeserializeText(const std::string& filepath) {
    std::ifstream stream(filepath);
    std::stringstream strStream;
    strStream << stream.rdbuf();

    YAML::Node data = YAML::Load(strStream.str());
    if (!data["Scene"]) return false;

    std::string sceneName = data["Scene"].as<std::string>();
    FUZE_CORE_TRACE("Deserializing scene '{0}'", sceneName);
    m_Scene->SetName(sceneName);

    auto entities = data["Entities"];
    if (entities) {
        for (auto entity : entities) {
            auto id = entity["Entity"].as<std::string>();

            std::string name;
            auto tagComponent = entity["TagComponent"];
            if (tagComponent) name = tagComponent["Tag"].as<std::string>();

            FUZE_CORE_TRACE("Deserializing entity with ID = {0}, name = {1}", id, name);

            Entity deserializedEntity = m_Scene->CreateEntityWithUUID(id, name);

            auto transformComponent = entity["TransformComponent"];
            if (transformComponent) {
                auto transform = transformComponent["Translation"].as<glm::vec3>();
                auto rotation = transformComponent["Rotation"].as<glm::vec3>();
                auto scale = transformComponent["Scale"].as<glm::vec3>();

                m_Scene->GetRegistry()->AddComponent<TransformComponent>(deserializedEntity, {transform, rotation, scale});
            }

            auto spriteComponent = entity["SpriteRendererComponent"];
            if (spriteComponent) {
                auto color = spriteComponent["Color"].as<glm::vec4>();

                m_Scene->GetRegistry()->AddComponent<SpriteRendererComponent>(deserializedEntity, color);
            }

            auto cameraComponent = entity["CameraComponent"];

            if (cameraComponent) {
                auto camera = cameraComponent["Camera"];
                auto position = camera["Position"].as<glm::vec3>();
                auto rotation = camera["Rotation"].as<float>();
                auto aspectRatio = camera["AspectRatio"].as<float>();
                auto size = camera["Size"].as<float>();

                m_Scene->CreateCamera(deserializedEntity, aspectRatio, size);

                auto& cc = m_Scene->GetRegistry()->GetComponent<CameraComponent>(deserializedEntity);

                cc.AspectRatio = aspectRatio;
                cc.Size = size;
                cc.NearClip = camera["NearClip"].as<float>();
                cc.FarClip = camera["FarClip"].as<float>();

                cc.Camera->SetPositionAndRotation(position, rotation);

                cc.RecalculateProjection();

                cc.ControllerActive = cameraComponent["ControllerActive"].as<bool>();
                auto primary = cameraComponent["Primary"].as<bool>();
                cc.Primary = primary;
                cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
                cc.SizeChanged = cameraComponent["SizeChanged"].as<bool>();

                if (primary) m_Scene->SetPrimaryCamera(deserializedEntity);
            }
        }
    }

    return true;
}

bool SceneSerializer::DeserializeBinary(const std::string& filepath) {
    (void)filepath;
    return false;
}

}
