#pragma once

#include <QObject>
#include <QTimer>
#include <QPointF>
#include <QElapsedTimer>


namespace TiledQuick
{
class TiledItem;
class MapObjectItem;

class PathAnimation : public QObject
{
    Q_OBJECT

    Q_PROPERTY(TiledQuick::TiledItem* object READ object CONSTANT)
    Q_PROPERTY(TiledQuick::MapObjectItem* path READ path CONSTANT)
    Q_PROPERTY(QPointF center READ center WRITE setCenter NOTIFY centerChanged)
    Q_PROPERTY(int loops READ loops WRITE setLoops NOTIFY loopsChanged)
    Q_PROPERTY(bool backward READ backward WRITE setBackward NOTIFY backwardChanged)
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(Direct direct READ direct NOTIFY directChanged)
    Q_PROPERTY(int passedLoops READ passedLoops NOTIFY passedLoopsChanged)

public:
    enum Loops
    {
        Infinite = 0,
        Once = 1
    };
    Q_ENUM(Loops)

    enum Direct
    {
        Backward = -1,
        Center = 0,
        Forward = 1
    };
    Q_ENUM(Direct)

    PathAnimation(TiledItem* object, MapObjectItem* path);

    TiledItem* object() const;
    MapObjectItem* path() const;
    QPointF const& center() const;
    int loops() const;
    bool backward() const;
    bool running() const;
    Direct direct() const;
    int passedLoops() const;

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void reset();

public slots:
    void setCenter(QPointF const& center);
    void setLoops(int loops);
    void setBackward(bool backward);
    void setRunning(bool running);

signals:
    void centerChanged(QPointF center);
    void loopsChanged(int loops);
    void backwardChanged(bool backward);
    void runningChanged(bool running);
    void directChanged(Direct direct);
    void passedLoopsChanged(int passedLoops);
    void started();
    void finished();
    void reached();

protected:
    virtual bool valid() const;
    virtual void update(qreal ms) = 0;
    virtual void resetStep();

    void finishStep();
    QPointF convertCoordinate(QPointF const& point) const;

private:
    void timeout();
    void finishLoop();
    void setDirect(Direct direct);
    void setPassedLoops(int passedLoops);

private:
    static int const UPDATE_INTERVAL;

    TiledItem* m_object;
    MapObjectItem* m_path;
    QPointF m_center;
    int m_loops;
    bool m_backward;
    bool m_running;
    Direct m_direct;
    int m_passedLoops;

    QTimer m_timer;
    QElapsedTimer m_elapsedTimer;
};
}
