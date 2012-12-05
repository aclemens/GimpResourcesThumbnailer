/* ============================================================
 *
 * Date        : 2010-01-17
 * Description : a unit test for loading gradients
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

#include "gradientloadertest.moc"

// Qt includes

#include <QImage>

// Local includes

#include "thumbnailcreator.h"

QTEST_MAIN(GradientLoaderTest)


void GradientLoaderTest::testGenerateThumbnail()
{
    GradientLoaderStub loader;
    QFile f;
    QVERIFY(loader.generateThumb(f).isNull());
}

void GradientLoaderTest::testGradientInformation()
{
    GradientLoaderStub loader;

    // valid
    const QString gradient("0.876461 0.943172 1.000000 1.000000 0.000000 1.000000 1.000000 1.000000 1.000000 0.000000 1.000000 0 0 0 0");
    QVERIFY(loader.checkGradientInfo(gradient));

    // valid
    const QString gradient1("0.876461 0.943172 1.000000 1.000000 0.000000 1.000000 1.000000 1.000000 1.000000 0.000000 1.000000 0 0");
    QVERIFY(loader.checkGradientInfo(gradient1));

    // valid
    const QString gradient2("0.876461 0.943172 1.000000 1.000000 0.000000 1.000000 1.000000 1.000000 1.000000 0.000000 1.000000");
    QVERIFY(loader.checkGradientInfo(gradient2));

    // invalid (not enough parameters)
    const QString gradient3("0.876461 0.943172 1.000000 0.000000 1.000000 1.000000 1.000000 0.000000 1.000000");
    QVERIFY(!loader.checkGradientInfo(gradient3));
    QVERIFY(!loader.checkGradientInfo(" "));

    // invalid (too much parameters)
    const QString gradient4("0.876461 0.943172 1.000000 1.00 0.00 1.00 1.00 1.00 1.00 0.0 1.00 0 0 0 0 0 0");
    QVERIFY(!loader.checkGradientInfo(gradient4));
}
