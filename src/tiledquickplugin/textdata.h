#pragma once

#include <QObject>

#include "mapobject.h"


namespace TiledQuick
{
class TextData : public QObject, public Tiled::TextData
{
    Q_OBJECT

    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY fontChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment NOTIFY alignmentChanged)
    Q_PROPERTY(bool wordWrap READ wordWrap WRITE setWordWrap NOTIFY wordWrapChanged)

public:
    TextData(Tiled::TextData const& textData, QObject* parent = nullptr);

    QString const& text() const;
    QFont const& font() const;
    QColor const& color() const;
    Qt::Alignment alignment() const;
    bool wordWrap() const;

public slots:
    void setText(QString const& text);
    void setFont(QFont const& font);
    void setColor(QColor const& color);
    void setAlignment(Qt::Alignment alignment);
    void setWordWrap(bool wordWrap);

signals:
    void textChanged(QString const& text);
    void fontChanged(QFont const& font);
    void colorChanged(QColor const& color);
    void alignmentChanged(Qt::Alignment alignment);
    void wordWrapChanged(bool wordWrap);
    void changed();
};
}
