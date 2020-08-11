#include "mapobjectitem.h"
#include "objectlayeritem.h"
#include "customattributes.h"
#include "statemachine.h"
#include "textdata.h"
#include "drawutils.h"

#include <QQmlEngine>
#include <QTransform>


namespace TiledQuick
{
QColor const DEBUG_PEN_COLOR = Qt::red;
int const DEBUG_PEN_WIDTH = 6;
int const DEBUG_POINT_WIDTH = 15;

MapObjectItem::MapObjectItem(Tiled::MapObject* mapObject, ObjectLayerItem* parent)
    : TiledItem(mapObject, parent)
    , m_stateMachine(new StateMachine(this))
    , m_drawDebug(false)
    , m_pressByShape(false)
    , m_clikable(false)
    , m_pressed(false)
{
    transformPolygon();

    setObjectName(mapObject->name());
    setSize(mapObject->hasDimensions() ? mapObject->size() : mapObject->polygon().boundingRect().size());
    setTransformOrigin(convert(mapObject->alignment(mapObject->map())));
    setPosition(convertCoordinates(mapObject->bounds(), mapObject->alignment()));
    setRotation(mapObject->rotation());
    setVisible(mapObject->isVisible());

    validateObjectName();

    if (mapObject->shape() == Tiled::MapObject::Shape::Text)
    {
        m_textData = new TextData(mapObject->textData(), this);
        connect(m_textData, &TextData::changed, this, [this] () { update(); });
    }

    m_pressCallback = compileCallback(PRESS_CALLBACK_NAME);
    m_releaseCallback = compileCallback(RELEASE_CALLBACK_NAME);
    m_clickCallback = compileCallback(CLICK_CALLBACK_NAME);
    m_drawDebug = mapObject->properties().value(DRAW_DEBUG_NAME, m_drawDebug).toBool();
    m_pressByShape = mapObject->properties().value(PRESS_BY_SHAPE_NAME, m_pressByShape).toBool();
    m_clikable = mapObject->properties().value(CLICKABLE_NAME, m_clikable).toBool();

    if (m_clikable)
    {
        setAcceptedMouseButtons(Qt::AllButtons);
    }
}

StateMachine* MapObjectItem::stateMachine() const
{
    return m_stateMachine;
}

TextData* MapObjectItem::textData() const
{
    return m_textData;
}

ObjectLayerItem* MapObjectItem::objectLayer() const
{
    return static_cast<ObjectLayerItem*>(parent());
}

Tiled::MapObject* MapObjectItem::mapObject() const
{
    return object<Tiled::MapObject>();
}

int MapObjectItem::id() const
{
    return mapObject()->id();
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
        return QQuickItem::Right;
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
    m_stateMachine->paint(painter);

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
    invokeCallback(m_pressCallback);
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
    invokeCallback(m_releaseCallback);

    if (containsPoint(event->pos()))
    {
        invokeCallback(m_clickCallback);
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
        painter->setPen(penPoint);
        painter->drawPoint(mapObject()->polygon().first());
        break;
    case Tiled::MapObject::Point:
        break;
    }
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
}
