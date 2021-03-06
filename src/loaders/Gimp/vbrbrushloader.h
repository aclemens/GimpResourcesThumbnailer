/* ============================================================
 *
 * Date        : 2009-06-26
 * Description : a VBR brush generator
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

#ifndef VBRBRUSHLOADER_H
#define VBRBRUSHLOADER_H

// Qt includes

#include <QImage>
#include <QString>

// Local includes

#include "resourceloader.h"

class QFile;

class VbrBrushLoader : public ResourceLoader
{
public:

    VbrBrushLoader() {};

protected:

    virtual QImage generateThumbnail(QFile& file);

private:

    VbrBrushLoader(const VbrBrushLoader&);
    VbrBrushLoader& operator=(const VbrBrushLoader&);

    QImage generateShapedBrush(QString shape, qreal radius, qreal hardness, qreal aspect, qreal angle, int spikes);
};

#endif /* VBRBRUSHLOADER_H */
