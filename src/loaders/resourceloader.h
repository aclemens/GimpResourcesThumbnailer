/* ============================================================
 *
 * Date        : 2009-07-03
 * Description : an abstract class for gimp resources
 *
 * Copyright (C) 2009-2011 by Andi Clemens <andi dot clemens at gmail dot com>
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

#include <QImage>

class QFile;
class QString;

class ResourceLoader
{
public:

    virtual ~ResourceLoader();

    static QImage load(const QString& path);

protected:

    ResourceLoader();
    virtual QImage generateThumbnail(QFile& file) = 0;

private:

    ResourceLoader(const ResourceLoader&);
    ResourceLoader& operator=(const ResourceLoader&);

    static ResourceLoader* createLoader(const QString& suffix);
};

#endif /* RESOURCELOADER_H */
