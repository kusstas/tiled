#pragma once

#include <QQuickItem>

#include "maprenderer.h"
#include "mapitem.h"
#include "mapref.h"


namespace TiledQuick
{
class MapViewport : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(TiledQuick::MapRef map READ map WRITE setMap NOTIFY mapChanged)
    Q_PROPERTY(QObject* external READ external WRITE setExternal NOTIFY externalChanged)

public:
    MapViewport(QQuickItem* parent = nullptr);

    MapRef const& map() const;
    QObject* external() const;

public slots:
    void setMap(MapRef const& map);
    void setExternal(QObject* external);

signals:
    void mapChanged(MapRef const& map);
    void externalChanged(QObject* external);

protected:
    void componentComplete() override;

private:
    void refresh();
    void updateScale();

private:
    MapRef m_map;
    QScopedPointer<MapItem> m_mapItem;
    QSharedPointer<Tiled::MapRenderer> m_renderer;
    QObject* m_external;
};
}

