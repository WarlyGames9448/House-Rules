#pragma once

#include "Scene/Entity.h"

namespace Fuze {
    class EditorSelectionContext {
    public:
        void SelectEntity(Entity entity)
        {
            m_SelectedEntity = entity;
        }

        void ClearSelection()
        {
            m_SelectedEntity = NULL_ENTITY;
        }

        Entity GetSelectedEntity() const
        {
            return m_SelectedEntity;
        }

        bool HasSelection() const
        {
            return m_SelectedEntity != NULL_ENTITY;
        }

    private:
        Entity m_SelectedEntity = NULL_ENTITY;
    };

}
