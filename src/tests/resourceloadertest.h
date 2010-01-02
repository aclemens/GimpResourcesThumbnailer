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

#ifndef RESOURCELOADERTEST_H
#define RESOURCELOADERTEST_H

// Qt includes

#include <QtCore/QObject>

class ResourceLoaderTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void testSuccess_data();
    void testSuccess();

    void testThumbnail_data();
    void testThumbnail();

    void testKnownResourcesShouldNotReturnInvalidType_data();
    void testKnownResourcesShouldNotReturnInvalidType();

    void testInvalidResourcesShouldReturnNullPointer_data();
    void testInvalidResourcesShouldReturnNullPointer();

    void testAllExampleData();
};

#endif /* RESOURCELOADERTEST_H_ */
