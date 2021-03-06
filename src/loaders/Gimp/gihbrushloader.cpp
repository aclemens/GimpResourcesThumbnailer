/* ============================================================
 *
 * Date        : 2009-07-03
 * Description : a generator for animated gimp brushes
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

#include "gihbrushloader.h"

// Qt includes

#include <QFile>
#include <QString>

QImage GihBrushLoader::generateThumbnail(QFile& file)
{
    // Read ahead the first two lines and skip the textual information. We don't need it.
    // The actual GBR data is found at line 3.
    file.readLine();
    file.readLine();

    return GbrBrushLoader::generateThumbnail(file);
}
