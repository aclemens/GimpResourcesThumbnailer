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

void BrushLoaderTest::testLoadGBRv1_data()
{
    QTest::addColumn<QString>("brushName");
    QTest::addColumn<QString>("result");

    QTest::newRow("13fcircle")  << QString(KDESRCDIR"/gbr/13fcircle.gbr")
                                << QString(KDESRCDIR"/gbr/13fcircle-result.png");
    QTest::newRow("19fcircle")  << QString(KDESRCDIR"/gbr/19fcircle.gbr")
                                << QString(KDESRCDIR"/gbr/19fcircle-result.png");
}

void BrushLoaderTest::testLoadGBRv1()
{
    QFETCH(QString, brushName);
    QFETCH(QString, result);

    QString brushfile(brushName);
    QString exampleBrushFile(result);
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

void BrushLoaderTest::testLoadGBRv2_3_data()
{
    QTest::addColumn<QString>("brushName");
    QTest::addColumn<QString>("result");

    QTest::newRow("feather")  << QString(KDESRCDIR"/gbr/feather.gbr")
                              << QString(KDESRCDIR"/gbr/feather-result.png");
    QTest::newRow("fraktle3") << QString(KDESRCDIR"/gbr/fraktle3.gbr")
                              << QString(KDESRCDIR"/gbr/fraktle3-result.png");
    QTest::newRow("colored")  << QString(KDESRCDIR"/gbr/colored.gbr")
                              << QString(KDESRCDIR"/gbr/colored-result.png");
}

void BrushLoaderTest::testLoadGBRv2_3()
{
    QFETCH(QString, brushName);
    QFETCH(QString, result);

    QString brushfile(brushName);
    QString exampleBrushFile(result);
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

void BrushLoaderTest::testLoadGIH_data()
{
    QTest::addColumn<QString>("brushName");
    QTest::addColumn<QString>("result");

    QTest::newRow("vine")           << QString(KDESRCDIR"/gih/vine.gih")
                                    << QString(KDESRCDIR"/gih/vine-result.png");
    QTest::newRow("SketchBrush-64") << QString(KDESRCDIR"/gih/SketchBrush-64.gih")
                                    << QString(KDESRCDIR"/gih/SketchBrush-64-result.png");
}

void BrushLoaderTest::testLoadGIH()
{
    QFETCH(QString, brushName);
    QFETCH(QString, result);

    QString brushfile(brushName);
    QString exampleBrushFile(result);
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
