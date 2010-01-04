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

#ifndef GRADIENTLOADER_H
#define GRADIENTLOADER_H

// Qt includes

#include <QImage>
#include <QList>
#include <QString>

// Local includes

#include "resourceloader.h"

class QFile;

struct GradientData
{
    enum Status
    {
        Ok = 0,
        Invalid
    };

//    enum BlendingFunction
//    {
//        Linear = 0,
//        Curved,
//        Sinusoidal,
//        SphericalIncreasing,
//        SphericalDecreasing
//    };
//
//    enum ColorMode
//    {
//        RGB = 0,
//        HSVcounterClockwise,
//        HSVclockwise
//    };
//
//    enum ColorType
//    {
//        Fixed = 0,
//        Foreground,
//        ForegroundTransparent,
//        Background,
//        BackgroundTransparent
//    };

    Status           status;

    float            startPoint;
    float            middlePoint;
    float            endPoint;

    float            leftColorRed;
    float            leftColorGreen;
    float            leftColorBlue;
    float            leftColorAlpha;

    float            rightColorRed;
    float            rightColorGreen;
    float            rightColorBlue;
    float            rightColorAlpha;

//    BlendingFunction blendingFunction;
//    ColorMode        coloringMode;
//
//    ColorType        leftColorType;
//    ColorType        rightColorType;
};

typedef QList<GradientData> GradientList;

// --------------------------------------------------------

class GradientLoader : public ResourceLoader
{
protected:

    virtual bool generateThumbnail(QFile& file);

private:

    bool validData(const QStringList& data);
    void debugGradientData(const GradientData& data);

    bool         checkGradientInformation(const QString& gradient);
    GradientData getGradientInformation(const QString& gradient);
    GradientList extractGradients(const QStringList& data);
    QImage       drawGradient(const QStringList& data);
};

#endif /* GRADIENTLOADER_H */
