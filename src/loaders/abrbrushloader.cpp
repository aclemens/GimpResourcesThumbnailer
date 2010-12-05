/* ============================================================
 *
 * Date        : 2009-06-27
 * Description : a generator for photoshop brushes
 *
 * Copyright (C) 2009 by Andi Clemens <andi dot clemens at gmx dot net>
 *
 * Code was inspired by Eric Lamarque <eric.lamarque@free.fr>
 *      (ABR plugin, http://registry.gimp.org/node/126)
 *
 * Adobe and Adobe Photoshop are trademarks of Adobe Systems
 * Incorporated that may be registered in certain jurisdictions.
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

#include "abrbrushloader.h"

// Qt includes

#include <QDataStream>
#include <QFile>
#include <QFileInfo>
#include <QImage>
#include <QString>

// KDE includes

#include <kdebug.h>

namespace
{
static const qint32 _8BIM_MAGIC = 0x3842494D; // 8BIM
static const qint32 _8BIM_TAG   = 0x73616D70; // samp
}

QImage AbrBrushLoader::generateThumbnail(QFile& file)
{
    QImage thumb;
    QDataStream in(&file);
    AbrHeader header;

    // read header
    if (!readHeader(in, header) || !streamIsOk(in))
    {
        return thumb;
    }

    switch (header.version)
    {
        case 1:
        case 2:
            loadv1_2_data(in, header, thumb);
            break;
        case 6:
            loadv6_data(in, header, thumb);
            break;
        default:
            break;
    }

    return thumb;
}

bool AbrBrushLoader::streamIsOk(QDataStream& stream)
{
    if (stream.status() != QDataStream::Ok)
    {
        kDebug() << "Error reading ABR brush data stream.";
        return false;
    }

    return true;
}

bool AbrBrushLoader::readHeader(QDataStream& stream, AbrHeader& header)
{
    stream >> header.version;

    switch (header.version)
    {
        case 1:
        case 2:
            stream >> header.count;
            break;
        case 6:
            stream >> header.subversion;
            header.count = getSamplesCount(stream);
            break;
        default:
            header.subversion = 0;
            header.count = 0;
    }

    return validHeader(header);
}

bool AbrBrushLoader::validHeader(AbrHeader& header)
{
    // assume that everything went wrong
    bool valid = false;

    // check for valid header values
    switch (header.version)
    {
        case 1:
        case 2:
            valid = true;
            break;
        case 6:
            if (header.subversion == 1 || header.subversion == 2)
            {
                valid = true;
            }

            break;
        default:
            valid = false;
            kDebug() << "Invalid ABR header version: " << header.version;
    }

    if (header.count <= 0)
    {
        kDebug() << "Invalid count in ABR header: " << header.count;
        valid = false;
    }

    return valid;
}

bool AbrBrushLoader::seachFor8BIM(QDataStream& stream)
{
    if (!streamIsOk(stream))
    {
        return false;
    }

    while (!stream.device()->atEnd())
    {
        qint32 magic = 0;
        qint32 tag   = 0;

        stream >> magic >> tag;

        if (magic != _8BIM_MAGIC)
        {
            kDebug() << "invalid magic number: " << QByteArray::fromHex(QString::number(magic, 16).toAscii());;
            return false;
        }

        if (tag == _8BIM_TAG)
        {
            return true;
        }

        if (!streamIsOk(stream))
        {
            return false;
        }

        qint32 sectionSize = 0;
        stream >> sectionSize;

        qint64 pos = stream.device()->pos() + sectionSize;
        stream.device()->seek(pos);

        if (!streamIsOk(stream))
        {
            return false;
        }
    }

    return false;
}

qint16 AbrBrushLoader::getSamplesCount(QDataStream& stream)
{
    if (!streamIsOk(stream))
    {
        return 0;
    }

    qint64 oldPos = stream.device()->pos();

    if (!seachFor8BIM(stream))
    {
        stream.device()->seek(oldPos);
        return 0;
    }

    qint32 sectionSize = 0;
    qint32 sectionEnd  = 0;
    qint32 samples     = 0;
    qint32 dataStart   = 0;
    qint32 brushSize   = 0;
    qint32 brushEnd    = 0;

    stream >> sectionSize;
    dataStart  = stream.device()->pos();
    sectionEnd = dataStart + sectionSize;

    while (!stream.device()->atEnd() && stream.device()->pos() < sectionEnd)
    {
        stream >> brushSize;
        brushEnd = brushSize;

        /* complement to 4 */
        while (brushEnd % 4 != 0)
        {
            ++brushEnd;
        }

        qint64 newPos = brushEnd + stream.device()->pos();
        stream.device()->seek(newPos);
        ++samples;
    }

    stream.device()->seek(dataStart);

    return samples;
}

bool AbrBrushLoader::loadv1_2_data(QDataStream& stream, AbrHeader& header, QImage& img)
{
    /**
     * @todo implement me!
     */
    Q_UNUSED(stream)
    Q_UNUSED(header)
    Q_UNUSED(img)

    return false;
}

bool AbrBrushLoader::loadv6_data(QDataStream& stream, AbrHeader& header, QImage& img)
{
    qint32 brush_size      = 0;
    qint32 brush_end       = 0;
    qint32 complement_to_4 = 0;
    qint64 next_brush      = 0;

    stream >> brush_size;
    brush_end = brush_size;

    /* complement to 4 */
    while (brush_end % 4 != 0)
    {
        ++brush_end;
    }

    complement_to_4 = brush_end - brush_size;
    next_brush = stream.device()->pos() + brush_end;

    // discard key
    stream.device()->seek(stream.device()->pos() + 37);

    if (header.subversion == 1)
    {
        /* discard short coordinates and unknown short */
        stream.device()->seek(stream.device()->pos() + 10);
    }
    else
    {
        /* discard unknown bytes */
        stream.device()->seek(stream.device()->pos() + 264);
    }

    if (!streamIsOk(stream))
    {
        return false;
    }

    qint32 top         = 0;
    qint32 left        = 0;
    qint32 bottom      = 0;
    qint32 right       = 0;
    qint16 depth       = 0;
    qint8  compression = 0;

    qint32 width       = 0;
    qint32 height      = 0;
    qint32 size        = 0;

    stream >> top
           >> left
           >> bottom
           >> right
           >> depth
           >> compression;

    width  = right - left;
    height = bottom - top;
    size   = width * height;

    char* buffer = new char[size];
    int r        = -1;

    if (!compression)
    {
        kDebug() << "trying to read uncompressed data...";
        r = stream.readRawData(buffer, size);
    }
    else
    {
        kDebug() << "trying to read compressed data...";
        r = rle_decode(stream, buffer, height);
    }

    if (r == -1)
    {
        kDebug() << "failed while reading data...";
        delete[] buffer;
        return false;
    }

    // set image data
    QImage tmpImage(width, height, QImage::Format_RGB32);
    qint32 step = 0;

    for (qint32 y = 0; y < height; ++y)
    {
        for (qint32 x = 0; x < width; ++x, ++step)
        {
            qint32 val = 255 - static_cast<uchar>(buffer[step]);
            tmpImage.setPixel(x, y, qRgb(val, val, val));
        }
    }

    img = tmpImage;

    delete[] buffer;

    if (!streamIsOk(stream) || img.isNull())
    {
        return false;
    }

    return true;
}

int AbrBrushLoader::rle_decode(QDataStream& stream, char* buffer, qint32 height)
{
    qint32  n;
    qint8   n_tmp;
    qint8   ch;
    qint8   ch_tmp;
    qint32  i, j, c;
    qint16* cscanline_len;

    // read compressed sizes for the scanlines
    cscanline_len = new qint16[height];

    for (i = 0; i < height; ++i)
    {
        stream >> cscanline_len[i];
    }

    // unpack scanline data
    for (i = 0; i < height; ++i)
    {
        for (j = 0; j < cscanline_len[i];)
        {
            stream >> n_tmp;
            n = n_tmp;
            ++j;

            if (n >= 128) /* force sign */
            {
                n -= 256;
            }

            if (n < 0)
            {
                /* copy the following char -n + 1 times */
                if (n == -128) /* it's a nop */
                {
                    continue;
                }

                n = -n + 1;
                stream >> ch;
                ++j;

                for (c = 0; c < n; ++c, ++buffer)
                {
                    *buffer = ch;
                }
            }
            else
            {
                /* read the following n + 1 chars (no compr) */
                for (c = 0; c < n + 1; ++c, ++j, ++buffer)
                {
                    stream >> ch_tmp;
                    *buffer = ch_tmp;
                }
            }
        }
    }

    delete[] cscanline_len;

    if (!streamIsOk(stream))
    {
        return -1;
    }

    return 0;
}
