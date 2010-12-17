/* ============================================================
 *
 * Date        : 2009-07-03
 * Description : a generator for animated gimp brushes
 *
 * Copyright (C) 2009-2010 by Andi Clemens <andi dot clemens at gmx dot net>
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

protected:

    virtual QImage generateThumbnail(QFile& file);
};

#endif /* GIHBRUSHLOADER_H */
