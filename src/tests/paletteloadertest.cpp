/* ============================================================
 *
 * Date        : 2010-01-17
 * Description : a unit test for loading palettes
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

#include "paletteloadertest.moc"

// Qt includes

#include <QImage>

// Local includes

#include "thumbnailcreator.h"

QTEST_MAIN(PaletteLoaderTest)


void PaletteLoaderTest::testGenerateThumbnail()
{
    PaletteLoaderStub loader;
    QFile f;
    QVERIFY(!loader.generateThumb(f));
}

void PaletteLoaderTest::testPaletteInformation()
{
    PaletteLoaderStub loader;

    // valid
    QVERIFY(loader.checkPaletteInfo("104  96  84"));
    QVERIFY(loader.checkPaletteInfo("104  96  84 colorname"));

    // invalid
    QVERIFY(!loader.checkPaletteInfo("colorname 104  96  84"));
    QVERIFY(!loader.checkPaletteInfo("104  96  colorname 84"));
    QVERIFY(!loader.checkPaletteInfo("104  96"));

    // valid (too much parameters, but we ignore the rest)
    QVERIFY(loader.checkPaletteInfo("104  96 111 21212"));
    QVERIFY(loader.checkPaletteInfo("104  96 111 21212 colorname"));

    // invalid
    QVERIFY(!loader.checkPaletteInfo(" "));
    QVERIFY(!loader.checkPaletteInfo(""));
}
