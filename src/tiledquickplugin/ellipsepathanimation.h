#pragma once

#include "pathanimation.h"


namespace TiledQuick
{
class EllipsePathAnimation : public PathAnimation
{
    Q_OBJECT

    Q_PROPERTY(int interval READ interval WRITE setInterval NOTIFY intervalChanged)
    Q_PROPERTY(qreal startAngle READ startAngle WRITE setStartAngle NOTIFY startAngleChanged)
    Q_PROPERTY(qreal endAngle READ endAngle WRITE setEndAngle NOTIFY endAngleChanged)

public:
    EllipsePathAnimation(TiledItem* object, MapObjectItem* path);

    int interval() const;
    qreal startAngle() const;
    qreal endAngle() const;

public slots:
    void setInterval(int interval);
    void setStartAngle(qreal startAngle);
    void setEndAngle(qreal endAngle);

signals:
    void intervalChanged(int interval);
    void startAngleChanged(qreal startAngle);
    void endAngleChanged(qreal endAngle);

protected:
    bool valid() const override;
    void update(qreal ms) override;
    void resetStep() override;

private:
    bool validInterval() const;
    qreal implicitStartAngle() const;
    qreal implicitEndAngle() const;

private:
    int m_interval;
    qreal m_startAngle;
    qreal m_endAngle;
    qreal m_angle;
};
}
