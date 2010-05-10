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
const int GRADIENT_PARAMS_MIN = 11;
const int GRADIENT_PARAMS_MAX = 15;


QImage GradientLoader::generateThumbnail(QFile& file)
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

    if (isValidResourceFileData(data))
    {
        thumb = drawGradient(data);
    }
    return thumb;
}

bool GradientLoader::isValidResourceFileData(const QStringList& data)
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
    if (values.count() < GRADIENT_PARAMS_MIN || values.count() > GRADIENT_PARAMS_MAX)
    {
        data.status = GradientData::Invalid;
        return data;
    }

    // --------------------------------------------------------

    data.startPoint      = values[0].toFloat(&ok);  allOk = allOk && ok;
    data.middlePoint     = values[1].toFloat(&ok);  allOk = allOk && ok;
    data.endPoint        = values[2].toFloat(&ok);  allOk = allOk && ok;

    data.leftColorRed    = values[3].toFloat(&ok);  allOk = allOk && ok;
    data.leftColorGreen  = values[4].toFloat(&ok);  allOk = allOk && ok;
    data.leftColorBlue   = values[5].toFloat(&ok);  allOk = allOk && ok;
    data.leftColorAlpha  = values[6].toFloat(&ok);  allOk = allOk && ok;

    data.rightColorRed   = values[7].toFloat(&ok);  allOk = allOk && ok;
    data.rightColorGreen = values[8].toFloat(&ok);  allOk = allOk && ok;
    data.rightColorBlue  = values[9].toFloat(&ok);  allOk = allOk && ok;
    data.rightColorAlpha = values[10].toFloat(&ok); allOk = allOk && ok;

    if (values.count() >= (GRADIENT_PARAMS_MIN + 2))
    {
        int _tmp = values[12].toInt(&ok);
        ok = ok && (_tmp >= 0 && _tmp < GradientData::LAST_COLORMODE);
        allOk = allOk && ok;
        if (ok)
        {
            data.coloringMode = (GradientData::ColorMode)_tmp;
        }
    }

    // --------------------------------------------------------

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

    GradientList gradientList = extractGradients(data);
    return drawGradient(gradientList);
}

QImage GradientLoader::drawGradient(const GradientList& data)
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

    foreach (const GradientData& gradient, data)
    {
        if (gradient.status != GradientData::Ok)
        {
            continue;
        }

        qreal start = gradient.startPoint + 0.01;
        qreal end   = gradient.endPoint;

        // do some basic range checks
        start = qMin(1.0, start);
        start = qMax(0.0, start);
        end   = qMin(1.0, end);
        end   = qMax(0.0, end);

        // check coloring mode
        if (gradient.coloringMode == GradientData::HSVclockwise ||
            gradient.coloringMode == GradientData::HSVcounterClockwise
        )
        {
            const int hue_range = 360;

            bool reverse        = (gradient.coloringMode == GradientData::HSVcounterClockwise);
            qreal stepping      = qAbs(end - start) / (qreal)hue_range;
            qreal pos           = reverse ? start : end;

            QColor col = QColor::fromRgbF(gradient.leftColorRed,
                                          gradient.leftColorGreen,
                                          gradient.leftColorBlue,
                                          gradient.leftColorAlpha);
            QColor hsv = col.toHsv();

            for (int h = 0.0; h < hue_range; ++h)
            {
                grad.setColorAt(pos, QColor::fromHsvF((qreal)h / hue_range,
                                                      hsv.saturationF(),
                                                      hsv.valueF(),
                                                      hsv.alphaF()));

                if (reverse)
                {
                    pos += stepping;
                }
                else
                {
                    pos -= stepping;
                }
            }
        }
        else
        {
            grad.setColorAt(start, QColor::fromRgbF(gradient.leftColorRed,
                                                    gradient.leftColorGreen,
                                                    gradient.leftColorBlue,
                                                    gradient.leftColorAlpha));

            grad.setColorAt(end, QColor::fromRgbF(gradient.rightColorRed,
                                                  gradient.rightColorGreen,
                                                  gradient.rightColorBlue,
                                                  gradient.rightColorAlpha));
        }
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
