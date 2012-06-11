/* ============================================================
 *
 * Date        : 2009-07-03
 * Description : a generator for gimp brushes
 *
 * Copyright (C) 2009-2012 by Andi Clemens <andi dot clemens at gmx dot net>
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

#include "gbrbrushloader.h"

// Qt includes

#include <QDataStream>
#include <QFile>
#include <QString>

// KDE includes

#include <kdebug.h>

// const variables
namespace
{
static const int HEADERSIZEv1 = 20;
static const int HEADERSIZEv2 = 28;
}

QImage GbrBrushLoader::generateThumbnail(QFile& file)
{
    QImage thumb;

    quint32 header      = 0;
    quint32 version     = 0;
    quint32 w           = 0;
    quint32 h           = 0;
    quint32 colorDepth  = 0;

    QDataStream in(&file);
    in >> header
       >> version
       >> w
       >> h
       >> colorDepth;

    // check if the brush has the right version and magic number
    bool validBrushFile = checkHeaderInformation(in, version);

    if (colorDepth != 1 && colorDepth != 4)
    {
        kDebug() << "Invalid color depth!";
        validBrushFile = false;
    }

    if (!validBrushFile)
    {
        file.close();
        return thumb;
    }

    // get the brush name size, skip the bytes
    unsigned int nameLength = header - ((version == 1) ? HEADERSIZEv1 : HEADERSIZEv2);
    in.skipRawData(nameLength);

    // read the image data
    int dataLength = w * h * colorDepth;
    QScopedArrayPointer<char> data(new char[dataLength]);
    int bytesRead = in.readRawData(data.data(), dataLength);
    file.close();

    // valid brush data?
    if (bytesRead == -1 || bytesRead != dataLength)
    {
        kDebug() << "Error while reading brush data ("
                 <<     "expected: " << dataLength << " bytes, "
                 <<     "read: "     << bytesRead  << " bytes"
                 << ")";
        return thumb;
    }

    // generate thumbnail
    QImage::Format imageFormat = (colorDepth == 1) ? QImage::Format_RGB32 : QImage::Format_ARGB32;

    thumb = QImage(w, h, imageFormat);
    quint32 step = 0;

    switch (colorDepth)
    {
        case 1: // Grayscale
        {
            for (quint32 y = 0; y < h; ++y)
            {
                for (quint32 x = 0; x < w; ++x, ++step)
                {
                    qint32 val = 255 - static_cast<uchar>(data[step]);
                    thumb.setPixel(x, y, qRgb(val, val, val));
                }
            }

            break;
        }

        case 4: // RGBA
        {
            for (quint32 y = 0; y < h; ++y)
            {
                for (quint32 x = 0; x < w; ++x, step += 4)
                {
                    thumb.setPixel(x, y, qRgba(static_cast<uchar>(data[step]),
                                               static_cast<uchar>(data[step + 1]),
                                               static_cast<uchar>(data[step + 2]),
                                               static_cast<uchar>(data[step + 3])));
                }
            }

            break;
        }
    }

    return thumb;
}

bool GbrBrushLoader::checkHeaderInformation(QDataStream& ds, quint32 version)
{
    bool isValid = true;

    switch (version)
    {
        case 1:
        {
            // no magic number and spacing information
            break;
        }

        case 2:
        case 3:
        {
            quint32 magic   = 0;
            quint32 spacing = 0;
            ds  >> magic
                >> spacing;

            if ((magic != 0x47494D50))
            {
                kDebug() << "No valid Gimp Brush file!";
                isValid = false;
            }

            break;
        }

        default:
        {
            isValid = false;
            break;
        }
    }

    return isValid;
}

