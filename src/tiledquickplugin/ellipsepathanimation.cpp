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

qreal EllipsePathAnimation::angle() const
{
    return m_angle;
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
    bool finish = false;
    auto newAngle = angle() + ((ms / interval()) * (implicitEndAngle() - implicitStartAngle()));

    auto const realAngle = newAngle * FULL_CIRCLE;
    object()->setPosition(convertCoordinate({path()->width() * std::cos(realAngle), path()->height() * std::sin(realAngle)}));

    if (implicitStartAngle() < implicitEndAngle())
    {
        finish = newAngle >= implicitEndAngle();
    }
    else
    {
        finish = newAngle <= implicitEndAngle();
    }

    setAngle(newAngle);
    if (finish)
    {
        finishStep();
    }
}

void EllipsePathAnimation::resetStep()
{
    setAngle(implicitStartAngle());
}

qreal EllipsePathAnimation::implicitStartAngle() const
{
    return direct() == Forward ? startAngle() : endAngle();
}

qreal EllipsePathAnimation::implicitEndAngle() const
{
    return direct() == Forward ? endAngle() : startAngle();
}

void EllipsePathAnimation::setAngle(qreal angle)
{
    m_angle = angle;
    emit angleChanged(m_angle);
}
}
