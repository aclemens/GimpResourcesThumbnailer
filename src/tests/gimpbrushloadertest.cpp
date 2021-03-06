/* ============================================================
 *
 * Date        : 2009-06-18
 * Description : a unit test for loading gimp brushes
 *
 * Copyright (C) 2009-2011 by Andi Clemens <andi dot clemens at gmail dot com>
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

#include "gimpbrushloadertest.h"
#include "gimpbrushloadertest.moc"

// Qt includes

#include <QImage>

// Local includes

#include "thumbnailcreator.h"

QTEST_MAIN(GimpBrushLoaderTest)

void GimpBrushLoaderTest::testLoadGBRv1_data()
{
    QTest::addColumn<QString>("brushName");
    QTest::addColumn<QString>("result");

    QTest::newRow("13fcircle")  << QString(KDESRCDIR"/gbr/13fcircle.gbr")
                                << QString(KDESRCDIR"/gbr/13fcircle-result.png");
    QTest::newRow("19fcircle")  << QString(KDESRCDIR"/gbr/19fcircle.gbr")
                                << QString(KDESRCDIR"/gbr/19fcircle-result.png");
}

void GimpBrushLoaderTest::testLoadGBRv1()
{
    QFETCH(QString, brushName);
    QFETCH(QString, result);

    QString brushfile(brushName);
    QString exampleBrushFile(result);
    int width  = 0;
    int height = 0;
    QImage img;

    // basic test: does create() work?
    ThumbnailCreator c;
    QVERIFY(c.create(brushfile, width, height, img));

    // compare the resulting thumbnail with an example output
    QImage example = QImage(exampleBrushFile).convertToFormat(img.format());

    QCOMPARE(img.width(),  example.width());
    QCOMPARE(img.height(), example.height());
    QCOMPARE(img.depth(),  example.depth());
    QCOMPARE(img, example);
}

void GimpBrushLoaderTest::testLoadGBRv2_3_data()
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

void GimpBrushLoaderTest::testLoadGBRv2_3()
{
    QFETCH(QString, brushName);
    QFETCH(QString, result);

    QString brushfile(brushName);
    QString exampleBrushFile(result);
    int width  = 0;
    int height = 0;
    QImage img;

    // basic test: does create() work?
    ThumbnailCreator c;
    QVERIFY(c.create(brushfile, width, height, img));

    // compare the resulting thumbnail with an example output
    QImage example = QImage(exampleBrushFile).convertToFormat(img.format());

    QCOMPARE(img.width(),  example.width());
    QCOMPARE(img.height(), example.height());
    QCOMPARE(img.depth(),  example.depth());
    QCOMPARE(img, example);
}

void GimpBrushLoaderTest::testLoadGIH_data()
{
    QTest::addColumn<QString>("brushName");
    QTest::addColumn<QString>("result");

    QTest::newRow("vine")           << QString(KDESRCDIR"/gih/vine.gih")
                                    << QString(KDESRCDIR"/gih/vine-result.png");
    QTest::newRow("SketchBrush-64") << QString(KDESRCDIR"/gih/SketchBrush-64.gih")
                                    << QString(KDESRCDIR"/gih/SketchBrush-64-result.png");
}

void GimpBrushLoaderTest::testLoadGIH()
{
    QFETCH(QString, brushName);
    QFETCH(QString, result);

    QString brushfile(brushName);
    QString exampleBrushFile(result);
    int width  = 0;
    int height = 0;
    QImage img;

    // basic test: does create() work?
    ThumbnailCreator c;
    QVERIFY(c.create(brushfile, width, height, img));

    // compare the resulting thumbnail with an example output
    QImage example = QImage(exampleBrushFile).convertToFormat(img.format());

    QCOMPARE(img.width(),  example.width());
    QCOMPARE(img.height(), example.height());
    QCOMPARE(img.depth(),  example.depth());
    QCOMPARE(img, example);
}

void GimpBrushLoaderTest::testLoadPAT_data()
{
    QTest::addColumn<QString>("patName");
    QTest::addColumn<QString>("result");

    QTest::newRow("gray")           << QString(KDESRCDIR"/pat/lala_gray.pat")
                                    << QString(KDESRCDIR"/pat/lala_gray_result.png");
    QTest::newRow("grayAlpha")      << QString(KDESRCDIR"/pat/lala_gray_alpha.pat")
                                    << QString(KDESRCDIR"/pat/lala_gray_alpha_result.png");
    QTest::newRow("rgb")            << QString(KDESRCDIR"/pat/lala_rgb.pat")
                                    << QString(KDESRCDIR"/pat/lala_rgb_result.png");
    QTest::newRow("rgba")           << QString(KDESRCDIR"/pat/lala_rgba.pat")
                                    << QString(KDESRCDIR"/pat/lala_rgba_result.png");
}

void GimpBrushLoaderTest::testLoadPAT()
{
    QFETCH(QString, patName);
    QFETCH(QString, result);

    QString patfile(patName);
    QString examplePatFile(result);
    int width  = 0;
    int height = 0;
    QImage img;

    // basic test: does create() work?
    ThumbnailCreator c;
    QVERIFY(c.create(patfile, width, height, img));

    // compare the resulting thumbnail with an example output
    QImage example = QImage(examplePatFile).convertToFormat(img.format());

    QCOMPARE(img.width(),  example.width());
    QCOMPARE(img.height(), example.height());
    QCOMPARE(img.depth(),  example.depth());
    QCOMPARE(img, example);
}
