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

    bool success = false;

    switch (header.version)
    {
        case 1:
        case 2:
            if (loadv1_2_data(in, header))
                success = true;
            break;
        case 6:
            if (loadv6_data(in, header))
                success = false;
            break;
        default:
            success = true;
    }

    return success;
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
    stream >> header.version;
    switch (header.version)
    {
        case 1:
        case 2:
            stream >> header.count;
            break;
        case 6:
            stream >> header.subversion;
            header.count = getSamplesCount(stream);
            break;
        default:
            header.subversion = 0;
            header.count = 0;
    }

    kDebug() << "version: "     << header.version;
    kDebug() << "subversion: "  << header.subversion;
    kDebug() << "count: "       << header.count;

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
    const qint32 MAGIC = 0x3842494D; // 8BIM
    const qint32 TAG   = 0x73616D70; // samp

    qint32 magic;
    qint32 tag;
    qint32 sectionSize;

    if (!streamIsOk(stream))
        return false;

    while (!stream.device()->atEnd())
    {
        stream >> magic >> tag;
        if (magic != MAGIC)
        {
            kDebug() << "invalid magic number: " << QByteArray::fromHex(QString::number(magic, 16).toAscii());;
            return false;
        }

        if (tag == TAG)
            return true;

        if (!streamIsOk(stream))
            return false;

        stream >> sectionSize;
        qint64 pos = stream.device()->pos() + sectionSize;
        stream.device()->seek(pos);

        if (!streamIsOk(stream))
            return false;
    }
    return false;
}

qint16 AbrBrushLoader::getSamplesCount(QDataStream& stream)
{
    if (!streamIsOk(stream))
        return 0;

    qint64 oldPos;
    qint32 sectionSize;
    qint32 sectionEnd;
    qint32 samples = 0;
    qint32 dataStart;

    qint32 brushSize;
    qint32 brushEnd;

    oldPos = stream.device()->pos();

    if (!seachFor8BIM(stream))
    {
        stream.device()->seek(oldPos);
        return 0;
    }

    stream >> sectionSize;
    dataStart  = stream.device()->pos();
    sectionEnd = sectionSize + dataStart;

    while (!stream.device()->atEnd() && stream.device()->pos() < sectionEnd)
    {
        stream >> brushSize;
        brushEnd = brushSize;
        /* complement to 4 */
        while (brushEnd % 4 != 0)
            brushEnd++;

        qint64 newPos = brushEnd + stream.device()->pos();
        stream.device()->seek(newPos);
        samples++;
    }

    stream.device()->seek(dataStart);

    return samples;
}

bool AbrBrushLoader::loadv1_2_data(QDataStream& stream, AbrHeader& header)
{
    return false;
}

bool AbrBrushLoader::loadv6_data(QDataStream& stream, AbrHeader& header)
{
    return false;
}
