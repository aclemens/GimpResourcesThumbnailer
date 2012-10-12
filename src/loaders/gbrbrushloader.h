/* ============================================================
 *
 * Date        : 2009-07-03
 * Description : a generator for gimp brushes
 *
 * Copyright (C) 2009-2012 by Andi Clemens <andi dot clemens at gmx dot net>
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
class QDataStream;

class GbrBrushLoader : public ResourceLoader
{
public:

    GbrBrushLoader() {};

protected:

    virtual QImage generateThumbnail(QFile& file);

private:

    GbrBrushLoader(const GbrBrushLoader&);
    GbrBrushLoader& operator=(const GbrBrushLoader&);

    bool checkHeaderInformation(QDataStream& ds, quint32 version);

    QImage renderThumbnail(quint32 w, quint32 h, quint32 colorDepth, const QScopedArrayPointer<char> &data);
};

#endif /* GBRBRUSHLOADER_H */
