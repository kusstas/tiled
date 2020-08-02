#include "drawutils.h"

#include <cmath>

void TiledQuick::DrawUtils::paintPixmap(QPainter* painter, QRect const& target, QRect const& source, QPixmap const& pixmap, QColor const& tint)
{
    auto const prevMode = painter->compositionMode();

    if (tint.isValid())
    {
        QPixmap overlay(source.size());
        QPainter overlayPainter(&overlay);

        overlayPainter.fillRect(source, tint);
        overlayPainter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
        overlayPainter.drawPixmap(source, pixmap, source);

        painter->drawPixmap(target, overlay, source);
        painter->setCompositionMode(QPainter::CompositionMode_Multiply);
    }

    painter->drawPixmap(target, pixmap, source);
    painter->setCompositionMode(prevMode);
}

QPointF TiledQuick::DrawUtils::rotate(QPointF const& pivot, QPointF const& target, qreal angle)
{
    qreal const p = (angle / 180) * M_PI;

    qreal s = std::sin(p);
    qreal c = std::cos(p);

    QPointF tr = target - pivot;

    qreal xnew = tr.x() * c - tr.y() * s;
    qreal ynew = tr.x() * s + tr.y() * c;


    return QPointF(xnew, ynew) + pivot;
}

qreal TiledQuick::DrawUtils::angle(QVector2D const& v1, QVector2D const& v2)
{
    qreal dot = QVector2D::dotProduct(v1, v2);
    dot = ( dot < -1.0 ? -1.0 : ( dot > 1.0 ? 1.0 : dot ) );

    qreal angle = (std::acos( dot ) / M_PI) * 180;

    return angle;
}
