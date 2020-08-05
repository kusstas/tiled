#pragma once

#include "pathanimation.h"

#include <QLineF>


namespace TiledQuick
{
class PolygonPathAnimation : public PathAnimation
{
    Q_OBJECT

    Q_PROPERTY(QList<int> intervals READ intervals WRITE setIntervals NOTIFY intervalsChanged)

public:
    PolygonPathAnimation(TiledItem* object, MapObjectItem* path);

    QList<int> const& intervals() const;

public slots:
    void setIntervals(QList<int> const& intervals);

signals:
    void intervalsChanged(QList<int> const intervals);

protected:
    bool valid() const override;
    void update(qreal ms) override;
    void resetStep() override;

private:
    QList<int> m_intervals;
    QList<QLineF> m_lines;
    int m_line;
    qreal m_progress;
};
}
