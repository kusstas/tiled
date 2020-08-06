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

    TiledItem* parentTiledItem() const;
    Tiled::Object* object() const;
    template <class T> T* object() const;
    TiledItem* provider() const;
    bool isProvider() const;

    virtual Tiled::Map* map();
    virtual Tiled::MapRenderer* renderer();
    virtual ScriptEngine* scriptEngine();
    virtual QQmlEngine* qqmlEngine();

    Q_INVOKABLE QString objectNamePath() const;

    virtual void start();

protected:
    void paint(QPainter* painter) override;

    Callback compileCallback(QString const& name, QStringList const& params = {});
    void invokeCallback(Callback& callback, QVariantList const& params = {});

    void validateObjectName();

private:
    Callback compileCommonCallback(QString const& name, QStringList const& params);
    void invokeCommonCallback(Callback& callback, QVariantList const& params);

private:
    static QString const JS_CALLBACK_FORMAT;
    static QStringList const JS_DEFAULT_PARAMS;

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

