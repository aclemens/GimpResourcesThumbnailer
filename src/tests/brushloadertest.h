/* ============================================================
 *
 * Date        : 2009-06-18
 * Description : a thumbcreator unit test for gimp brushes
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

#ifndef BRUSHLOADERTEST_H
#define BRUSHLOADERTEST_H

// Qt includes

#include <QtTest/QtTest>

class BrushLoaderTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void testLoadGimpBrushV1();
    void testLoadGimpBrushV2();

};

#endif /* BRUSHLOADERTEST_H_ */
