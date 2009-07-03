/* ============================================================
 *
 * Date        : 2009-07-03
 * Description : a generator for gimp brushes
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

#ifndef GBRBRUSHLOADER_H
#define GBRBRUSHLOADER_H

// Local includes

#include "resourceloader.h"

class QFile;
class QString;

class GbrBrushLoader : public ResourceLoader
{

public:

    GbrBrushLoader();
    GbrBrushLoader(const QString& path);
    ~GbrBrushLoader();

protected:

    virtual bool generateThumbnail(QFile& file);
};

#endif /* GBRBRUSHLOADER_H */
