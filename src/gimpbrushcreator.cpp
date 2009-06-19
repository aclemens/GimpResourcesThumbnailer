/* ============================================================
 *
 * Date        : 2009-06-17
 * Description : a thumb creator for gimp brushes
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

#include "gimpbrushcreator.h"

// Qt includes

#include <QImage>
#include <QString>
#include <QFile>

// KDE includes

#include <kdebug.h>

extern "C"
{
    KDE_EXPORT ThumbCreator *new_creator()
    {
        return new GimpBrushCreator;
    }
}

GimpBrushCreator::GimpBrushCreator()
{
}

bool GimpBrushCreator::create(const QString &path, int width, int height, QImage &img)
{
    Q_UNUSED(width)
    Q_UNUSED(height)

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        kDebug() << "Error loading Gimp Brush (GBR) file.";
        return false;
    }

    quint32 headerSize;
    quint32 version;
    quint32 w;
    quint32 h;
    quint32 colorDepth;
    quint32 magic;
    quint32 spacing;
    QString brushName;

    QDataStream in(&file);
    in >> headerSize
       >> version
       >> w
       >> h
       >> colorDepth
       >> magic
       >> spacing;

    if ((magic != 0x47494D50))
    {
        kDebug() << "This is no valid Gimp Brush (GBR) file!";
        file.close();
        return false;
    }

    if (version != 2 && version != 3)
    {
        kDebug() << "Unknown Gimp Brush (GBR) version!";
        file.close();
        return false;
    }

    if (colorDepth != 1 && colorDepth != 4)
    {
        kDebug() << "Invalid color depth!";
        file.close();
        return false;
    }

    // read the brush name
    unsigned int nameLength = headerSize - 28;
    char* brushName_c       = new char[nameLength];
    in.readRawData(brushName_c, nameLength);
    brushName = QString(brushName_c);

    // read the image data
    unsigned int dataLength = w * h * colorDepth;
    char* data              = new char[dataLength];
    in.readRawData(data, dataLength);

    // close file
    file.close();

    // generate thumbnail
    bool success = true;

    QImage::Format imageFormat;
    imageFormat = (colorDepth == 1) ? QImage::Format_RGB32 : QImage::Format_ARGB32;

    QImage thumbnail(w, h, imageFormat);
    quint32 step = 0;

    if (colorDepth == 1 && data)
    {
        // Grayscale
        for (quint32 y = 0; y < h; ++y)
        {
            for (quint32 x = 0; x < w; ++x, ++step)
            {
                qint32 val = 255 - static_cast<uchar> (data[step]);
                thumbnail.setPixel(x, y, qRgb(val, val, val));
            }
        }
    }
    else if (colorDepth == 4 && data)
    {
        // RGBA
        for (quint32 y = 0; y < h; ++y)
        {
            for (quint32 x = 0; x < w; ++x, step += 4)
            {
                thumbnail.setPixel(x, y, qRgba(static_cast<uchar>(data[step]),
                                               static_cast<uchar>(data[step+1]),
                                               static_cast<uchar>(data[step+2]),
                                               static_cast<uchar>(data[step+3])));
            }
        }
    }
    else
    {
        success = false;
    }

    if (success)
    {
        // load image data into reference
        img = thumbnail;
        kDebug() << "Thumbnail for Gimp Brush (GBR) '" << brushName << "' successfully generated!";
    }
    else
    {
        kDebug() << "Failed generating Gimp Brush (GBR) thumbnail...";
        success = false;
    }

    // cleanup
    delete[] brushName_c;
    delete[] data;

    return success;
}

ThumbCreator::Flags GimpBrushCreator::flags() const
{
    return None;
}
