#pragma once

#include <QVector2D>
#include <QPainter>
#include <QPixmap>
#include <QColor>
#include <QRect>


namespace TiledQuick
{
class DrawUtils
{
public:
    static void paintPixmap(QPainter* painter, QRect const& target, QRect const& source, QPixmap const& pixmap, QColor const& tint = QColor());
    static QPointF rotate(QPointF const& pivot, QPointF const& target, qreal angle);
    static qreal angle(QVector2D const& v1, QVector2D const& v2);
};
}

