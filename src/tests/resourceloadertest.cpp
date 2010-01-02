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

#include <QDir>
#include <QString>

// KDE includes

#include <qtest_kde.h>

// Local includes

#include "resourceloader.h"

QTEST_KDEMAIN(ResourceLoaderTest, GUI)

void ResourceLoaderTest::testSuccess_data()
{
    QTest::addColumn<QString>("filename");
    QTest::addColumn<int>("result");

    QTest::newRow("GBR") << QString(KDESRCDIR"/gbr/13fcircle.gbr");
    QTest::newRow("GIH") << QString(KDESRCDIR"/gih/vine.gih");
    QTest::newRow("PAT") << QString(KDESRCDIR"/pat/lala_rgb.pat");
}

void ResourceLoaderTest::testSuccess()
{
    QFETCH(QString, filename);

    ResourceLoader* loader = ResourceLoader::create(filename);
    QVERIFY(loader);

    // make sure an invalid pointer will not fail the rest of the tests
    if (!loader)
    {
        return;
    }

    // make sure the content was loaded successfully
    QVERIFY(loader->success());

    delete loader;
}

void ResourceLoaderTest::testThumbnail_data()
{
    QTest::addColumn<QString>("filename");
    QTest::addColumn<int>("result");

    QTest::newRow("GBR") << QString(KDESRCDIR"/gbr/13fcircle.gbr");
    QTest::newRow("GIH") << QString(KDESRCDIR"/gih/vine.gih");
    QTest::newRow("PAT") << QString(KDESRCDIR"/pat/lala_rgb.pat");
}

void ResourceLoaderTest::testThumbnail()
{
    QFETCH(QString, filename);

    ResourceLoader* loader = ResourceLoader::create(filename);
    QVERIFY(loader);

    // make sure an invalid pointer will not fail the rest of the tests
    if (!loader)
    {
        return;
    }

    // make sure the thumbnail was created successfully
    QVERIFY(!loader->thumbnail().isNull());

    delete loader;
}

void ResourceLoaderTest::testKnownResourcesShouldNotReturnInvalidType_data()
{
    QTest::addColumn<QString>("filename");
    QTest::addColumn<int>("result");

    QTest::newRow("GBR") << QString(KDESRCDIR"/gbr/13fcircle.gbr") << (int)ResourceLoader::GBR;
    QTest::newRow("GIH") << QString(KDESRCDIR"/gih/vine.gih")      << (int)ResourceLoader::GIH;
    QTest::newRow("PAT") << QString(KDESRCDIR"/pat/lala_rgb.pat")  << (int)ResourceLoader::PAT;
}

void ResourceLoaderTest::testKnownResourcesShouldNotReturnInvalidType()
{
    QFETCH(QString, filename);
    QFETCH(int,     result);

    ResourceLoader* loader = ResourceLoader::create(filename);
    QVERIFY(loader);

    // make sure an invalid pointer will not fail the rest of the tests
    if (!loader)
    {
        return;
    }

    // resource type should not be INVALID...
    QVERIFY(loader->resourceType() != ResourceLoader::INVALID);

    // ... and should match the appropriate type for the file extension
    QCOMPARE((int)loader->resourceType(), result);

    delete loader;
}

void ResourceLoaderTest::testInvalidResourcesShouldReturnNullPointer_data()
{
    QTest::addColumn<QString>("filename");

    QTest::newRow("test1") << "filename.gbr";
    QTest::newRow("test2") << "";
    QTest::newRow("test3") << "filename.blub";
    QTest::newRow("test4") << "     ";
}

void ResourceLoaderTest::testInvalidResourcesShouldReturnNullPointer()
{
    QFETCH(QString, filename);

    ResourceLoader* loader = ResourceLoader::create(filename);
    QVERIFY(!loader);
}

void ResourceLoaderTest::testAllExampleData()
{
    QStringList allFiles;

    QStringList filePatterns;
    filePatterns << "*.abr";
    filePatterns << "*.gbr";
    filePatterns << "*.ggr";
    filePatterns << "*.gih";
    filePatterns << "*.gpl";
    filePatterns << "*.pat";
    filePatterns << "*.vbr";

    QDirIterator it(QString(KDESRCDIR), QDir::NoDotAndDotDot | QDir::AllDirs, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        QDir exampleDir(it.next());

        QVERIFY(exampleDir.exists());
        QVERIFY(exampleDir.isReadable());

        QStringList files = exampleDir.entryList(filePatterns, QDir::Files);

        foreach (const QString& file, files)
        {
            allFiles.append(exampleDir.absoluteFilePath(file));
        }
    }

    QVERIFY(!allFiles.isEmpty());

    foreach (const QString& file, allFiles)
    {
        ResourceLoader* loader = ResourceLoader::create(file);
        QVERIFY(loader);

        if (loader)
        {
            QVERIFY(loader->success());
            QVERIFY(!loader->thumbnail().isNull());
        }

        delete loader;
    }
}
