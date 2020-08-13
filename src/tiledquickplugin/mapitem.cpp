#include "mapitem.h"
#include "objectlayeritem.h"
#include "mapobjectitem.h"
#include "layerscreator.h"


namespace TiledQuick
{
int const MapItem::DETECT_COLLIDING_INTERVAL = 2;

MapItem::MapItem(QSharedPointer<Tiled::Map> const& map,
                 QSharedPointer<Tiled::MapRenderer> const& renderer,
                 QPointer<QObject> const& external,
                 QQuickItem* parent)
    : TiledItem(map.get(), this, parent)
    , m_map(map)
    , m_renderer(renderer)
    , m_scriptEngine(this)
    , m_external(external)
{
    if (m_map->backgroundColor().isValid())
    {
        setFillColor(m_map->backgroundColor());
    }

    setSize(QSize(m_map->tileWidth() * m_map->width(), m_map->tileHeight() * m_map->height()));
    LayersCreator::create(m_map->layers(), this);

    m_detectCollidingTimer.setSingleShot(false);
    m_detectCollidingTimer.setTimerType(Qt::PreciseTimer);
    connect(&m_detectCollidingTimer, &QTimer::timeout, this, &MapItem::detectColliding);

    start();
}

MapItem::~MapItem()
{
    destroyItem();
}

QObject* MapItem::external()
{
    return m_external;
}

Tiled::Map* MapItem::map()
{
    return m_map.get();
}

Tiled::MapRenderer* MapItem::renderer()
{
    return m_renderer.get();
}

ScriptEngine* MapItem::scriptEngine()
{
    return &m_scriptEngine;
}

QQmlEngine* MapItem::qqmlEngine()
{
    return qmlEngine(parentItem());
}

void MapItem::resetExternal(QPointer<QObject> const& external)
{
    m_external = external;
}

void MapItem::detectColliding()
{
}
}
