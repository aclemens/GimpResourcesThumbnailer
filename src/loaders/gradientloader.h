/* ============================================================
 *
 * Date        : 2010-01-02
 * Description : a gradient generator
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

    enum ColorMode
    {
        RGB = 0,
        HSVcounterClockwise,
        HSVclockwise,
        LAST_COLORMODE
    };

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

    ColorMode        coloringMode;
};

typedef QList<GradientData> GradientList;

// --------------------------------------------------------

/**
 * @brief A loader class for gradient resource files.
 *
 * The GradientLoader class is used to generate thumbnails for gradient resource files. The file format is a normal
 * ASCII text file, where each gradient is represented by one line in the file. The information is separated by one
 * or more whitespace characters:
 *
     \verbatim
     GRADIENT FILE SPECIFICATION:

     Line 1 (string)   : Resource type (must be "GIMP Gradient")
     Line 2 (string)   : The name of the gradient ("Name: <THENAME>")
     Line 3 (int)      : The number of gradients in the following sections, each gradient is represented by a single line
     Line 4-...        : A line describing one gradient

     GRADIENT DEFINITION (line 4-...):

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
     Column15: (int)   : Right ColorType (Fixed, Foreground, ForegroundTransparent, Background, BackgroundTransparent)
     \endverbatim
 */
class GradientLoader : public ResourceLoader
{
protected:

    /**
     * Generates a thumbnail from a resource file. This is the main method of the GradientLoader class, normally this
     * is the only method you need to call from this class.
     * @param file a resource file
     * @return a QImage used for the thumbnail creator object
     */
    virtual QImage generateThumbnail(QFile& file);

    /**
     * Checks if the content of a resource file is valid.
     * @param data the content of a resource file
     * @return true if valid, otherwise false
     */
    bool isValidResourceFileData(const QStringList& data);

    /**
     * Prints the GradientData object to stdout. This is a helper method for debugging and shouldn't be called normally.
     * @param data a GradientData object
     */
    void debugGradientData(const GradientData& data);

    /**
     * Checks the gradient information for validity
     * @see getGradientInformation
     * @param gradient string that holds the gradient information
     * @return
     */
    bool checkGradientInformation(const QString& gradient);

    /**
     * Extracts the information from a gradient's string representation.
     * @param gradient string that holds the gradient information
     * @return a GradientData object representing the gradient
     */
    GradientData getGradientInformation(const QString& gradient);

    /**
     * Generates a list of GradientData objects from a resource file. Each line in the file is run through
     * checkGradientInformation. If it passes the test, the data gets added to the GradientList.
     * @param data
     * @return a list of gradient data
     */
    GradientList extractGradients(const QStringList& data);

    /**
     * Draws a gradient from the given data.
     * @param data the resource file as a list of strings.
     * @return a QImage used for the thumbnail creator object
     */
    QImage drawGradient(const QStringList& data);
    /**
     * Draws a gradient from the given data.
     * @param data a GradientList object
     * @return a QImage used for the thumbnail creator object
     */
    QImage drawGradient(const GradientList& data);
};

#endif /* GRADIENTLOADER_H */
