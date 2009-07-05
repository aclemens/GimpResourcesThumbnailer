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

#include "abrbrushloader.h"

// Qt includes

#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QDataStream>

// KDE includes

#include <kdebug.h>

bool AbrBrushLoader::generateThumbnail(QFile& file)
{
    QDataStream in(&file);

    // read header
    AbrHeader header;
    if (!readHeader(in, header) || !streamIsOk(in))
        return false;

    return true;
}

bool AbrBrushLoader::streamIsOk(QDataStream& stream)
{
    if (stream.status() != QDataStream::Ok)
    {
        kDebug() << "Error reading ABR brush data stream.";
        return false;
    }
    return true;
}

bool AbrBrushLoader::readHeader(QDataStream& stream, AbrHeader& header)
{
    // save current stream position
    qint64 oldpos = stream.device()->pos();

    stream >> header.version;
    switch (header.version)
    {
        case 1:
        case 2:
            stream >> header.count;
            break;
        case 6:
            stream >> header.subversion;
            break;
        default:
            header.subversion = 0;
            header.count      = 0;
    }

    // restore previous stream position
    stream.device()->seek(oldpos);

    return validHeader(header);
}

bool AbrBrushLoader::validHeader(AbrHeader& header)
{
    // assume that everything went wrong
    bool valid = false;

    // check for valid header values
    switch (header.version)
    {
        case 1:
        case 2:
            valid = true;
            break;
        case 6:
            if (header.subversion == 1 || header.subversion == 2)
                valid = true;
            break;
        default:
            valid = false;
            kDebug() << "Invalid ABR header version: " << header.version;
    }

    if (header.count <= 0)
    {
        kDebug() << "Invalid count in ABR header: " << header.count;
        valid = false;
    }

    return valid;
}

bool AbrBrushLoader::seachFor8BIM(QDataStream& stream)
{
    const qint32 MAGIC = 0x3842494D;    // 8BIM
    const qint32 TAG   = 0x73616D70;    // samp

    qint32 magic;
    qint32 tag;
    qint32 sectionSize;

    while (!stream.device()->atEnd())
    {
        stream >> magic
               >> tag;

        if (magic != MAGIC)
        {
            kDebug() << "invalid magic number: " << QByteArray::fromHex(QString::number(magic, 16).toAscii());;
            return false;
        }

        if (tag == TAG)
            return true;

        stream >> sectionSize;
        qint64 pos = stream.device()->pos() + sectionSize;
        stream.device()->seek(pos);
    }
    return false;
}
