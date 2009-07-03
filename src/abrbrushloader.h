/* ============================================================
 *
 * Date        : 2009-06-27
 * Description : a generator for photoshop brushes
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

#ifndef ABRBRUSHLOADER_H
#define ABRBRUSHLOADER_H

// Qt includes

#include <QtCore/QtGlobal>

// Local includes

#include "resourceloader.h"

class QString;
class QFile;
class QDataStream;

struct AbrHeader
{
    qint16 version;
    qint16 count;
};

struct AbrSampledBrushHeader
{
    qint16 type;
    qint32 size;
    qint32 misc;    // this value is ignored
    qint16 spacing;
    qint8  antiAliasing;
    qint16 bounds[4];
    qint32 bounds_long[4];
    qint16 depth;
};

class AbrBrushLoader : public ResourceLoader
{
public:

    AbrBrushLoader();
    ~AbrBrushLoader();

public:

    AbrHeader*             header;
    AbrSampledBrushHeader* sampledBrushHeader;

private:

    virtual bool generateThumbnail(QFile& file);

    bool streamIsOk(QDataStream& stream);

    bool validAbrHeader();
    bool validAbrHeader(AbrHeader* header);

    bool validAbrSampledBrushHeader();
    bool validAbrSampledBrushHeader(AbrSampledBrushHeader* header);
};

#endif /* ABRBRUSHLOADER_H */
