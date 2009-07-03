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

#include "abrbrushloader.h"

// Qt includes

#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QDataStream>

// KDE includes

#include <kdebug.h>

AbrBrushGenerator::AbrBrushGenerator()
                 : ResourceLoader()
{
    header             = 0;
    sampledBrushHeader = 0;
}

AbrBrushGenerator::~AbrBrushGenerator()
{
    delete header;
    delete sampledBrushHeader;
}

//bool AbrBrushGenerator::load(const QString& filename)
//{
//    if (filename.isEmpty())
//        return false;
//
//    QFile file(filename);
//    if (!file.open(QIODevice::ReadOnly))
//    {
//        kDebug() << "Error loading ABR Brush file.";
//        return false;
//    }
//
//    QFileInfo fi(file);
//    if (fi.suffix().toUpper() != QString("ABR"))
//    {
//        kDebug() << "Error loading ABR Brush file.";
//        return false;
//    }
//
//    return load(file);
//}

bool AbrBrushGenerator::generateThumbnail(QFile& file)
{
    QDataStream in(&file);

    // check stream status
    if (!streamIsOk(in))
        return false;

    // header already set? If so, delete it
    if (header)
        delete header;
    header = new AbrHeader;

    // read stream
    in >> header->version
       >> header->count;

    // check stream and header
    if (!streamIsOk(in) || !validAbrHeader())
        return false;

    kDebug() << "version: " << header->version;
    kDebug() << "count: "   << header->count;

    // continue reading the AbrSampledBrushHeader
    if (sampledBrushHeader)
        delete sampledBrushHeader;
    sampledBrushHeader = new AbrSampledBrushHeader;

    in >> sampledBrushHeader->misc
       >> sampledBrushHeader->spacing
       >> sampledBrushHeader->antiAliasing;

    for (int i = 0; i < 4; ++i)
    {
        qint16 tmp;
        in >> tmp;
        sampledBrushHeader->bounds[i] = tmp;
    }

    for (int i = 0; i < 4; ++i)
    {
        qint32 tmp;
        in >> tmp;
        sampledBrushHeader->bounds_long[i] = tmp;
    }

    in >> sampledBrushHeader->depth;

    // check stream and sampled brush header
    if (!streamIsOk(in) || !validAbrSampledBrushHeader())
        return false;

    kDebug() << "spacing: "      << sampledBrushHeader->spacing;
    kDebug() << "antiAliasing: " << sampledBrushHeader->antiAliasing;
    kDebug() << "bounds: "       << sampledBrushHeader->bounds;
    kDebug() << "bounds_long: "  << sampledBrushHeader->bounds_long;
    kDebug() << "depth: "        << sampledBrushHeader->depth;

    return false;
}

bool AbrBrushGenerator::streamIsOk(QDataStream& stream)
{
    if (stream.status() != QDataStream::Ok)
    {
        kDebug() << "Error reading ABR brush data stream.";
        return false;
    }
    return true;
}

bool AbrBrushGenerator::validAbrHeader()
{
    if (!header)
        return false;

    return validAbrHeader(header);
}

bool AbrBrushGenerator::validAbrHeader(AbrHeader* header)
{
    if (!header)
    {
        kDebug() << "No valid ABR header found.";
        return false;
    }

    bool valid = true;

    if (header->version != 6 && header->version != 12)
    {
        kDebug() << "Invalid ABR header version: " << header->version;
        valid = false;
    }

    if (header->count <= 0)
    {
        kDebug() << "Invalid count in ABR header: " << header->count;
        valid = false;
    }

    return valid;
}

bool AbrBrushGenerator::validAbrSampledBrushHeader()
{
    if (!sampledBrushHeader)
        return false;

    return validAbrSampledBrushHeader(sampledBrushHeader);
}

bool AbrBrushGenerator::validAbrSampledBrushHeader(AbrSampledBrushHeader* /*header*/)
{
    return true;
}
