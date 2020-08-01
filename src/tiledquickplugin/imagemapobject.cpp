#include "imagemapobject.h"

#include "objectlayeritem.h"
#include "mapobject.h"

#include <QPainter>


namespace TiledQuick
{
ImageMapObject::ImageMapObject(Tiled::MapObject* mapObject, TiledQuick::ObjectLayerItem* parent)
    : QQuickPaintedItem(parent)
    , m_mapObject(mapObject)
{
    setObjectName(mapObject->name());
    setParentItem(parent);
    handleAlignment();
    setSize(QSize(mapObject->width(), mapObject->height()));
    setRotation(mapObject->rotation());
    setVisible(mapObject->isVisible());

    setAcceptedMouseButtons(Qt::AllButtons);

    update();
}

void ImageMapObject::paint(QPainter* painter)
{
    auto pixmap = m_mapObject->cell().tileset()->tileAt(m_mapObject->cell().tileId())->image();

    int scaleX = m_mapObject->cell().flippedHorizontally() ? -1 : 1;
    int scaleY = m_mapObject->cell().flippedVertically() ? -1 : 1;

    if (scaleX < 0 || scaleY < 0)
    {
        QTransform transform;
        transform.scale(scaleX, scaleY);

        pixmap = pixmap.transformed(transform);
    }

    painter->drawPixmap(QRect(0, 0, width(), height()), pixmap, QRect(0, 0, pixmap.width(), pixmap.height()));
}

void ImageMapObject::mousePressEvent(QMouseEvent* event)
{
    QQuickPaintedItem::mousePressEvent(event);
}

void ImageMapObject::mouseReleaseEvent(QMouseEvent* event)
{
    QQuickPaintedItem::mouseReleaseEvent(event);
}

void ImageMapObject::handleAlignment()
{
    auto const alignment = m_mapObject->alignment(m_mapObject->map());
    auto const xLeft = m_mapObject->x();
    auto const xCenter = m_mapObject->x() - (m_mapObject->width() / 2);
    auto const xRight = m_mapObject->x() - m_mapObject->width();

    auto const yTop = m_mapObject->y();
    auto const yCenter = m_mapObject->y() - (m_mapObject->height() / 2);
    auto const yBottom = m_mapObject->y() - m_mapObject->height();

    switch (alignment)
    {
    case Tiled::Unspecified:
    case Tiled::TopLeft:
        setX(xLeft);
        setY(yTop);
        break;
    case Tiled::Top:
        setTransformOrigin(QQuickItem::Top);
        setX(xCenter);
        setY(yTop);
        break;
    case Tiled::TopRight:
        setTransformOrigin(QQuickItem::TopRight);
        setX(xRight);
        setY(yTop);
        break;
    case Tiled::Left:
        setTransformOrigin(QQuickItem::Left);
        setX(xLeft);
        setY(yCenter);
        break;
    case Tiled::Center:
        setTransformOrigin(QQuickItem::Center);
        setX(xCenter);
        setY(yCenter);
        break;
    case Tiled::Right:
        setTransformOrigin(QQuickItem::Right);
        setX(xRight);
        setY(yCenter);
        break;
    case Tiled::BottomLeft:
        setTransformOrigin(QQuickItem::BottomLeft);
        setX(xLeft);
        setY(yBottom);
        break;
    case Tiled::Bottom:
        setTransformOrigin(QQuickItem::Bottom);
        setX(xCenter);
        setY(yBottom);
        break;
    case Tiled::BottomRight:
        setTransformOrigin(QQuickItem::BottomRight);
        setX(xRight);
        setY(yBottom);
        break;
    }
}
}
