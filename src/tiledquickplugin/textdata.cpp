#include "textdata.h"


namespace TiledQuick
{
TextData::TextData(Tiled::TextData const& textData, QObject* parent)
    : QObject(parent)
    , Tiled::TextData(textData)
{
    connect(this, &TextData::textChanged, this, &TextData::changed);
    connect(this, &TextData::fontChanged, this, &TextData::changed);
    connect(this, &TextData::colorChanged, this, &TextData::changed);
    connect(this, &TextData::alignmentChanged, this, &TextData::changed);
    connect(this, &TextData::wordWrapChanged, this, &TextData::changed);
}

QString const& TextData::text() const
{
    return Tiled::TextData::text;
}

QFont const& TextData::font() const
{
    return Tiled::TextData::font;
}

QColor const& TextData::color() const
{
    return Tiled::TextData::color;
}

Qt::Alignment TextData::alignment() const
{
    return Tiled::TextData::alignment;
}

bool TextData::wordWrap() const
{
    return Tiled::TextData::wordWrap;
}

void TextData::setText(QString const& text)
{
    if (Tiled::TextData::text == text)
    {
        return;
    }

    Tiled::TextData::text = text;
    emit textChanged(Tiled::TextData::text);
}

void TextData::setFont(QFont const& font)
{
    if (Tiled::TextData::font == font)
    {
        return;
    }

    Tiled::TextData::font = font;
    emit fontChanged(Tiled::TextData::font);
}

void TextData::setColor(QColor const& color)
{
    if (Tiled::TextData::color == color)
    {
        return;
    }

    Tiled::TextData::color = color;
    emit colorChanged(Tiled::TextData::color);
}

void TextData::setAlignment(Qt::Alignment alignment)
{
    if (Tiled::TextData::alignment == alignment)
    {
        return;
    }

    Tiled::TextData::alignment = alignment;
    emit alignmentChanged(Tiled::TextData::alignment);
}

void TextData::setWordWrap(bool wordWrap)
{
    if (Tiled::TextData::wordWrap == wordWrap)
    {
        return;
    }

    Tiled::TextData::wordWrap = wordWrap;
    emit wordWrapChanged(Tiled::TextData::wordWrap);
}
}
