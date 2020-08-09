#pragma once

#include <QObject>
#include <QPointF>
#include <QVariantList>


namespace TiledQuick
{
class TiledItem;
class MapObjectItem;
class PathAnimation;

class ScriptEngine : public QObject
{
    Q_OBJECT

public:
    ScriptEngine(TiledItem* provider, QObject* parent = nullptr);

    Q_INVOKABLE TiledQuick::TiledItem* find(QString const& path) const;
    Q_INVOKABLE TiledQuick::TiledItem* find(int id) const;
    Q_INVOKABLE TiledQuick::PathAnimation* getPathAnimation(TiledQuick::TiledItem* object) const;
    Q_INVOKABLE QVariantList getPathAnimations(TiledQuick::TiledItem* object) const;
    Q_INVOKABLE TiledQuick::PathAnimation* createPathAnimation(TiledQuick::TiledItem* object, TiledQuick::MapObjectItem* path) const;

private:
    TiledItem* m_provider;
};
}

