/* ============================================================
 *
 * Date        : 2009-06-17
 * Description : a thumb creator for gimp resources
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

/**
 * @mainpage
 * @author Andi Clemens
 *
 * GimpResourcesThumbnailer is a thumbnail service for KDE. It can be used in any KDE application that supports
 * the thumbnail kioslave, like dolphin or digiKam.
 * You can preview all of your Gimp resources like brushes and patterns, to easily manage them into folders,
 * without the need to open Gimp.
 *
 * Currently the following resource types are supported:
 * @li <b>Gimp Brushes</b> (.GBR, .GIH, .VBR)
 * @li <b>Gimp Patterns</b> (.PAT)
 * @li <b>Gimp Palettes</b> (.GPL)
 * @li <b>Gimp Gradients</b> (.GGR)
 * @li <b>Adobe Photoshop Brushes</b> (.ABR (v6))
 *
 * For a list of missing features, see \ref todo.
 */

#ifndef THUMBNAILCREATOR_H
#define THUMBNAILCREATOR_H

// KDE includes

#include <kio/thumbcreator.h>

class QImage;
class QString;

/**
 * @brief The %ThumbnailCreator generates the thumbnail for a resource file.
 *
 * This class will be called by the @e thumbnail kioslave each time the KDE system needs to generate a
 * thumbnail for a file.
 *
 * It creates a ResourceLoader object that will try to start the correct loader for the Gimp resource file.
 */
class ThumbnailCreator : public ThumbCreator
{
public:

    ThumbnailCreator();

    /**
     * Creates a thumbnail
     *
     * Note that the width and height parameters should not be used
     * for scaling.
     * If the resulting preview is larger than width x height, it will be
     * scaled down.
     *
     * @param path the (always local) file path to create a preview for
     * @param width maximum width for the preview
     * @param height maximum height for the preview
     * @param img this image will contain the preview on success
     *
     * @return true if preview generation succeeded
     */
    virtual bool  create(const QString& path, int width, int height, QImage& img);

    /**
     * The flags of this plugin:
     * @li @b None nothing special
     * @li @b DrawFrame a frame should be painted around the preview
     * @li @b BlendIcon the mimetype icon should be blended over the preview
     *
     * @return flags for this plugin
     */
    virtual Flags flags() const;

private:

    ThumbnailCreator(const ThumbnailCreator&);
    ThumbnailCreator& operator=(const ThumbnailCreator&);
};

#endif /* THUMBNAILCREATOR_H */

