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

#ifndef GIHBRUSHLOADER_H
#define GIHBRUSHLOADER_H

#include "gbrbrushloader.h"

class QFile;

class GihBrushLoader : public GbrBrushLoader
{
public:

    GihBrushLoader() {};

protected:

    virtual QImage generateThumbnail(QFile& file);

private:

    GihBrushLoader(const GihBrushLoader&);
    GihBrushLoader& operator=(const GihBrushLoader&);
};

#endif /* GIHBRUSHLOADER_H */
