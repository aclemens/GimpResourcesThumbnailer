/* ============================================================
 *
 * Date        : 2009-06-26
 * Description : a VBR brush generator
 *
 * Copyright (C) 2009-2010 by Andi Clemens <andi dot clemens at gmx dot net>
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

#include "vbrbrushloader.h"

// Qt includes

#include <QFile>
#include <QPainter>
#include <QPixmap>
#include <QRadialGradient>
#include <QString>
#include <QStringList>
#include <QTextStream>

// KDE includes

#include <kdebug.h>

namespace
{
/**
 * the maximum thumbnail size
 */
static const int MAX_SIZE = 256;
}

QImage VbrBrushLoader::generateThumbnail(QFile& file)
{
    QTextStream in(&file);
    QStringList data;
    QImage thumb;

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
     Line 6: The brush radius, in pixels.
     Line 7: The number of spikes for the shape.
     Line 8: The brush hardness.
     Line 9: The brush aspect ratio.
     Line 10: The brush angle.
     */

    int dataSize = data.count();

    // make sure data can be read
    if (dataSize != 8 && dataSize != 10)
    {
        kDebug() << "Invalid Gimp Brush (VBR) header";
        return thumb;
    }

    QString& magic   = data[0];
    QString& version = data[1];

    // check basic parameters
    if (
        (magic != QString("GIMP-VBR"))  ||
        (version != QString("1.0") && version != QString("1.5"))
    )
    {
        kDebug() << "Invalid Gimp Brush (VBR) data!";
        return thumb;
    }

    // now load the rest
    bool shaped = (dataSize == 10);
    QString empty;

    QString brushName   = data[2];
    QString spacing     = shaped ? data[4] : data[3];
    QString radius      = shaped ? data[5] : data[4];
    QString hardness    = shaped ? data[7] : data[5];
    QString aspectRatio = shaped ? data[8] : data[6];
    QString angle       = shaped ? data[9] : data[7];
    QString spikes      = shaped ? data[6] : empty;
    QString style       = shaped ? data[3] : empty;

    //    kDebug() << "brushName: "   << brushName;
    //    kDebug() << "spacing: "     << spacing;
    //    kDebug() << "radius: "      << radius;
    //    kDebug() << "hardness: "    << hardness;
    //    kDebug() << "aspectRatio: " << aspectRatio;
    //    kDebug() << "angle: "       << angle;
    //    kDebug() << "spikes: "      << spikes;
    //    kDebug() << "style: "       << style;

    // --------------------------------------------------------

    bool ok;
    bool allOk = true;

    qreal r_radius = radius.toDouble(&ok);
    allOk = allOk && ok;

    qreal r_hardness = hardness.toDouble(&ok);
    allOk = allOk && ok;

    qreal r_aspectRatio = aspectRatio.toDouble(&ok);
    allOk = allOk && ok;

    qreal r_angle = angle.toDouble(&ok);
    allOk = allOk && ok;

    int r_spikes = 0;

    if (!spikes.isEmpty())
    {
        r_spikes = spikes.toInt(&ok);
        allOk = allOk && ok;
    }

    if (!allOk)
    {
        kDebug() << "Error while converting strings to their appropriate value type!";
        return thumb;
    }

    thumb = generateShapedBrush(style, r_radius, r_hardness, r_aspectRatio, r_angle, r_spikes);
    return thumb;
}

QImage VbrBrushLoader::generateShapedBrush(QString shape, qreal radius, qreal hardness, qreal aspect, qreal angle, int spikes)
{
    /**
     * @todo re-think the "hardness" implementation
     */

    /**
     * @todo Implement spikes
     */
    Q_UNUSED(spikes)

    // create a pixmap with MAX_SIZE, fill it with white
    QPixmap brushPixmap(MAX_SIZE, MAX_SIZE);
    brushPixmap.fill(Qt::white);

    QPainter brushPainter(&brushPixmap);
    brushPainter.setRenderHint(QPainter::Antialiasing, true);
    brushPainter.setPen(QPen(Qt::transparent));

    // center the coord system in the image
    brushPainter.translate(brushPixmap.rect().center());

    // rotate the brush
    brushPainter.rotate(-angle);

    brushPainter.save();

    // radius of the brush
    qreal brushRadius  = (radius > MAX_SIZE) ? (qreal)MAX_SIZE : radius;

    // brush rectangle
    QRectF brushRect(0, 0, brushRadius, brushRadius / aspect);

    QString _shape = shape.toLower();

    if (_shape == "square")
    {
        // hardness of the brush
        QRadialGradient gradient(QPoint(0, 0), brushRadius);
        gradient.setColorAt(0.0,      Qt::black);
        gradient.setColorAt(hardness, Qt::black);
        gradient.setColorAt(1.0,      Qt::white);
        brushPainter.setBrush(QBrush(gradient));

        int w = brushRect.width();
        int h = brushRect.height();
        brushPainter.drawRect(0 - (w / 2.0), 0 - (h / 2.0), w, h);
    }
    else if (_shape == "diamond")
    {
        /**
         * @todo implement diamond shape
         */
    }
    else
    {
        // hardness of the brush
        QRadialGradient gradient(QPoint(0, 0), brushRadius);
        gradient.setColorAt(0.0,      Qt::black);
        gradient.setColorAt(hardness, Qt::black);
        gradient.setColorAt(1.0,      Qt::white);
        brushPainter.setBrush(QBrush(gradient));

        int w = brushRect.width();
        int h = brushRect.height();
        brushPainter.drawEllipse(QPoint(0, 0), w, h);
    }

    brushPainter.restore();
    brushPainter.end();

    return brushPixmap.toImage();
}
