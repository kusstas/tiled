#include "mapobjectitem.h"
#include "objectlayeritem.h"
#include "drawutils.h"

#include <QQuickItem>
#include <QQmlEngine>
#include <QTransform>


namespace TiledQuick
{
constexpr auto ON_PRESS_CALLBACK_NAME = "onPress";
constexpr auto ON_CLICK_CALLBACK_NAME = "onClick";
constexpr auto ON_RELEASE_CALLBACK_NAME = "onRelease";
constexpr auto DRAW_DEBUG_NAME = "drawDebug";
constexpr auto PRESS_BY_SHAPE_NAME = "pressByShape";
constexpr auto PRESS_GUARD_NAME = "pressGuard";
QString const JS_CALLBACK_FORMAT = "(function(obj) { %1 })";
QColor const DEBUG_PEN_COLOR = Qt::red;
int const DEBUG_PEN_WIDTH = 6;
int const DEBUG_POINT_WIDTH = 15;

MapObjectItem::MapObjectItem(Tiled::MapObject* mapObject, ObjectLayerItem* parent)
    : QQuickPaintedItem(parent)
    , m_mapObject(mapObject)
    , m_drawDebug(false)
    , m_pressByShape(false)
    , m_pressGuard(false)
    , m_pressed(false)
{
    transformPolygon();

    setObjectName(mapObject->name());
    setParentItem(parent);
    setSize(mapObject->hasDimensions() ? mapObject->size() : mapObject->polygon().boundingRect().size());
    setTransformOrigin(convert(mapObject->alignment(mapObject->map())));
    setPosition(convertCoordinates(mapObject->bounds(), mapObject->alignment()));
    setRotation(mapObject->rotation());
    setVisible(mapObject->isVisible());

    m_onPressCallback = compileCallback(ON_PRESS_CALLBACK_NAME);
    m_onReleaseCallback = compileCallback(ON_RELEASE_CALLBACK_NAME);
    m_onClickCallback = compileCallback(ON_CLICK_CALLBACK_NAME);
    m_drawDebug = mapObject->properties().value(DRAW_DEBUG_NAME, m_drawDebug).toBool();
    m_pressByShape = mapObject->properties().value(PRESS_BY_SHAPE_NAME, m_pressByShape).toBool();
    m_pressGuard = mapObject->properties().value(PRESS_GUARD_NAME, m_pressGuard).toBool();

    if (clickable())
    {
        setAcceptedMouseButtons(Qt::AllButtons);
    }
}

ObjectLayerItem* MapObjectItem::objectLayer() const
{
    return static_cast<ObjectLayerItem*>(parent());
}

Tiled::MapObject* MapObjectItem::mapObject() const
{
    return m_mapObject;
}

QPointF MapObjectItem::convertCoordinates(QRectF const& rect, Tiled::Alignment origin)
{
    auto const xLeft = rect.x();
    auto const xCenter = rect.x() - (rect.width() / 2);
    auto const xRight = rect.x() - rect.width();

    auto const yTop = rect.y();
    auto const yCenter = rect.y() - (rect.height() / 2);
    auto const yBottom = rect.y() - rect.height();

    qreal x, y;

    switch (origin)
    {
    case Tiled::Unspecified:
    case Tiled::TopLeft:
        x = xLeft;
        y = yTop;
        break;
    case Tiled::Top:
        x = xCenter;
        y = yTop;
        break;
    case Tiled::TopRight:
        x = xRight;
        y = yTop;
        break;
    case Tiled::Left:
        x = xLeft;
        y = yCenter;
        break;
    case Tiled::Center:
        x = xCenter;
        y = yCenter;
        break;
    case Tiled::Right:
        x = xRight;
        y = yCenter;
        break;
    case Tiled::BottomLeft:
        x = xLeft;
        y = yBottom;
        break;
    case Tiled::Bottom:
        x = xCenter;
        y = yBottom;
        break;
    case Tiled::BottomRight:
        x = xRight;
        y = yBottom;
        break;
    }

    return {x, y};
}

QQuickItem::TransformOrigin MapObjectItem::convert(Tiled::Alignment origin)
{
    switch (origin)
    {
    case Tiled::Unspecified:
    case Tiled::TopLeft:
        return QQuickItem::TopLeft;
    case Tiled::Top:
        return QQuickItem::Top;
    case Tiled::TopRight:
        return QQuickItem::TopRight;
    case Tiled::Left:
        return QQuickItem::Left;
    case Tiled::Center:
        return QQuickItem::Center;
    case Tiled::Right:
        return QQuickItem::Right;;
    case Tiled::BottomLeft:
        return QQuickItem::BottomLeft;
    case Tiled::Bottom:
        return QQuickItem::Bottom;
    case Tiled::BottomRight:
        return QQuickItem::BottomRight;
        break;
    }

    return QQuickItem::TopLeft;
}

void MapObjectItem::paint(QPainter* painter)
{
    if (!mapObject()->cell().isEmpty())
    {
        auto pixmap = mapObject()->cell().tileset()->tileAt(mapObject()->cell().tileId())->image();

        int scaleX = mapObject()->cell().flippedHorizontally() ? -1 : 1;
        int scaleY = mapObject()->cell().flippedVertically() ? -1 : 1;

        if (scaleX < 0 || scaleY < 0)
        {
            QTransform transform;
            transform.scale(scaleX, scaleY);

            pixmap = pixmap.transformed(transform);
        }

        QRect const target(0, 0, width(), height());
        QRect const source(0, 0, pixmap.width(), pixmap.height());

        DrawUtils::paintPixmap(painter, target, source, pixmap, objectLayer()->tintColor());
    }

    if (m_drawDebug)
    {
        drawDebug(painter);
    }
}

void MapObjectItem::mousePressEvent(QMouseEvent* event)
{
    if (!containsPoint(event->pos()))
    {
        QQuickPaintedItem::mousePressEvent(event);
        return;
    }

    m_pressed = true;

    event->accept();
    invokeCallback(m_onPressCallback, ON_PRESS_CALLBACK_NAME);
}

void MapObjectItem::mouseReleaseEvent(QMouseEvent* event)
{
    if (!m_pressed)
    {
        QQuickPaintedItem::mouseReleaseEvent(event);
        return;
    }

    m_pressed = false;


    event->accept();
    invokeCallback(m_onReleaseCallback, ON_RELEASE_CALLBACK_NAME);

    if (containsPoint(event->pos()))
    {
        invokeCallback(m_onClickCallback, ON_CLICK_CALLBACK_NAME);
    }
}

void MapObjectItem::transformPolygon()
{
    if (!mapObject()->polygon().isEmpty())
    {
        auto rect = mapObject()->polygon().boundingRect();
        auto const translated = mapObject()->polygon().translated(-rect.topLeft());

        mapObject()->setPolygon(translated);
        mapObject()->setPosition(mapObject()->position() + rect.topLeft());

        rect.translate(-rect.topLeft());
        QPointF const pivot = mapObject()->position() + translated.first();
        QPointF const translatedPos = DrawUtils::rotate(pivot, mapObject()->position(), mapObject()->rotation());
        QVector2D const origin(1, 0);
        QVector2D const currentTilt(translated.first().x(), translated.first().y());
        QVector2D const previousTilt(pivot.x() - translatedPos.x(), pivot.y() - translatedPos.y());
        qreal const angle = DrawUtils::angle(origin.normalized(), previousTilt.normalized())
                            - DrawUtils::angle(origin.normalized(), currentTilt.normalized());

        mapObject()->setPosition(translatedPos);
        mapObject()->setRotation(angle);
    }
}

void MapObjectItem::drawDebug(QPainter* painter)
{
    QPen penLine(DEBUG_PEN_COLOR), penPoint(DEBUG_PEN_COLOR);
    penLine.setWidth(DEBUG_PEN_WIDTH);
    penPoint.setWidth(DEBUG_POINT_WIDTH);

    painter->setPen(penLine);

    auto const penMargin = penLine.width() / 2;
    QRect const bounding = QRect(penMargin, penMargin, width() - penLine.width(), height() - penLine.width());

    switch (mapObject()->shape())
    {
    case Tiled::MapObject::Rectangle:
    case Tiled::MapObject::Text:
        painter->drawRect(bounding);
        break;
    case Tiled::MapObject::Polygon:
        painter->drawPolygon(mapObject()->polygon());
        painter->setPen(penPoint);
        painter->drawPoint(mapObject()->polygon().first());
        break;
    case Tiled::MapObject::Ellipse:
        painter->drawEllipse(bounding);
        break;
    case Tiled::MapObject::Polyline:
        painter->drawPolyline(mapObject()->polygon());
        break;
    case Tiled::MapObject::Point:
        break;
        break;
    }
}

bool MapObjectItem::clickable() const
{
    return m_onPressCallback.isCallable() || m_onReleaseCallback.isCallable() || m_onClickCallback.isCallable() || m_pressGuard;
}

bool MapObjectItem::containsPoint(QPointF const& localPos) const
{
    if (m_pressByShape)
    {
        switch (mapObject()->shape())
        {
        case Tiled::MapObject::Rectangle:
        case Tiled::MapObject::Text:
            return containsPointInBoundingRect(localPos);
        case Tiled::MapObject::Polygon:
            return mapObject()->polygon().containsPoint(localPos, Qt::WindingFill);
        case Tiled::MapObject::Ellipse:
            return (((localPos.x() * localPos.x()) / (width() * width()))
                    + ((localPos.y() * localPos.y()) / (height() * height()))) <= 1.0;
        case Tiled::MapObject::Polyline:
        case Tiled::MapObject::Point:
            break;
        }

        return false;
    }

    return containsPointInBoundingRect(localPos);
}

bool MapObjectItem::containsPointInBoundingRect(QPointF const& localPos) const
{
    return QRectF(0, 0, width(), height()).contains(localPos);
}

QJSValue MapObjectItem::compileCallback(QString const& name)
{
    auto const value = mapObject()->properties().value(name);

    if (!value.isValid())
    {
        return {};
    }

    if (value.type() != QVariant::String)
    {
        qCritical() << "Callback:" << name << "should be string! Object:" << objectName();
        return {};
    }

    auto const jsValue = objectLayer()->qqmlEngine()->evaluate(JS_CALLBACK_FORMAT.arg(value.toString()));

    if (jsValue.isError())
    {
        qCritical() << "Can't compile callback:" << name << "object:" << objectName() << jsValue.toString();
        return {};
    }

    return jsValue;
}

void MapObjectItem::invokeCallback(QJSValue& callback, QString const& name)
{
    if (!callback.isCallable())
    {
        return;
    }

    auto const result = callback.call({objectLayer()->qqmlEngine()->toScriptValue(this)});

    if (result.isError())
    {
        qCritical() << "Can't compile callback:" << name << "object:" << objectName() << result.toString();
    }
}
}
