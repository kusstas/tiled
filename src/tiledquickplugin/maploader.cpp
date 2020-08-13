#include "maploader.h"

#include "map.h"
#include "mapreader.h"
#include "mapwriter.h"
#include "tiled.h"
#include "tileset.h"


namespace TiledQuick
{
MapLoader::MapLoader(QObject* parent)
    : QObject(parent)
    , m_status(Null)
{
}

QUrl const& MapLoader::source() const
{
    return m_source;
}

MapRef const& MapLoader::map() const
{
    return m_map;
}

QString const& MapLoader::error() const
{
    return m_error;
}

MapLoader::Status MapLoader::status() const
{
    return m_status;
}

void MapLoader::setSource(QUrl const& source)
{
    if (m_source == source)
    {
        return;
    }

    m_source = source;

    Tiled::MapReader mapReader;

    Tiled::Map* map = mapReader.readMap(Tiled::urlToLocalFileOrQrc(source)).release();
    Status status = map ? Ready : Error;
    QString error = map ? QString() : mapReader.errorString();

    bool const mapDiff = m_map.get() != map;
    bool const statusDiff = m_status != status;
    bool const errorDiff = m_error != error;

    m_map.reset(map);
    m_status = status;
    m_error = error;

    emit sourceChanged(source);

    if (mapDiff)
    {
        emit mapChanged(m_map);
    }
    if (statusDiff)
    {
        emit statusChanged(status);
    }
    if (errorDiff)
    {
        emit errorChanged(error);
    }
}
}
