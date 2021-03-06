/* ============================================================
 *
 * Date        : 2009-07-03
 * Description : a loader for gimp patterns
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

#ifndef PATTERNLOADER_H
#define PATTERNLOADER_H

// Local includes

#include "resourceloader.h"

class QFile;

class PatternLoader : public ResourceLoader
{
public:

    PatternLoader() {};

protected:

    virtual QImage generateThumbnail(QFile& file);

private:

    PatternLoader(const PatternLoader&);
    PatternLoader& operator=(const PatternLoader&);
};

#endif /* PATTERNLOADER_H */
