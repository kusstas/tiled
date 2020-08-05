#include "pathanimation.h"
#include "mapobjectitem.h"


namespace TiledQuick
{
int const PathAnimation::UPDATE_INTERVAL = 1000 / 60;
constexpr qreal NS_IN_MS = 1000000.0;

PathAnimation::PathAnimation(TiledItem* object, MapObjectItem* path)
    : QObject(object)
    , m_object(object)
    , m_path(path)
    , m_center(0, 0)
    , m_loops(Once)
    , m_backward(false)
    , m_running(false)
    , m_passedLoops(0)
    , m_direct(Forward)
{
    m_timer.setSingleShot(false);
    m_timer.setInterval(UPDATE_INTERVAL);

    connect(&m_timer, &QTimer::timeout, this, &PathAnimation::timeout);
    connect(this, &PathAnimation::finished, this, &QObject::deleteLater);
}

TiledItem* PathAnimation::object() const
{
    return m_object;
}

MapObjectItem* PathAnimation::path() const
{
    return m_path;
}

QPointF const& PathAnimation::center() const
{
    return m_center;
}

int PathAnimation::loops() const
{
    return m_loops;
}

bool PathAnimation::backward() const
{
    return m_backward;
}

bool PathAnimation::running() const
{
    return m_running;
}

void PathAnimation::start()
{
    reset();
    setRunning(true);

    emit started();
}

void PathAnimation::stop()
{
    setRunning(false);
    reset();

    emit finished();
}

void PathAnimation::reset()
{
    m_elapsedTimer.restart();
    m_direct = Forward;
    m_passedLoops = 0;

    resetStep();
}

void PathAnimation::setCenter(QPointF const& center)
{
    if (m_center == center)
    {
        return;
    }

    m_center = center;
    emit centerChanged(m_center);
}

void PathAnimation::setLoops(int loops)
{
    if (m_loops == loops)
    {
        return;
    }

    m_loops = loops;
    emit loopsChanged(m_loops);
}

void PathAnimation::setBackward(bool backward)
{
    if (m_backward == backward)
    {
        return;
    }

    m_backward = backward;
    emit backwardChanged(m_backward);
}

void PathAnimation::setRunning(bool running)
{
    if (m_running == running)
    {
        return;
    }

    if (running)
    {
        if (valid())
        {
            m_timer.start();
        }
        else
        {
            qCritical() << "Can't start path animation for object:" << object();
        }
    }
    else
    {
        m_timer.stop();
    }

    m_running = running;
    emit runningChanged(m_running);
}

bool PathAnimation::valid() const
{
    return true;
}

void PathAnimation::resetStep()
{
}

PathAnimation::Direct PathAnimation::direct() const
{
    return m_direct;
}

void PathAnimation::finishStep()
{
    if (backward())
    {
        m_direct = m_direct > Center ? Backward : Forward;
    }

    resetStep();

    if (direct() == Forward)
    {
        finishLoop();
    }
}

QPointF PathAnimation::convertCoordinate(QPointF const& point) const
{
    return object()->parentItem()->mapFromItem(path(), point) - center();
}

void PathAnimation::timeout()
{
    update(m_elapsedTimer.nsecsElapsed() / NS_IN_MS);
    m_elapsedTimer.restart();
}

void PathAnimation::finishLoop()
{
    ++m_passedLoops;

    if (m_loops > Infinite && m_loops >= m_passedLoops)
    {
        stop();
    }
}
}
