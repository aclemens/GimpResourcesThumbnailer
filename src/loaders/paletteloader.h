/* ============================================================
 *
 * Date        : 2010-01-02
 * Description : a palette thumbnail generator
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

#ifndef PALETTELOADER_H
#define PALETTELOADER_H

// Qt includes

#include <QImage>
#include <QList>
#include <QString>

// Local includes

#include "resourceloader.h"

class QFile;

struct PaletteData
{
    enum Status
    {
        Ok = 0,
        Invalid
    };

    Status status;
    int    red;
    int    green;
    int    blue;
};

typedef QList<PaletteData> PaletteList;

// --------------------------------------------------------

class PaletteLoader : public ResourceLoader
{
protected:

    virtual bool generateThumbnail(QFile& file);

private:

    bool        validData(const QStringList& data);
    void        prepareData(QStringList& data);

    void        debugPaletteData(const PaletteData& data);

    bool        checkPaletteInformation(const QString& palette);
    PaletteData getPaletteInformation(const QString& palette);
    PaletteList extractPaletteColors(const QStringList& data);
    QImage      drawPalette(const QStringList& data);
};

#endif /* PALETTELOADER_H */
