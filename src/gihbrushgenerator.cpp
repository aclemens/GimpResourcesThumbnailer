/* ============================================================
 *
 * Date        : 2009-07-03
 * Description : a generator for animated gimp brushes
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

#include "gihbrushgenerator.h"

// Qt includes

#include <QFile>
#include <QString>

GihBrushGenerator::GihBrushGenerator()
                 : GbrBrushGenerator()
{
    m_type = GIH;
}

GihBrushGenerator::GihBrushGenerator(const QString& path)
                 : GbrBrushGenerator(path)
{
    m_type = GIH;
}

GihBrushGenerator::~GihBrushGenerator()
{
}

bool GihBrushGenerator::generateThumbnail(QFile& file)
{
    // Read ahead two lines and skip the textual information. We don't need it.
    // The actual GBR data is found at line 3.
    file.readLine();
    file.readLine();

    return GbrBrushGenerator::generateThumbnail(file);
}
