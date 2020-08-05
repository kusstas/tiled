#pragma once

#include "mapref.h"

#include <QObject>
#include <QUrl>


namespace TiledQuick
{
class MapLoader : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(TiledQuick::MapRef map READ map NOTIFY sourceChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString error READ error NOTIFY errorChanged)

public:
    enum Status
    {
        Null,
        Ready,
        Error
    };
    Q_ENUM(Status)

    explicit MapLoader(QObject* parent = nullptr);

    QUrl const& source() const;
    MapRef const& map() const;
    QString const& error() const;
    Status status() const;

public slots:
    void setSource(QUrl const& source);

signals:
    void sourceChanged(QUrl const& source);
    void mapChanged(MapRef const& map);
    void statusChanged(Status status);
    void errorChanged(QString const& error);

private:
    QUrl m_source;
    MapRef m_map;
    Status m_status;
    QString m_error;
};
} // namespace TiledQuick
