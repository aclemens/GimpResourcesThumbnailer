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

void BrushLoaderTest::testLoadGBRv1()
{
    QString brushfile(KDESRCDIR"/19fcircle.gbr");
    QString exampleBrushFile(KDESRCDIR"/19fcircle-result.png");
    int width  = 0;
    int height = 0;
    QImage img;

    // basic test: does create() work?
    GimpBrushCreator c;
    QVERIFY( c.create(brushfile, width, height, img) );

    // compare the resulting thumbnail with an example output
    QImage example = QImage(exampleBrushFile).convertToFormat(img.format());

    QCOMPARE(img.width(),  example.width());
    QCOMPARE(img.height(), example.height());
    QCOMPARE(img.depth(),  example.depth());
    QCOMPARE(img, example);
}

void BrushLoaderTest::testLoadGBRv2_3()
{
    QString brushfile(KDESRCDIR"/feather.gbr");
    QString exampleBrushFile(KDESRCDIR"/feather-result.png");
    int width  = 0;
    int height = 0;
    QImage img;

    // basic test: does create() work?
    GimpBrushCreator c;
    QVERIFY( c.create(brushfile, width, height, img) );

    // compare the resulting thumbnail with an example output
    QImage example = QImage(exampleBrushFile).convertToFormat(img.format());

    QCOMPARE(img.width(),  example.width());
    QCOMPARE(img.height(), example.height());
    QCOMPARE(img.depth(),  example.depth());
    QCOMPARE(img, example);
}
