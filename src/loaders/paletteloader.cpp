/* ============================================================
 *
 * Date        : 2010-01-02
 * Description : a palette thumbnail generator
 *
 * Copyright (C) 2009 by Andi Clemens <andi dot clemens at gmx dot net>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * ============================================================ */

#include "paletteloader.h"

// Qt includes

#include <QFile>
#include <QPainter>
#include <QPixmap>
#include <QTextStream>
#include <QRegExp>

// KDE includes

#include <kdebug.h>

/**
 * the tile size
 */
const int TILE_SIZE = 16;

/**
 * maximum number of colors per row
 */
const int MAX_COLORS_IN_ROW = 16;

/**
 * The number of parameters to describe a palette
 */
const int PALETTE_PARAMS = 3;


bool PaletteLoader::generateThumbnail(QFile& file)
{
    /*
     GRADIENT FILE SPECIFICATION:

     Line 1 (string)   : Resource type (must be "GIMP Palette")
     Line 2 (string)   : The name of the palette ("Name: <THENAME>")
     Line 3 (string)   : Number of columns [optional], it can be omitted, we will always render TILE_SIZE columns

     Delimiter line    : Starts with "#", all the following lines contain the palette data
     */

    QTextStream in(&file);
    QStringList data;

    while (!in.atEnd())
    {
        data << in.readLine();
    }

    // close the file
    file.close();

    if (validData(data))
    {
        prepareData(data);
        QImage img = drawPalette(data);

        if (!img.isNull())
        {
            m_thumbnail = img;
            m_success   = true;
            kDebug() << "OK!";
            return true;
        }
    }
    return false;
}

void PaletteLoader::prepareData(QStringList& data)
{
    QRegExp dataReg("^\\d+\\s+\\d+\\s+\\d+\\s*.*");
    QStringList list;

    foreach (const QString& line, data)
    {
        QString dataLine = line.trimmed();
        if (dataReg.exactMatch(dataLine))
        {
            list.append(dataLine);
        }
    }

    data.clear();
    data.append(list);
}

bool PaletteLoader::validData(const QStringList& data)
{
    if (data.isEmpty())
    {
        return false;
    }

    bool validData        = true;
    int dataSize          = data.count();
    QString magic         = data[0];

    // check basic parameters
    if (magic != QString("GIMP Palette") || dataSize < 1)
    {
        kDebug() << "Invalid basic parameters";
        validData = false;
    }

    if (!validData)
    {
        kDebug() << "Invalid Gimp Palette (GPL) data!";
    }
    return validData;
}

PaletteData PaletteLoader::getPaletteInformation(const QString& palette)
{
    QRegExp sep("\\s+");
    QStringList values = palette.split(sep);
    bool ok            = true;
    bool allOk         = true;

    PaletteData data;
    if (values.count() < PALETTE_PARAMS)
    {
        data.status = PaletteData::Invalid;
        return data;
    }

    data.red   = values[0].toInt(&ok);  allOk = allOk && ok;
    data.green = values[1].toInt(&ok);  allOk = allOk && ok;
    data.blue  = values[2].toInt(&ok);  allOk = allOk && ok;

    // check if data was converted correctly, set status accordingly
    data.status = allOk ? PaletteData::Ok : PaletteData::Invalid;

    return data;
}

bool PaletteLoader::checkPaletteInformation(const QString& palette)
{
    PaletteData data = getPaletteInformation(palette);

    return (data.status == PaletteData::Ok);
}

QImage PaletteLoader::drawPalette(const QStringList& data)
{
    if (data.isEmpty())
    {
        return QImage();
    }

    int numberOfColors = data.count();

    int w     = (TILE_SIZE * MAX_COLORS_IN_ROW);
    int min_h = w / 2;
    int max_h = ((numberOfColors / MAX_COLORS_IN_ROW) * TILE_SIZE);
    int h     = (max_h > min_h) ? max_h : min_h;

    // avoid cutting off the border on the right side of the thumbnail
    const int BORDER_WIDTH = 1;

    QPixmap pix(w + BORDER_WIDTH, h + BORDER_WIDTH);
    pix.fill(Qt::white);

    QPainter p(&pix);
    QPen pen(Qt::black);
    pen.setWidth(BORDER_WIDTH);
    p.setPen(pen);
    p.setRenderHint(QPainter::Antialiasing, false);

    PaletteList paletteList = extractPaletteColors(data);

    int column = 0;
    int row    = 0;
    foreach (const PaletteData& palette, paletteList)
    {
        if (palette.status != PaletteData::Ok)
        {
            continue;
        }

        QRect r(column, row, TILE_SIZE, TILE_SIZE);
        QColor color = QColor::fromRgb(palette.red, palette.green, palette.blue);
        p.fillRect(r, color);
        p.drawRect(r);

        column += TILE_SIZE;
        if (column >= w)
        {
            row += TILE_SIZE;
            column = 0;
        }
    }

    p.end();

    return pix.toImage();
}

void PaletteLoader::debugPaletteData(const PaletteData& data)
{
    kDebug() << "Status: " << data.status;
    kDebug() << "red: "    << data.red;
    kDebug() << "green: "  << data.green;
    kDebug() << "blue: "   << data.blue;
}

PaletteList PaletteLoader::extractPaletteColors(const QStringList& data)
{
    PaletteList list;

    foreach (const QString& info, data)
    {
        if (checkPaletteInformation(info))
        {
            list.append(getPaletteInformation(info));
        }
    }

    return list;
}
