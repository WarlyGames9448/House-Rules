#include "Scene/SceneSerializer.h"

#include <yaml-cpp/yaml.h>

#include "Utils/FileUtils.h"

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
    out << YAML::BeginMap;
    out << YAML::Key << "Entity" << YAML::Value << entity;

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

void SceneSerializer::SerializeText(const std::string& filepath) {
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Scene" << YAML::Value << m_Scene->GetName();
    out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

    for (Entity entity : m_Scene->GetRegistry()->GetEntities()) {
        if (entity == NULL_ENTITY) return;

        SerializeEntity(out, entity);
    }

    out << YAML::EndMap;

    FileUtils::WriteTextFile(filepath, out.c_str());
}

void SceneSerializer::SerializeBinary(const std::string& filepath) {
    (void)filepath;
    return;
}

void SceneSerializer::DeserializeText(const std::string& filepath) {
    (void)filepath;
    return;
}

void SceneSerializer::DeserializeBinary(const std::string& filepath) {
    (void)filepath;
    return;
}

}
