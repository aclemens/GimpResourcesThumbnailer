/* ============================================================
 *
 * Date        : 2009-07-03
 * Description : an abstract class for gimp resources
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

#include "resourceloader.h"

// Qt includes

#include <QFile>
#include <QFileInfo>
#include <QString>

// KDE includes

#include <kdebug.h>

// Loader includes

#include "abrbrushloader.h"
#include "gbrbrushloader.h"
#include "gihbrushloader.h"
#include "gradientloader.h"
#include "paletteloader.h"
#include "patternloader.h"
#include "vbrbrushloader.h"

ResourceLoader::ResourceLoader()
{
}

ResourceLoader::~ResourceLoader()
{
}

ResourceLoader* ResourceLoader::createLoader(const QFile &file)
{
    ResourceLoader* loader = 0;

    QFileInfo fi(file);
    const QString suffix = fi.suffix().toUpper();

    if (suffix == QString("GBR"))
    {
        loader = new GbrBrushLoader();
    }
    else if (suffix == QString("GIH"))
    {
        loader = new GihBrushLoader();
    }
    else if (suffix == QString("VBR"))
    {
        loader = new VbrBrushLoader();
    }
    else if (suffix == QString("PAT"))
    {
        loader = new PatternLoader();
    }
    else if (suffix == QString("ABR"))
    {
        loader = new AbrBrushLoader();
    }
    else if (suffix == QString("GGR"))
    {
        loader = new GradientLoader();
    }
    else if (suffix == QString("GPL"))
    {
        loader = new PaletteLoader();
    }

    return loader;
}

QImage ResourceLoader::load(const QString& path)
{
    QImage thumb;
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly))
    {
        kDebug() << "Error loading resource file.";
        return thumb;
    }

    QScopedPointer<ResourceLoader> loader(createLoader(file));

    if (!loader.isNull())
    {
        thumb = loader->generateThumbnail(file);
    }

    return thumb;
}
