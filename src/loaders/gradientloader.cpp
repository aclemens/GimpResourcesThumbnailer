/* ============================================================
 *
 * Date        : 2010-01-02
 * Description : a gradient generator
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

#include "gradientloader.h"

// Qt includes

#include <QFile>
#include <QLinearGradient>
#include <QPainter>
#include <QPixmap>
#include <QRegExp>
#include <QTextStream>

// KDE includes

#include <kdebug.h>

/**
 * the maximum thumbnail size
 */
const int MAX_THUMB_SIZE = 256;

/**
 * The size of the header
 */
const int HEADER_SIZE = 3;

/**
 * The minimum data in a gradient file
 */
const int MIN_DATA = 4;

/**
 * The number of parameters to describe a gradient
 */
const int GRADIENT_PARAMS = 11;


bool GradientLoader::generateThumbnail(QFile& file)
{
    /*
     GRADIENT FILE SPECIFICATION:

     Line 1 (string)   : Resource type (must be "GIMP Gradient")
     Line 2 (string)   : The name of the gradient ("Name: <THENAME>")
     Line 3 (int)      : The number of gradients in the following sections, each gradient is represented by a single line
     Line 4-X          : The line describing one gradient

     GRADIENT DEFINITION (line 4-X):

     Column1   (float) : Start point
     Column2   (float) : Middle point
     Column3   (float) : End point

     Column4   (float) : Red value of left color
     Column5   (float) : Green value of left color
     Column6   (float) : Blue value of left color
     Column7   (float) : Alpha value of left color

     Column8   (float) : Red value of right color
     Column9   (float) : Green value of right color
     Column10  (float) : Blue value of right color
     Column11  (float) : Alpha value of right color

     // OPTIONAL (not used at the moment)
     Column12: (int)   : BlendingFunction (linear, curved, sinusoidal, spherical (counterclockwise), spherical (clockwise)
     Column13: (int)   : ColorMode (RGB, HSVcounterclockwise, HSVclockwise)
     Column14: (int)   : Left  ColorType (Fixed, Foreground, ForegroundTransparent, Background, BackgroundTransparent)
     Column14: (int)   : Right ColorType (Fixed, Foreground, ForegroundTransparent, Background, BackgroundTransparent)
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
        QImage img = drawGradient(data);

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

bool GradientLoader::validData(const QStringList& data)
{
    if (data.isEmpty() || data.count() < MIN_DATA)
    {
        return false;
    }

    bool validData        = true;
    int dataSize          = data.count();
    QString magic         = data[0];
    int numberOfGradients = data[2].toInt();

    // check basic parameters
    if ( (magic != QString("GIMP Gradient")) || (numberOfGradients < 1) )
    {
        kDebug() << "Invalid basic parameters";
        validData = false;
    }

    // check if gradient data is valid
    validData = validData && (numberOfGradients == (dataSize - HEADER_SIZE));
    if (validData)
    {
        for (int i = HEADER_SIZE; i < dataSize; ++i)
        {
            validData = validData && checkGradientInformation(data[i]);
            if (!validData)
            {
                kDebug() << "Invalid gradient lines";
                break;
            }
        }
    }

    if (!validData)
    {
        kDebug() << "Invalid Gimp Gradient (GGR) data!";
    }
    return validData;
}

GradientData GradientLoader::getGradientInformation(const QString& gradient)
{
    QRegExp sep("\\s+");
    QStringList values = gradient.split(sep);
    bool ok            = true;
    bool allOk         = true;

    GradientData data;
    if (values.count() < GRADIENT_PARAMS)
    {
        data.status = GradientData::Invalid;
        return data;
    }

    data.startPoint      = values[0].toFloat(&ok);  allOk = allOk && ok;
    data.middlePoint     = values[1].toFloat(&ok);  allOk = allOk && ok;
    data.endPoint        = values[2].toFloat(&ok);  allOk = allOk && ok;
                                                    allOk = allOk && ok;
    data.leftColorRed    = values[3].toFloat(&ok);  allOk = allOk && ok;
    data.leftColorGreen  = values[4].toFloat(&ok);  allOk = allOk && ok;
    data.leftColorBlue   = values[5].toFloat(&ok);  allOk = allOk && ok;
    data.leftColorAlpha  = values[6].toFloat(&ok);  allOk = allOk && ok;
                                                    allOk = allOk && ok;
    data.rightColorRed   = values[7].toFloat(&ok);  allOk = allOk && ok;
    data.rightColorGreen = values[8].toFloat(&ok);  allOk = allOk && ok;
    data.rightColorBlue  = values[9].toFloat(&ok);  allOk = allOk && ok;
    data.rightColorAlpha = values[10].toFloat(&ok); allOk = allOk && ok;

    /**
     * @todo use blendingFunction and other optional parameters for better representation
      of the gradient
     */
    /*
    data.blendingFunction  = (GradientData::BlendingFunction)values[11].toInt(&ok);
    allOk = allOk && ok;

    data.coloringMode      = (GradientData::ColorMode)values[12].toInt(&ok);
    allOk = allOk && ok;

    data.leftColorType     = (GradientData::ColorType)values[13].toInt(&ok);
    allOk = allOk && ok;

    data.rightColorType    = (GradientData::ColorType)values[14].toInt(&ok);
    allOk = allOk && ok;
    */

    // check if data was converted correctly, set status accordingly
    data.status = allOk ? GradientData::Ok : GradientData::Invalid;

    return data;
}

bool GradientLoader::checkGradientInformation(const QString& gradient)
{
    GradientData data = getGradientInformation(gradient);

    return (data.status == GradientData::Ok);
}

QImage GradientLoader::drawGradient(const QStringList& data)
{
    if (data.isEmpty())
    {
        return QImage();
    }

    QPixmap pix(MAX_THUMB_SIZE, MAX_THUMB_SIZE / 2);
    pix.fill(Qt::white);

    QPainter p(&pix);
    p.setPen(QPen(Qt::transparent));
    p.setRenderHint(QPainter::Antialiasing, true);

    QLinearGradient grad(0.0, 0.0, 1.0, 0.0);
    grad.setCoordinateMode(QGradient::StretchToDeviceMode);

    GradientList gradientList = extractGradients(data);
    foreach (const GradientData& gradient, gradientList)
    {
        if (gradient.status != GradientData::Ok)
        {
            continue;
        }

        qreal start = gradient.startPoint + 0.01;
        qreal end   = gradient.endPoint;

        // do some basic range checks
        if (start > 1.0)
        {
            start = 1.0;
        }

        if (start < 0.0)
        {
            start = 0.0;
        }

        if (end > 1.0)
        {
            end = 1.0;
        }

        if (end < 0.0)
        {
            end = 0.0;
        }

        grad.setColorAt(start, QColor::fromRgbF(gradient.leftColorRed,
                                                gradient.leftColorGreen,
                                                gradient.leftColorBlue,
                                                gradient.leftColorAlpha));

        grad.setColorAt(end, QColor::fromRgbF(gradient.rightColorRed,
                                              gradient.rightColorGreen,
                                              gradient.rightColorBlue,
                                              gradient.rightColorAlpha));
    }

    p.fillRect(pix.rect(), QBrush(grad));
    p.end();

    return pix.toImage();
}

void GradientLoader::debugGradientData(const GradientData& data)
{
    kDebug() << "Status: "          << data.status;

    kDebug() << "start: "           << data.startPoint;
    kDebug() << "middle: "          << data.middlePoint;
    kDebug() << "end: "             << data.endPoint;

    kDebug() << "leftColorRed: "    << data.leftColorRed;
    kDebug() << "leftColorGreen: "  << data.leftColorGreen;
    kDebug() << "leftColorBlue: "   << data.leftColorBlue;
    kDebug() << "leftColorAlpha: "  << data.leftColorAlpha;

    kDebug() << "rightColorRed: "   << data.rightColorRed;
    kDebug() << "rightColorGreen: " << data.rightColorGreen;
    kDebug() << "rightColorBlue: "  << data.rightColorBlue;
    kDebug() << "rightColorAlpha: " << data.rightColorAlpha;
}

GradientList GradientLoader::extractGradients(const QStringList& data)
{
    GradientList list;

    foreach (const QString& info, data)
    {
        if (checkGradientInformation(info))
        {
            list.append(getGradientInformation(info));
        }
    }

    return list;
}
