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

#include "brushloadertest.h"
#include "brushloadertest.moc"

// Qt includes

#include <QImage>

// Local includes

#include "gimpbrushcreator.h"

QTEST_MAIN(BrushLoaderTest)

void BrushLoaderTest::testLoadGimpBrushV2()
{
    QString brushfile("feather.gbr");
    int width  = 0;
    int height = 0;
    QImage img;

    GimpBrushCreator c;

    // basic test: does create() work?
    QVERIFY( c.create(brushfile, width, height, img) );
}
