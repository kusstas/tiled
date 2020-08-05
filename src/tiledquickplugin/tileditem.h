#pragma once

#include <QQuickPaintedItem>
#include <QJSValue>

namespace Tiled
{
class Map;
class MapRenderer;
class Object;
}

namespace TiledQuick
{
class ScriptEngine;

struct Callback
{
    QString name;
    QJSValue value;
};

class TiledItem : public QQuickPaintedItem
{
    Q_OBJECT

public:
    static QString const OBJECT_PATH_DELIM;

    TiledItem(Tiled::Object* object, TiledItem* parent);
    TiledItem(Tiled::Object* object, TiledItem* provider, QQuickItem* parent);
    ~TiledItem() override;

    Tiled::Object* object() const;
    template <class T> T* object() const;
    TiledItem* provider() const;

    virtual Tiled::Map* map();
    virtual Tiled::MapRenderer* renderer();
    virtual ScriptEngine* scriptEngine();
    virtual QQmlEngine* qqmlEngine();

    virtual void start();

protected:
    void paint(QPainter* painter) override;
    Callback compileCallback(QString const& name);
    void invokeCallback(Callback& callback);
    void validateObjectName();

private:
    static QString const JS_CALLBACK_FORMAT;

    Tiled::Object* m_object;
    TiledItem* m_provider;
    Callback m_startCallback;
    Callback m_destroyCallback;
};

template <class T>
T* TiledItem::object() const
{
    return static_cast<T*>(object());
}
}

