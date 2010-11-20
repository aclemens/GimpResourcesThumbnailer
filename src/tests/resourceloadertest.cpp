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
#include <kdebug.h>

// Local includes

#include "resourceloader.h"

QTEST_KDEMAIN(ResourceLoaderTest, GUI)

void ResourceLoaderTest::testInvalidResourcesShouldReturnEmptyImage_data()
{
    QTest::addColumn<QString>("filename");

    QTest::newRow("test1") << "willfail.gbr";
    QTest::newRow("test2") << "";
    QTest::newRow("test3") << "willfail.vbr";
    QTest::newRow("test4") << "     ";
}

void ResourceLoaderTest::testInvalidResourcesShouldReturnEmptyImage()
{
    QFETCH(QString, filename);

    QImage thumb = ResourceLoader::load(filename);
    QVERIFY(thumb.isNull());
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

    QDirIterator it(QString(KDESRCDIR), QDir::NoDotAndDotDot | QDir::AllDirs,
                    QDirIterator::Subdirectories | QDirIterator::FollowSymlinks );

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
    kDebug() << "testing " << allFiles.count() << " resources...";

    foreach (const QString& file, allFiles)
    {
        QImage thumb = ResourceLoader::load(file);
        QVERIFY(!thumb.isNull());
    }
}
