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
        GBR = 0,
        GIH,
        VBR,
        PAT,
        ABR
    };

    ResourceLoader();
    virtual ~ResourceLoader();

    bool                   load(const QString& path);
    bool                   success();

    int                    resourceType();

    QImage&                thumbnail();
    QString&               name();

    static ResourceLoader* getLoader(const QString& path);

protected:

    int                    m_type;
    QString                m_name;
    QImage                 m_thumbnail;

    virtual bool           generateThumbnail(QFile& file) = 0;
};

#endif /* RESOURCELOADER_H */
