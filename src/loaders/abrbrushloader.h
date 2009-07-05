/* ============================================================
 *
 * Date        : 2009-06-27
 * Description : a generator for photoshop brushes
 *
 * Copyright (C) 2009 by Andi Clemens <andi dot clemens at gmx dot net>
 *
 * Code was inspired by Eric Lamarque <eric.lamarque@free.fr>
 *      (ABR plugin, http://registry.gimp.org/node/126)
 *
 * Adobe and Adobe Photoshop are trademarks of Adobe Systems
 * Incorporated that may be registered in certain jurisdictions.
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

class QFile;
class QDataStream;

struct AbrHeader
{
    qint16 version;
    qint16 subversion;
    qint16 count;
};

class AbrBrushLoader : public ResourceLoader
{

protected:

    virtual bool generateThumbnail(QFile& file);

private:

    bool   streamIsOk(QDataStream& stream);

    bool   readHeader(QDataStream& stream, AbrHeader& header);
    bool   validHeader(AbrHeader& header);

    bool   seachFor8BIM(QDataStream& stream);
    qint16 getSamplesCount(QDataStream& stream);
};

#endif /* ABRBRUSHLOADER_H */
