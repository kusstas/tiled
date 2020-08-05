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

public:
    MapViewport(QQuickItem* parent = nullptr);

    MapRef const& map() const;

public slots:
    void setMap(MapRef const& map);

signals:
    void mapChanged(MapRef const& map);

protected:
    void componentComplete() override;

private:
    void refresh();
    void updateScale();

private:
    MapRef m_map;
    QScopedPointer<MapItem> m_mapItem;
    QSharedPointer<Tiled::MapRenderer> m_renderer;
};
}

