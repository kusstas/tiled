#include "scriptengine.h"
#include "tileditem.h"
#include "mapobjectitem.h"

#include "pathanimation.h"
#include "polygonpathanimation.h"
#include "ellipsepathanimation.h"


namespace TiledQuick
{
ScriptEngine::ScriptEngine(TiledItem* provider, QObject* parent)
    : QObject(parent)
    , m_provider(provider)
{
}

TiledItem* ScriptEngine::find(QString const& path) const
{
    auto const objs = path.splitRef(TiledItem::OBJECT_PATH_DELIM);

    TiledItem* child = m_provider;

    for (auto const& name : objs)
    {
        child = child->findChild<TiledItem*>(name.toString(), Qt::FindDirectChildrenOnly);

        if (!child)
        {
            break;
        }
    }

    return child;
}

TiledItem* ScriptEngine::find(int id) const
{
    auto const children = m_provider->findChildren<TiledItem*>();

    auto const finded = std::find_if(children.cbegin(), children.cend(), [id] (TiledItem* item)
    {
        return item->id() == id;
    });

    return finded != children.cend() ? *finded : nullptr;
}

PathAnimation* ScriptEngine::getPathAnimation(TiledItem* object) const
{
    return object->findChild<PathAnimation*>(QString(), Qt::FindDirectChildrenOnly);
}

QVariantList ScriptEngine::getPathAnimations(TiledItem* object) const
{
    auto const animations = object->findChildren<PathAnimation*>(QString(), Qt::FindDirectChildrenOnly);

    QVariantList out;
    out.reserve(animations.size());

    for (auto const& animation : animations)
    {
        out << QVariant::fromValue(animation);
    }

    return out;
}

PathAnimation* ScriptEngine::createPathAnimation(TiledItem* object, MapObjectItem* path) const
{
    PathAnimation* animation = nullptr;

    if (!object)
    {
        qCritical() << "Can't create path animation: object is null";
        return animation;
    }
    if (!path)
    {
        qCritical() << "Can't create path animation: path is null";
        return animation;
    }

    switch (path->mapObject()->shape())
    {
    case Tiled::MapObject::Rectangle:
    case Tiled::MapObject::Polygon:
    case Tiled::MapObject::Polyline:
        animation = new PolygonPathAnimation(object, path);
        break;
    case Tiled::MapObject::Ellipse:
        animation = new EllipsePathAnimation(object, path);
        break;
    case Tiled::MapObject::Text:
    case Tiled::MapObject::Point:
        qCritical() << "Can't create path animation: path should be [Rectangle. Polygon, Polyline, Ellipse])";
        break;
    }

    return animation;
}
}
