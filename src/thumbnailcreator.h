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

#ifndef THUMBNAILCREATOR_H
#define THUMBNAILCREATOR_H

// KDE includes

#include <kio/thumbcreator.h>

class QFile;
class QImage;
class QString;

class ThumbnailCreator : public ThumbCreator
{
public:

    ThumbnailCreator();
    virtual bool  create(const QString& path, int, int, QImage& img);
    virtual Flags flags() const;
};

#endif /* THUMBNAILCREATOR_H */

