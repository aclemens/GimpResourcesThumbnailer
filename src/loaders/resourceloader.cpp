/* ============================================================
 *
 * Date        : 2009-07-03
 * Description : an abstract class for gimp resources
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

#include "resourceloader.h"

// Qt includes

#include <QFile>
#include <QFileInfo>

// KDE includes

#include <kdebug.h>

// Loader includes

#include "gihbrushloader.h"
#include "gbrbrushloader.h"
#include "abrbrushloader.h"
#include "vbrbrushloader.h"
#include "patternloader.h"

ResourceLoader::ResourceLoader()
{
    m_success = false;
    m_type    = INVALID;
}

ResourceLoader::~ResourceLoader()
{
}

ResourceLoader* ResourceLoader::create(const QString& path)
{
    ResourceLoader* loader = 0;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        kDebug() << "Error loading resource file.";
        return loader;
    }

    QFileInfo fi(file);
    QString suffix = fi.suffix().toUpper();

    if (suffix == QString("GBR"))
    {
        loader = new GbrBrushLoader();
        loader->m_type = GBR;
    }
    else if (suffix == QString("GIH"))
    {
        loader = new GihBrushLoader();
        loader->m_type = GIH;
    }
    else if (suffix == QString("VBR"))
    {
        /**
         * @todo implement me (VbrBrushLoader)
         */
    }
    else if (suffix == QString("PAT"))
    {
        loader = new PatternLoader();
        loader->m_type = PAT;
    }
    else if (suffix == QString("ABR"))
    {
        loader = new AbrBrushLoader();
        loader->m_type = ABR;
    }

    if (loader)
    {
        loader->load(path);
    }

    return loader;
}

QImage& ResourceLoader::thumbnail()
{
    return m_thumbnail;
}

int ResourceLoader::resourceType()
{
    return m_type;
}

bool ResourceLoader::load(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        kDebug() << "Error loading resource file.";
        return false;
    }

    m_success = generateThumbnail(file);
    return m_success;
}

bool ResourceLoader::success()
{
    return (!m_thumbnail.isNull() && m_success);
}
