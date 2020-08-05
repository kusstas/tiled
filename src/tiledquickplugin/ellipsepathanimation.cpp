#include "ellipsepathanimation.h"
#include "mapobjectitem.h"

#include <cmath>


namespace TiledQuick
{
constexpr auto FULL_CIRCLE = 2 * M_PI;

EllipsePathAnimation::EllipsePathAnimation(TiledItem* object, MapObjectItem* path)
    : PathAnimation(object, path)
    , m_interval(0)
    , m_startAngle(0)
    , m_endAngle(1)
    , m_angle(0)
{
}

int EllipsePathAnimation::interval() const
{
    return m_interval;
}

qreal EllipsePathAnimation::startAngle() const
{
    return m_startAngle;
}

qreal EllipsePathAnimation::endAngle() const
{
    return m_endAngle;
}

void EllipsePathAnimation::setInterval(int interval)
{
    if (m_interval == interval)
    {
        return;
    }

    if (interval <= 0)
    {
        qCritical() << "Interval in" << path() << "cannot be negative";
        return;
    }

    m_interval = interval;
    emit intervalChanged(m_interval);
}

void EllipsePathAnimation::setStartAngle(qreal startAngle)
{
    if (qFuzzyCompare(m_startAngle, startAngle))
    {
        return;
    }

    m_startAngle = startAngle;
    emit startAngleChanged(m_startAngle);
}

void EllipsePathAnimation::setEndAngle(qreal endAngle)
{
    if (qFuzzyCompare(m_endAngle, endAngle))
    {
        return;
    }

    m_endAngle = endAngle;
    emit endAngleChanged(m_endAngle);
}

bool EllipsePathAnimation::valid() const
{
    return m_interval > 0;
}

void EllipsePathAnimation::update(qreal ms)
{
    m_angle += ((ms / interval()) * (implicitEndAngle() - implicitStartAngle()));

    auto const realAngle = m_angle * FULL_CIRCLE;
    object()->setPosition(convertCoordinate({path()->width() * std::cos(realAngle), path()->height() * std::sin(realAngle)}));

    if (implicitStartAngle() < implicitEndAngle())
    {
        if (m_angle >= implicitEndAngle())
        {
            finishStep();
        }
    }
    else
    {
        if (m_angle <= implicitEndAngle())
        {
            finishStep();
        }
    }
}

void EllipsePathAnimation::resetStep()
{
    m_angle = implicitStartAngle();
}

qreal EllipsePathAnimation::implicitStartAngle() const
{
    return direct() == Forward ? startAngle() : endAngle();
}

qreal EllipsePathAnimation::implicitEndAngle() const
{
    return direct() == Forward ? endAngle() : startAngle();
}
}
