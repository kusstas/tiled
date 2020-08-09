#include "tileditem.h"
#include "customattributes.h"
#include "scriptengine.h"
#include "object.h"

#include <QQmlEngine>
#include <QStringBuilder>


namespace TiledQuick
{
QString const TiledItem::OBJECT_PATH_DELIM = ".";
QString const TiledItem::JS_CALLBACK_FORMAT = "(function(%1) { %2 })";
QStringList const TiledItem::JS_DEFAULT_PARAMS = {"object", "engine", "external"};
constexpr auto PARAMS_JOIN = ",";

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

TiledItem* TiledItem::parentTiledItem() const
{
    return qobject_cast<TiledItem*>(parentItem());
}

Tiled::Object* TiledItem::object() const
{
    return m_object;
}

TiledItem* TiledItem::provider() const
{
    return m_provider;
}

bool TiledItem::isProvider() const
{
    return m_provider == this;
}

int TiledItem::id() const
{
    return 0;
}

QObject* TiledItem::external()
{
    return m_provider->external();
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

QString TiledItem::objectNamePath() const
{
    if (isProvider() || parentTiledItem()->isProvider())
    {
        return objectName();
    }

    return parentTiledItem()->objectNamePath() % OBJECT_PATH_DELIM % objectName();
}

QVariant TiledItem::getTiledProperty(QString const& name) const
{
    return object()->property(name);
}

void TiledItem::setTiledProperty(QString const& name, QVariant const& value)
{
    object()->setProperty(name, value);
}

void TiledItem::start()
{
    invokeCallback(m_startCallback);
}

void TiledItem::paint(QPainter* /*painter*/)
{
}

Callback TiledItem::compileCallback(QString const& name, QStringList const& params)
{
    QStringList allParams = JS_DEFAULT_PARAMS;
    allParams.append(params);

    return compileCommonCallback(name, allParams);
}

void TiledItem::invokeCallback(Callback& callback, QVariantList const& params)
{
    QVariantList allParams = {QVariant::fromValue(this),
                              QVariant::fromValue(scriptEngine()),
                              QVariant::fromValue(external())};
    allParams.append(params);

    return invokeCommonCallback(callback, allParams);
}

void TiledItem::validateObjectName()
{
    if (objectName().contains(OBJECT_PATH_DELIM))
    {
        qWarning() << "Object name:" << objectName() << "should not contains: \"" << OBJECT_PATH_DELIM << "\"";
    }
}

Callback TiledItem::compileCommonCallback(QString const& name, QStringList const& params)
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

    auto const jsParams = params.join(PARAMS_JOIN);
    auto const jsValue = qqmlEngine()->evaluate(JS_CALLBACK_FORMAT.arg(jsParams, value.toString()));

    if (jsValue.isError())
    {
        qCritical() << "Can't compile callback:" << name << "object:" << objectName() << jsValue.toString();
        return {};
    }

    return {name, jsValue};
}

void TiledItem::invokeCommonCallback(Callback& callback, QVariantList const& params)
{
    if (!callback.value.isCallable())
    {
        return;
    }

    QJSValueList jsParams;
    jsParams.reserve(params.size());

    for (auto const& param : params)
    {
        jsParams << qqmlEngine()->toScriptValue(param);
    }

    auto const result = callback.value.call(jsParams);

    if (result.isError())
    {
        qCritical() << "Can't compile callback:" << callback.name << "object:" << objectName() << result.toString();
    }
}
}
