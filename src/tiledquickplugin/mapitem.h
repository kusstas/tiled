#pragma once

#include "tileditem.h"

#include "map.h"
#include "maprenderer.h"
#include "scriptengine.h"
#include "layerscontainer.h"

#include <QSharedPointer>
#include <QScopedPointer>


namespace TiledQuick
{
class MapItem : public TiledItem
{
    Q_OBJECT

public:
    MapItem(QSharedPointer<Tiled::Map> const& map, QSharedPointer<Tiled::MapRenderer> const& renderer, QQuickItem* parent);

    Tiled::Map* map() override;
    Tiled::MapRenderer* renderer() override;
    ScriptEngine* scriptEngine() override;
    QQmlEngine* qqmlEngine() override;

    void start() override;

private:
    QSharedPointer<Tiled::Map> m_map;
    QSharedPointer<Tiled::MapRenderer> m_renderer;
    QScopedPointer<LayersContainer> m_layersContainer;
    ScriptEngine m_scriptEngine;
};
} // namespace TiledQuick
