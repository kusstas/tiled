#pragma once

#include <QQuickPaintedItem>
#include <QJSValue>
#include <QTimer>


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

    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(QString objectNamePath READ objectNamePath CONSTANT)
    Q_PROPERTY(QTimer* timer READ timer CONSTANT)

public:
    static QString const OBJECT_PATH_DELIM;

    TiledItem(Tiled::Object* object, TiledItem* parent);
    TiledItem(Tiled::Object* object, TiledItem* provider, QQuickItem* parent);

    TiledItem* parentTiledItem() const;
    Tiled::Object* object() const;
    template <class T> T* object() const;
    TiledItem* provider() const;
    bool isProvider() const;

    virtual int id() const;
    QString objectNamePath() const;
    QTimer* timer();

    virtual QObject* external();
    virtual Tiled::Map* map();
    virtual Tiled::MapRenderer* renderer();
    virtual ScriptEngine* scriptEngine();
    virtual QQmlEngine* qqmlEngine();

    void start();
    Q_INVOKABLE void destroyItem();
    Q_INVOKABLE QVariant getTiledProperty(QString const& name) const;
    Q_INVOKABLE void setTiledProperty(QString const& name, QVariant const& value);

protected:
    virtual void onDestroyItem();
    void paint(QPainter* painter) override;
    Callback compileCallback(QString const& name, QStringList const& params = {});
    void invokeCallback(Callback& callback, QVariantList const& params = {});
    void validateObjectName();

private:
    Callback compileCommonCallback(QString const& name, QStringList const& params);
    void invokeCommonCallback(Callback& callback, QVariantList const& params);
    void timeout();

private:
    static QString const JS_CALLBACK_FORMAT;
    static QStringList const JS_DEFAULT_PARAMS;

    QTimer m_timer;
    Tiled::Object* m_object;
    TiledItem* m_provider;
    Callback m_startCallback;
    Callback m_destroyCallback;
    Callback m_timeoutCallback;
};

template <class T>
T* TiledItem::object() const
{
    return static_cast<T*>(object());
}
}

