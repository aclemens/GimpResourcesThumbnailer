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

#include <QFile>
#include <QFileInfo>
#include <QImage>
#include <QString>
#include <QStringList>

// KDE includes

#include <kdebug.h>

// Local includes

#include "abrbrushgenerator.h"

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
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        kDebug() << "Error loading Gimp Brush file.";
        return false;
    }

    bool success = false;

    QFileInfo fi(file);
    QString suffix = fi.suffix().toUpper();

    if (suffix == QString("VBR"))
    {
        success = createVBR(file, width, height, img);
    }
    else if (suffix == QString("GIH"))
    {
        // Read ahead two lines and skip the textual information. We don't need it.
        // The actual GBR data is found at line 3.
        file.readLine();
        file.readLine();
        success = createGBR(file, width, height, img);
    }
    else if (suffix == QString("ABR"))
    {
        AbrBrushGenerator g;
        success = g.load(file);
    }
    else if (suffix == QString("GBR"))
    {
        success = createGBR(file, width, height, img);
    }
    else
    {
        success = false;
    }

    return success;
}

bool GimpBrushCreator::createGBR(QFile& file, int, int, QImage &img)
{
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
       >> colorDepth;

    // check if the brush has the right version and magic number
    bool validBrushFile = true;
    switch (version)
    {
        case 1:
        {
            // no magic number and spacing information
            kDebug() << "Gimp Brush format: v1";
            break;
        }
        case 2: case 3:
        {
            in >> magic >> spacing;
            if ((magic != 0x47494D50))
            {
                kDebug() << "No valid Gimp Brush file!";
                validBrushFile = false;
            }
            kDebug() << "Gimp Brush format: v2/v3";
            break;
        }
        default:
            validBrushFile = false;
    }

    if (colorDepth != 1 && colorDepth != 4)
    {
        kDebug() << "Invalid color depth!";
        validBrushFile = false;
    }

    if (!validBrushFile)
    {
        file.close();
        return false;
    }

    // read the brush name
    unsigned int nameLength = headerSize - ((version == 1) ? 20 : 28);
    char* brushName_c       = new char[nameLength];
    in.readRawData(brushName_c, nameLength);
    brushName = QString(brushName_c);

    // read the image data
    int dataLength = w * h * colorDepth;
    char* data     = new char[dataLength];
    int bytesRead  = in.readRawData(data, dataLength);
    file.close();

    // valid brush data?
    if (bytesRead == -1 || bytesRead != dataLength)
        return false;

    // generate thumbnail
    QImage::Format imageFormat;
    imageFormat = (colorDepth == 1) ? QImage::Format_RGB32 : QImage::Format_ARGB32;

    QImage thumbnail(w, h, imageFormat);
    quint32 step = 0;

    switch (colorDepth)
    {
        case 1:
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
            break;
        }
        case 4:
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
    }

    // load image data into reference
    img = thumbnail;
    kDebug() << "Thumbnail for Gimp Brush '" << brushName << "' successfully generated!";

    // cleanup
    delete[] brushName_c;
    delete[] data;

    return (!img.isNull());
}

bool GimpBrushCreator::createVBR(QFile& file, int width, int height, QImage &img)
{
    Q_UNUSED(width)
    Q_UNUSED(height)
    Q_UNUSED(img)

//    int imgSize = qMax(width, height);

    QTextStream in(&file);
    QStringList data;

    while (!in.atEnd())
    {
        data << in.readLine();
    }

    // close the file
    file.close();

    /*
     Non-shaped brushes:

     Line 1: Must always contain the magic string "GIMP-VBR".
     Line 2: Version number, always "1.0".
     Line 3: The name of the brush.  This is a UTF-8 string, with a maximum length of 255 bytes.
     Line 4: The brush spacing.
     Line 5: The brush radius, in pixels.
     Line 6: The brush hardness.
     Line 7: The brush aspect ratio.
     Line 8: The brush angle.

     Shaped brushes:

     Line 1: Must always contain the magic string "GIMP-VBR".
     Line 2: Version number, always "1.5".
     Line 3: The name of the brush.  This is a UTF-8 string, with a maximum length of 255 bytes.
     Line 4: A string giving the shape of the brush.  Currently "Circle",
             "Square", and "Diamond" are supported.  The possible shapes
             are defined by the GimpBrushGeneratedShape enum in
             core-enums.h.
     Line 5: The brush spacing.
     Line 6: The number of spikes for the shape.
     Line 7: The brush radius, in pixels.
     Line 8: The brush hardness.
     Line 9: The brush aspect ratio.
     Line 10: The brush angle.
     */

    int dataSize     = data.count();
    QString& magic   = data[0];
    QString& version = data[1];

    // check basic parameters
    if (
        (magic != QString("GIMP-VBR"))                           ||
        (version != QString("1.0") && version != QString("1.5")) ||
        (dataSize != 8 && dataSize != 10)
       )
    {
        kDebug() << "Invalid Gimp Brush (VBR) data!";
        return false;
    }

    // now load the rest
    bool shaped = (dataSize == 10) ? true : false;
    QString empty;

    QString& brushName   = data[2];
    QString& spacing     = shaped ? data[4] : data[3];
    QString& radius      = shaped ? data[6] : data[4];
    QString& hardness    = shaped ? data[7] : data[5];
    QString& aspectRatio = shaped ? data[8] : data[6];
    QString& angle       = shaped ? data[9] : data[7];
    QString& spikes      = shaped ? data[5] : empty;
    QString& style       = shaped ? data[3] : empty;

    kDebug() << "brushName: "   << brushName;
    kDebug() << "spacing: "     << spacing;
    kDebug() << "radius: "      << radius;
    kDebug() << "hardness: "    << hardness;
    kDebug() << "aspectRatio: " << aspectRatio;
    kDebug() << "angle: "       << angle;
    kDebug() << "spikes: "      << spikes;
    kDebug() << "style: "       << style;

    return false;
}

ThumbCreator::Flags GimpBrushCreator::flags() const
{
    return None;
}
