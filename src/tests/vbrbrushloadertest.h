/* ============================================================
 *
 * Date        : 2010-01-17
 * Description : a unit test for loading vbr brushes
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

#ifndef VBRBRUSHLOADERTEST_H
#define VBRBRUSHLOADERTEST_H

// Qt includes

#include <QtCore/QObject>
#include <QtTest/QtTest>
#include <QtGui/QImage>

// Local includes

#include "vbrbrushloader.h"

class QFile;

class VbrLoaderStub : public QObject, public VbrBrushLoader
{
    Q_OBJECT

public:

    VbrLoaderStub() {};
    ~VbrLoaderStub() {};

    QImage generateThumb(QFile& file) { return generateThumbnail(file); };
};

// --------------------------------------------------------

class VbrBrushLoaderTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void testGenerateThumbnail();
};

#endif /* GIMPBRUSHLOADERTEST_H_ */
