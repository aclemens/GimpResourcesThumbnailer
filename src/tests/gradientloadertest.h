/* ============================================================
 *
 * Date        : 2009-06-18
 * Description : a unit test for loading gimp brushes
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

#ifndef GRADIENTLOADERTEST_H
#define GRADIENTLOADERTEST_H

// Qt includes

#include <QtCore/QObject>
#include <QtTest/QtTest>

// Local includes

#include "gradientloader.h"

class QFile;

class GradientLoaderStub : public QObject, public GradientLoader
{
    Q_OBJECT

public:

    GradientLoaderStub() {};
    ~GradientLoaderStub() {};

    bool generateThumb(QFile& file) { return generateThumbnail(file); };
    bool checkGradientInfo(const QString& gradient) { return checkGradientInformation(gradient); };
};

// --------------------------------------------------------

class GradientLoaderTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void testGenerateThumbnail();
    void testGradientInformation();
};

#endif /* GIMPBRUSHLOADERTEST_H_ */
