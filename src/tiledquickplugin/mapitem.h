#pragma once

#include "tileditem.h"

#include "map.h"
#include "maprenderer.h"
#include "scriptengine.h"
#include "layerscontainer.h"

#include <QPointer>
#include <QSharedPointer>
#include <QScopedPointer>


namespace TiledQuick
{
class MapItem : public TiledItem
{
    Q_OBJECT

public:
    MapItem(QSharedPointer<Tiled::Map> const& map,
            QSharedPointer<Tiled::MapRenderer> const& renderer,
            QPointer<QObject> const& external,
            QQuickItem* parent);
    ~MapItem() override;

    QObject* external() override;
    Tiled::Map* map() override;
    Tiled::MapRenderer* renderer() override;
    ScriptEngine* scriptEngine() override;
    QQmlEngine* qqmlEngine() override;

    void start() override;
    void exit() override;
    void resetExternal(QPointer<QObject> const& external);

private:
    QSharedPointer<Tiled::Map> m_map;
    QSharedPointer<Tiled::MapRenderer> m_renderer;
    QScopedPointer<LayersContainer> m_layersContainer;
    ScriptEngine m_scriptEngine;
    QPointer<QObject> m_external;
};
} // namespace TiledQuick
