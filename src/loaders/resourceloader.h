/* ============================================================
 *
 * Date        : 2009-07-03
 * Description : an abstract class for gimp resources
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

#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H

#include <QString>
#include <QImage>

class QFile;

class ResourceLoader
{
public:

    enum ResourceType
    {
        INVALID = -1,
        GBR     =  0,
        GIH,
        VBR,
        PAT,
        GGR,
        ABR
    };

    ResourceLoader();
    virtual ~ResourceLoader();

    bool                   success();
    int                    resourceType();
    QImage&                thumbnail();
    static ResourceLoader* create(const QString& path);

protected:

    bool         m_success;
    QImage       m_thumbnail;
    virtual bool generateThumbnail(QFile& file) = 0;

private:

    int  m_type;
    bool load(const QString& path);
};

#endif /* RESOURCELOADER_H */
