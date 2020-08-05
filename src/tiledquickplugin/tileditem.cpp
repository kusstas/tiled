#include "tileditem.h"
#include "customattributes.h"
#include "scriptengine.h"
#include "object.h"

#include <QQmlEngine>


namespace TiledQuick
{
QString const TiledItem::OBJECT_PATH_DELIM = ".";
QString const TiledItem::JS_CALLBACK_FORMAT = "(function(object, engine) { %1 })";

TiledItem::TiledItem(Tiled::Object* object, TiledItem* parent)
    : TiledItem(object, parent, parent)
{
}

TiledItem::TiledItem(Tiled::Object* object, TiledItem* provider, QQuickItem* parent)
    : QQuickPaintedItem(parent)
    , m_object(object)
    , m_provider(provider)
{
    setParentItem(parent);

    m_startCallback = compileCallback(START_CALLBACK_NAME);
    m_destroyCallback = compileCallback(DESTROY_CALLBACK_NAME);
}

TiledItem::~TiledItem()
{
    invokeCallback(m_destroyCallback);
}

Tiled::Object* TiledItem::object() const
{
    return m_object;
}

TiledItem* TiledItem::provider() const
{
    return m_provider;
}

Tiled::Map* TiledItem::map()
{
    return m_provider->map();
}

Tiled::MapRenderer* TiledItem::renderer()
{
    return m_provider->renderer();
}

ScriptEngine* TiledItem::scriptEngine()
{
    return m_provider->scriptEngine();
}

QQmlEngine* TiledItem::qqmlEngine()
{
    return m_provider->qqmlEngine();
}

void TiledItem::start()
{
    invokeCallback(m_startCallback);
}

void TiledItem::paint(QPainter* /*painter*/)
{
}

Callback TiledItem::compileCallback(QString const& name)
{
    auto const value = object()->properties().value(name);

    if (!value.isValid())
    {
        return {};
    }

    if (value.type() != QVariant::String)
    {
        qCritical() << "Callback:" << name << "should be string! Object:" << objectName();
        return {};
    }

    auto const jsValue = qqmlEngine()->evaluate(JS_CALLBACK_FORMAT.arg(value.toString()));

    if (jsValue.isError())
    {
        qCritical() << "Can't compile callback:" << name << "object:" << objectName() << jsValue.toString();
        return {};
    }

    return {name, jsValue};
}

void TiledItem::invokeCallback(Callback& callback)
{
    if (!callback.value.isCallable())
    {
        return;
    }

    auto const result = callback.value.call({qqmlEngine()->toScriptValue(this), qqmlEngine()->toScriptValue(scriptEngine())});

    if (result.isError())
    {
        qCritical() << "Can't compile callback:" << callback.name << "object:" << objectName() << result.toString();
    }
}

void TiledItem::validateObjectName()
{
    if (objectName().contains(OBJECT_PATH_DELIM))
    {
        qWarning() << "Object name:" << objectName() << "should not contains: \"" << OBJECT_PATH_DELIM << "\"";
    }
}
}
