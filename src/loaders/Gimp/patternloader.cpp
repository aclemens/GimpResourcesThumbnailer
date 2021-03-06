/* ============================================================
 *
 * Date        : 2009-07-03
 * Description : a loader for gimp patterns
 *
 * Copyright (C) 2009-2011 by Andi Clemens <andi dot clemens at gmail dot com>
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

#include "patternloader.h"

// Qt includes

#include <QDataStream>
#include <QFile>
#include <QString>

// KDE includes

#include <kdebug.h>

namespace
{
static const int HEADERSIZE = 24;
}

QImage PatternLoader::generateThumbnail(QFile& file)
{
    QImage thumb;

    quint32 header     = 0;
    quint32 version    = 0;
    quint32 w          = 0;
    quint32 h          = 0;
    quint32 colorDepth = 0;
    quint32 magic      = 0;

    QDataStream in(&file);
    in >> header
       >> version
       >> w
       >> h
       >> colorDepth
       >> magic;

    // check if the pattern has the right version and magic number
    bool validPatternFile = true;

    if (version != 1)
    {
        kDebug() << "Invalid version!";
        validPatternFile = false;
    }

    if (magic != 0x47504154)
    {
        kDebug() << "Invalid magic number!";
        validPatternFile = false;
    }

    if (colorDepth < 1 || colorDepth > 4)
    {
        kDebug() << "Invalid color depth: " << colorDepth;
        validPatternFile = false;
    }

    if (!validPatternFile)
    {
        file.close();
        return thumb;
    }

    // get the brush name size, skip the bytes
    unsigned int nameLength = header - HEADERSIZE;
    in.skipRawData(nameLength);

    // read the image data
    int dataLength = w * h * colorDepth;
    QScopedArrayPointer<char> data(new char[dataLength]);
    int bytesRead  = in.readRawData(data.data(), dataLength);
    file.close();

    // valid brush data?
    if (bytesRead == -1 || bytesRead != dataLength)
    {
        return thumb;
    }

    // generate thumbnail
    QImage::Format imageFormat = (colorDepth == 1 || colorDepth == 3) ? QImage::Format_RGB32 : QImage::Format_ARGB32;

    thumb  = QImage(w, h, imageFormat);
    quint32 step = 0;

    switch (colorDepth)
    {
        case 1: // Grayscale
        {
            for (quint32 y = 0; y < h; ++y)
            {
                for (quint32 x = 0; x < w; ++x, ++step)
                {
                    qint32 val = static_cast<uchar>(data[step]);
                    thumb.setPixel(x, y, qRgb(val, val, val));
                }
            }

            break;
        }

        case 2: // Grayscale with alpha
        {
            for (quint32 y = 0; y < h; ++y)
            {
                for (quint32 x = 0; x < w; ++x, step += 2)
                {
                    qint32 val   = static_cast<uchar>(data[step]);
                    qint32 alpha = static_cast<uchar>(data[step + 1]);
                    thumb.setPixel(x, y, qRgba(val, val, val, alpha));
                }
            }

            break;
        }

        case 3: // RGB
        {
            for (quint32 y = 0; y < h; ++y)
            {
                for (quint32 x = 0; x < w; ++x, step += 3)
                {
                    thumb.setPixel(x, y, qRgb(static_cast<uchar>(data[step]),
                                              static_cast<uchar>(data[step + 1]),
                                              static_cast<uchar>(data[step + 2])));
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
