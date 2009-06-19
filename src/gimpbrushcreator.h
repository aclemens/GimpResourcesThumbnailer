/* ============================================================
 *
 * Date        : 2009-06-17
 * Description : a thumb creator for gimp brushes
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

#ifndef GIMPBRUSHCREATOR_H
#define GIMPBRUSHCREATOR_H

// KDE includes

#include <kio/thumbcreator.h>

class QFile;
class QImage;
class QString;

class GimpBrushCreator : public ThumbCreator
{
public:

    GimpBrushCreator();
    virtual bool create(const QString& path, int, int, QImage& img);
    virtual Flags flags() const;

private:

    bool createGBR(QFile& file, int, int, QImage& img);
    bool createVBR(QFile& file, int, int, QImage& img);
};

#endif /* GIMPBRUSHCREATOR_H */

