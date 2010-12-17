/* ============================================================
 *
 * Date        : 2010-01-17
 * Description : a unit test for loading palettes
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

#ifndef PALETTELOADERTEST_H
#define PALETTELOADERTEST_H

// Qt includes

#include <QtCore/QObject>
#include <QtTest/QtTest>
#include <QtGui/QImage>

// Local includes

#include "paletteloader.h"

class QFile;

class PaletteLoaderStub : public QObject, public PaletteLoader
{
    Q_OBJECT

public:

    PaletteLoaderStub() {};
    virtual ~PaletteLoaderStub() {};

    QImage generateThumb(QFile& file)
    {
        return generateThumbnail(file);
    };
    bool checkPaletteInfo(const QString& palette)
    {
        return checkPaletteInformation(palette);
    };
};

// --------------------------------------------------------

class PaletteLoaderTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void testGenerateThumbnail();
    void testPaletteInformation();
};

#endif /* GIMPBRUSHLOADERTEST_H_ */
