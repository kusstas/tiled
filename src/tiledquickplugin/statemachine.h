#pragma once

#include <QSharedPointer>
#include <QTimer>
#include <QObject>
#include <QMap>


class QPainter;

namespace Tiled
{
class Cell;
class Tile;
}

namespace TiledQuick
{
class MapObjectItem;

struct Transition
{
    static QString const ANY_STATE;

    Transition(QString const& from, QString const& to, Tiled::Tile* tile) : from(from), to(to), tile(tile) { }

    int concretizedScore() const
    {
        return static_cast<int>(from != ANY_STATE) + static_cast<int>(to != ANY_STATE);
    }

    QString from;
    QString to;
    Tiled::Tile* tile;
};

struct State
{
    State(QString const& name, Tiled::Tile* tile = nullptr) : name(name), tile(tile) { }

    QString name;
    Tiled::Tile* tile;
};

struct StateNode
{
    QMap<QString, QSharedPointer<Transition>> transitions;
};

class StateMachine : public QObject
{
    Q_OBJECT

public:
    explicit StateMachine(MapObjectItem* parent);

    MapObjectItem* mapObjectItem() const;
    Tiled::Cell const* cell() const;

    void paint(QPainter* painter);

    Q_INVOKABLE void setState(QString const& name, QString const& tilesetName, int tileId);
    Q_INVOKABLE void setTransition(QString const& from, QString const& to, QString const& tilesetName, int tileId);
    Q_INVOKABLE void setInvisibleState(QString const& name);

private:
    Tiled::Tile* findTile(QString const& tilesetName, int tileId);
    QSharedPointer<Transition> getTransition(QString const& from, QString const& to);

    void onStateChanged();
    void applyState();
    void startTransition();
    void handleFrame();
    void timeout();
    void animationFinished();

private:
    QTimer m_timer;
    QMap<QString, QSharedPointer<State>> m_states;
    QMap<QString, StateNode> m_transitionsGraph;
    QSharedPointer<State> m_currentState;
    QSharedPointer<Transition> m_currentTransition;
    Tiled::Tile* m_currentTile;
    Tiled::Tile* m_currentFrame;
    int m_currentFrameIndex;
    bool m_infinite;
};
}
