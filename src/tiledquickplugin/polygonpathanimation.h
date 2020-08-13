#pragma once

#include "pathanimation.h"

#include <QLineF>


namespace TiledQuick
{
class PolygonPathAnimation : public PathAnimation
{
    Q_OBJECT

    Q_PROPERTY(QList<int> intervals READ intervals WRITE setIntervals NOTIFY intervalsChanged)
    Q_PROPERTY(int line READ line NOTIFY lineChanged)
    Q_PROPERTY(qreal lineProgress READ lineProgress NOTIFY lineProgressChanged)

public:
    PolygonPathAnimation(TiledItem* object, MapObjectItem* path);

    QList<int> const& intervals() const;
    int line() const;
    qreal lineProgress() const;

public slots:
    void setIntervals(QList<int> const& intervals);

signals:
    void intervalsChanged(QList<int> const intervals);
    void lineChanged(int line);
    void lineProgressChanged(qreal lineProgress);

protected:
    bool valid() const override;
    void update(qreal ms) override;
    void resetStep() override;

private:
    void setLine(int line);
    void setLineProgress(qreal lineProgress);

private:
    QList<int> m_intervals;
    QList<QLineF> m_lines;
    int m_line;
    qreal m_lineProgress;
};
}
