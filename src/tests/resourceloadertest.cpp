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

#include "resourceloadertest.h"
#include "resourceloadertest.moc"

// Qt includes

#include <QString>

// Local includes

#include "resourceloader.h"

QTEST_MAIN(ResourceLoaderTest)

void ResourceLoaderTest::testKnownResourcesShouldNotReturnInvalidType_data()
{
    QTest::addColumn<QString>("filename");
    QTest::addColumn<int>    ("result");

    QTest::newRow("GBR") << QString(KDESRCDIR"/gbr/13fcircle.gbr") << (int)ResourceLoader::GBR;
    QTest::newRow("GIH") << QString(KDESRCDIR"/gih/vine.gih")      << (int)ResourceLoader::GIH;
}

void ResourceLoaderTest::testKnownResourcesShouldNotReturnInvalidType()
{
    QFETCH(QString, filename);
    QFETCH(int,     result);

    ResourceLoader* loader = ResourceLoader::getLoader(filename);

    // resource type should not be INVALID...
    QVERIFY(loader->resourceType() != ResourceLoader::INVALID);

    // ... and should match the appropriate type for the file extension
    QCOMPARE(loader->resourceType(), result);
}

void ResourceLoaderTest::testUnknownResourcesShouldReturnNullPointer_data()
{
    QTest::addColumn<QString>("filename");

    QTest::newRow("test1") << "filename.gbr";
    QTest::newRow("test2") << "";
    QTest::newRow("test3") << "filename.blub";
}

void ResourceLoaderTest::testUnknownResourcesShouldReturnNullPointer()
{
    QFETCH(QString, filename);

    ResourceLoader* loader = ResourceLoader::getLoader(filename);
    QVERIFY(!loader);
    delete loader;
}
