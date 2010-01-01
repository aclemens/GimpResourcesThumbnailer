/* ============================================================
 *
 * Date        : 2009-06-26
 * Description : a VBR brush generator
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

#include "vbrbrushloader.h"

// Qt includes

#include <QFile>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QPixmap>
#include <QPainter>

// KDE includes

#include <kdebug.h>

const int MAX_SIZE = 256;

bool VbrBrushLoader::generateThumbnail(QFile& file)
{
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
     Line 6: The brush radius, in pixels.
     Line 7: The number of spikes for the shape.
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

    kDebug() << "brushName: "   << brushName;
    kDebug() << "spacing: "     << spacing;
    kDebug() << "radius: "      << radius;
    kDebug() << "hardness: "    << hardness;
    kDebug() << "aspectRatio: " << aspectRatio;
    kDebug() << "angle: "       << angle;
    kDebug() << "spikes: "      << spikes;
    kDebug() << "style: "       << style;

    // --------------------------------------------------------

    qreal r_radius      = radius.toDouble();
    qreal r_hardness    = hardness.toDouble();
    qreal r_aspectRatio = aspectRatio.toDouble();
    qreal r_angle       = angle.toDouble();
    int r_spikes        = angle.toInt();

    QImage img = generateShapedBrush(style, r_radius, r_hardness, r_aspectRatio, r_angle, r_spikes);

    if (!img.isNull())
    {
        m_thumbnail = img;
        return true;
    }

    return false;
}

QImage VbrBrushLoader::generateShapedBrush(QString shape, qreal radius, qreal hardness, qreal aspect, qreal angle, int spikes)
{
    /**
     * @todo Implement hardness
     */
    Q_UNUSED(hardness)

    /**
     * @todo Implement spikes
     */
    Q_UNUSED(spikes)

    QPixmap pix(MAX_SIZE, MAX_SIZE);
    pix.fill(Qt::white);

    QPainter p(&pix);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setBrush(QBrush(Qt::black));
    p.setPen(QPen());

    // center point of thumbnail
    p.save();
    QPointF centerP = QRect(0, 0, MAX_SIZE, MAX_SIZE).center();
    p.translate(centerP);

    // rotate world matrix
    p.rotate(-angle);

    // radius of the brush
    qreal t_radius  = (radius > MAX_SIZE) ? (qreal)MAX_SIZE : radius;

    // brush rectangle
    QRectF b_rect(0, 0, t_radius, t_radius / aspect);

    QString _shape = shape.toLower();
    if (_shape == "square")
    {
        int w = b_rect.width();
        int h = b_rect.height();
        p.drawRect(0 - (w / 2.0), 0 - (h / 2.0), w, h);
    }
    else if (_shape == "diamond")
    {
        /**
         * @todo implement diamond shape
         */
    }
    else
    {
        int w = b_rect.width();
        int h = b_rect.height();
        p.drawEllipse(QPoint(0, 0), w, h);
    }

    p.end();
    return pix.toImage();
}
