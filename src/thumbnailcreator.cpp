/* ============================================================
 *
 * Date        : 2009-06-17
 * Description : a thumb creator for gimp resources
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

#include "thumbnailcreator.h"

// Qt includes

#include <QFile>
#include <QFileInfo>
#include <QImage>
#include <QString>
#include <QStringList>

// KDE includes

#include <kdebug.h>

// Local includes

#include "resourceloader.h"
#include "gbrbrushgenerator.h"
#include "abrbrushgenerator.h"

extern "C"
{
    KDE_EXPORT ThumbCreator *new_creator()
    {
        return new ThumbnailCreator;
    }
}

ThumbnailCreator::ThumbnailCreator()
{
}

bool ThumbnailCreator::create(const QString &path, int width, int height, QImage &img)
{
    Q_UNUSED(width)
    Q_UNUSED(height)

    bool success           = false;
    ResourceLoader* loader = ResourceLoader::getLoader(path);
    QImage thumbnail;

    if (loader && loader->success())
    {
        success   = true;
        thumbnail = loader->thumbnail();
    }
    delete loader;

    img = (thumbnail.isNull()) ? QImage() : thumbnail;
    return success;
}

bool ThumbnailCreator::createVBR(QFile& file, int width, int height, QImage &img)
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

ThumbCreator::Flags ThumbnailCreator::flags() const
{
    return None;
}
