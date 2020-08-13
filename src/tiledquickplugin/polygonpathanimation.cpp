#include "polygonpathanimation.h"
#include "mapobjectitem.h"


namespace TiledQuick
{
PolygonPathAnimation::PolygonPathAnimation(TiledItem* object, MapObjectItem* path)
    : PathAnimation(object, path)
    , m_line(0)
    , m_lineProgress(0)
{
    QPolygonF polygon = path->mapObject()->shape() == Tiled::MapObject::Rectangle
                        ? QPolygonF(QRectF(0, 0, path->width(), path->height()))
                        : path->mapObject()->polygon();

    if (path->mapObject()->shape() == Tiled::MapObject::Polygon)
    {
        polygon << polygon.first();
    }

    auto const linesCount = polygon.size() - 1;

    m_lines.reserve(linesCount);
    for (int l = 0; l < linesCount; ++l)
    {
        m_lines << QLineF(polygon[l], polygon[l + 1]);
    }
}

QList<int> const& PolygonPathAnimation::intervals() const
{
    return m_intervals;
}

int PolygonPathAnimation::line() const
{
    return m_line;
}

qreal PolygonPathAnimation::lineProgress() const
{
    return m_lineProgress;
}

void PolygonPathAnimation::setIntervals(QList<int> const& intervals)
{
    if (m_intervals == intervals)
    {
        return;
    }

    for (auto const& interval : intervals)
    {
        if (interval < 0)
        {
            qCritical() << "Intervals in" << path() << "cannot be negative";
            return;
        }
    }

    if (intervals.size() == 1)
    {
        m_intervals.clear();
        m_intervals.reserve(m_lines.size());

        qreal perimeter = 0;

        for (auto const& l : m_lines)
        {
            perimeter += l.length();
        }

        for (auto const& l : m_lines)
        {
            m_intervals << static_cast<int>(intervals.first() * (l.length() / perimeter));
        }
    }
    else if (m_lines.size() == intervals.size())
    {
        m_intervals = intervals;
    }
    else
    {
        qCritical() << "Count of intervals in PolygonPathAnimation should equal count lines or one for whole path!" << path();
        return;
    }

    emit intervalsChanged(m_intervals);
}

bool PolygonPathAnimation::valid() const
{
    return m_lines.size() == intervals().size();
}

void PolygonPathAnimation::update(qreal ms)
{
    bool finish = false;
    int nextLine = line();
    qreal diff = direct() * ms / m_intervals[line()];
    qreal progress = qMin(1.0, qMax(0.0, lineProgress() + diff));

    object()->setPosition(convertCoordinate(m_lines[line()].pointAt(progress)));

    switch (direct())
    {
    case Backward:
        if (progress <= 0)
        {
            progress = 1;
            --nextLine;
        }
        finish = nextLine < 0;
        break;
    case Forward:
        if (progress >= 1)
        {
            progress = 0;
            ++nextLine;
        }
        finish = nextLine >= m_lines.size();
        break;
    default:
        break;
    }

    if (finish)
    {
        finishStep();
    }
    else
    {
        setLine(nextLine);
        setLineProgress(progress);
    }
}

void PolygonPathAnimation::resetStep()
{
    switch (direct())
    {
    case TiledQuick::PathAnimation::Backward:
        setLine(m_lines.size() - 1);
        setLineProgress(1);
        break;
    case TiledQuick::PathAnimation::Forward:
        setLine(0);
        setLineProgress(0);
        break;
    default:
        break;
    }
}

void PolygonPathAnimation::setLine(int line)
{
    if (m_line == line)
    {
        return;
    }

    m_line = line;
    emit lineChanged(m_line);
}

void PolygonPathAnimation::setLineProgress(qreal lineProgress)
{
    m_lineProgress = lineProgress;
    emit lineProgressChanged(m_lineProgress);
}
}
