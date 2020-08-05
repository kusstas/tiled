#pragma once

#include "map.h"

#include <QSharedPointer>


namespace TiledQuick
{
class MapRef : public QSharedPointer<Tiled::Map>
{
    Q_GADGET

public:
    MapRef() = default;
};
}

Q_DECLARE_METATYPE(TiledQuick::MapRef)
