/* ============================================================
 *
 * Date        : 2009-06-17
 * Description : a thumb creator for gimp resources
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

#include "thumbnailcreator.h"

// Qt includes

#include <QImage>
#include <QString>

// Local includes

#include "resourceloader.h"

extern "C"
{
    KDE_EXPORT ThumbCreator *new_creator()
    {
        return new ThumbnailCreator;
    }
}

ThumbnailCreator::ThumbnailCreator()
{
}

bool ThumbnailCreator::create(const QString &path, int width, int height, QImage &img)
{
    Q_UNUSED(width)
    Q_UNUSED(height)

    bool success           = false;
    ResourceLoader* loader = ResourceLoader::create(path);

    if (loader && loader->success())
    {
        success = true;
        img     = loader->thumbnail();
    }

    delete loader;
    return success;
}

ThumbCreator::Flags ThumbnailCreator::flags() const
{
    return None;
}
