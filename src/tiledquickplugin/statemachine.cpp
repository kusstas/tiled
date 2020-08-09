#include "objectlayeritem.h"
#include "mapobjectitem.h"
#include "statemachine.h"
#include "drawutils.h"
#include "map.h"

#include <QPainter>
#include <QTransform>


namespace TiledQuick
{
QString const Transition::ANY_STATE = "*";

StateMachine::StateMachine(MapObjectItem* parent)
    : QObject(parent)
{
    m_states[QString()] = QSharedPointer<State>(new State(QString(), parent->mapObject()->cell().tile()));

    m_timer.setSingleShot(true);
    connect(&m_timer, &QTimer::timeout, this, &StateMachine::timeout);

    onStateChanged();
    connect(parent, &MapObjectItem::stateChanged, this, &StateMachine::onStateChanged);
}

MapObjectItem* StateMachine::mapObjectItem() const
{
    return static_cast<MapObjectItem*>(parent());
}

Tiled::Cell const* StateMachine::cell() const
{
    return &mapObjectItem()->mapObject()->cell();
}

void StateMachine::paint(QPainter* painter)
{
    QPixmap pixmap;

    if (m_currentFrame)
    {
        pixmap = m_currentFrame->image();
    }

    if (pixmap.isNull())
    {
        return;
    }

    int scaleX = cell()->flippedHorizontally() ? -1 : 1;
    int scaleY = cell()->flippedVertically() ? -1 : 1;

    if (scaleX < 0 || scaleY < 0)
    {
        QTransform transform;
        transform.scale(scaleX, scaleY);

        pixmap = pixmap.transformed(transform);
    }

    QRect const target(0, 0, mapObjectItem()->width(), mapObjectItem()->height());
    QRect const source(0, 0, pixmap.width(), pixmap.height());

    DrawUtils::paintPixmap(painter, target, source, pixmap, mapObjectItem()->objectLayer()->layer()->tintColor());
}

void StateMachine::setState(QString const& name, QString const& tilesetName, int tileId)
{   
    auto const tile = findTile(tilesetName, tileId);

    if (!tile)
    {
        qCritical() << "Can't set state:" << name << " - can't find tile!";
        return;
    }

    m_states[name] = QSharedPointer<State>(new State(name, tile));
    if (name == mapObjectItem()->state())
    {
        onStateChanged();
    }
}

void StateMachine::setTransition(QString const& from, QString const& to, QString const& tilesetName, int tileId)
{
    auto const tile = findTile(tilesetName, tileId);

    if (!tile)
    {
        qCritical() << "Can't set transition:" << from << "->" << to << " - can't find tile!";
        return;
    }

    QSharedPointer<Transition> transition(new Transition(from, to, tile));
    m_transitionsGraph[from].transitions[to] = transition;
}

void StateMachine::setInvisibleState(QString const& name)
{
    m_states[name] = QSharedPointer<State>(new State(name));
}

Tiled::Tile* StateMachine::findTile(QString const& tilesetName, int tileId)
{
    if (tileId < 0)
    {
        qCritical() << "Can't find tile: - tile id:" << tileId << "should be positive!";
        return nullptr;
    }

    auto const& tilesets = mapObjectItem()->map()->tilesets();

    auto const tilesetIt = std::find_if(tilesets.cbegin(),
                                        tilesets.cend(),
                                        [&tilesetName] (Tiled::SharedTileset const& tileset)
    {
            return tileset->name() == tilesetName;
    });

    if (tilesetIt == tilesets.cend())
    {
        qCritical() << "Can't find tile: - tileset:" << tilesetName << "not found!";
        return nullptr;
    }

    auto const tileset = *tilesetIt;

    return tileset->tileAt(tileId);
}

QSharedPointer<Transition> StateMachine::getTransition(QString const& from, QString const& to)
{
    QList<QSharedPointer<Transition>> candidates;

    auto const addIfValid = [&candidates] (QSharedPointer<Transition> const& transition)
    {
        if (transition)
        {
            candidates.append(transition);
        }
    };

    if (m_transitionsGraph.contains(from))
    {
        auto const& node = &m_transitionsGraph[from];

        addIfValid(node->transitions.value(to, nullptr));
        addIfValid(node->transitions.value(Transition::ANY_STATE, nullptr));
    }
    if (m_transitionsGraph.contains(Transition::ANY_STATE))
    {
        auto const& node = &m_transitionsGraph[Transition::ANY_STATE];

        addIfValid(node->transitions.value(to, nullptr));
        addIfValid(node->transitions.value(Transition::ANY_STATE, nullptr));
    }

    std::sort(candidates.begin(),
              candidates.end(),
              [] (QSharedPointer<Transition> const& l, QSharedPointer<Transition> const& r)
    {
        return l->concretizedScore() < r->concretizedScore();
    });

    return candidates.isEmpty() ? nullptr : candidates.first();
}

void StateMachine::onStateChanged()
{
    m_currentTransition = getTransition(m_currentState ? m_currentState->name : QString(), mapObjectItem()->state());
    m_currentState = m_states.value(mapObjectItem()->state(), m_states[QString()]);

    if (m_currentTransition)
    {
        startTransition();
    }
    else
    {
        applyState();
    }
}

void StateMachine::applyState()
{
    m_timer.stop();
    m_infinite = true;
    m_currentFrameIndex = 0;
    m_currentTile = m_currentState ? m_currentState->tile : nullptr;

    if (m_currentTile)
    {
        handleFrame();
    }
    else
    {
        mapObjectItem()->update();
    }
}

void StateMachine::startTransition()
{
    m_timer.stop();
    m_infinite = false;
    m_currentFrameIndex = 0;
    m_currentTile = m_currentTransition ? m_currentTransition->tile : nullptr;

    if (m_currentTile)
    {
        handleFrame();
    }
    else
    {
        applyState();
    }
}

void StateMachine::handleFrame()
{
    if (m_currentTile->isAnimated())
    {
        auto const& frames = m_currentTile->frames();

        if (m_currentFrameIndex >= frames.size())
        {
            if (m_infinite)
            {
                m_currentFrameIndex = 0;
            }
            else
            {
                animationFinished();
                return;
            }
        }

        auto const& frame = frames[m_currentFrameIndex++];

        m_currentFrame = m_currentTile->tileset()->tileAt(frame.tileId);
        mapObjectItem()->update();
        m_timer.start(frame.duration);
    }
    else
    {
        m_currentFrame = m_currentState->tile;
        mapObjectItem()->update();
    }
}

void StateMachine::timeout()
{
    handleFrame();
}

void StateMachine::animationFinished()
{
    applyState();
}
}
