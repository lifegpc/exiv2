// ***************************************************************** -*- C++ -*-
/*
 * Copyright (C) 2004-2021 Exiv2 authors
 * This program is part of the Exiv2 distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, 5th Floor, Boston, MA 02110-1301 USA.
 */
#ifndef PREVIEW_HPP_
#define PREVIEW_HPP_

// *****************************************************************************
#include "exiv2lib_export.h"

#include "image.hpp"

// *****************************************************************************
// namespace extensions
namespace Exiv2 {

// *****************************************************************************
// class definitions

    //! Type of preview image.
    typedef int PreviewId;

    /*!
      @brief Preview image properties.
     */
    struct EXIV2API PreviewProperties {
        //! Preview image mime type.
        std::string mimeType_;
        //! Preview image extension.
        std::string extension_;
        //! Preview image size in bytes.
        uint32_t size_;
        //! Preview image width in pixels or 0 for unknown width.
        uint32_t width_;
        //! Preview image height in pixels or 0 for unknown height.
        uint32_t height_;
        //! Identifies type of preview image.
        PreviewId id_;
    };

    //! Container type to hold all preview images metadata.
    typedef std::vector<PreviewProperties> PreviewPropertiesList;

    /*!
      @brief Class that holds preview image properties and data buffer.
     */
    class EXIV2API PreviewImage {
        friend class PreviewManager;
    public:
        //! @name Constructors
        //@{
        //! Copy constructor
        PreviewImage(const PreviewImage& rhs);
        //@}

        //! @name Manipulators
        //@{
        //! Assignment operator
        PreviewImage& operator=(const PreviewImage& rhs);
        //@}

        //! @name Accessors
        //@{
        /*!
          @brief Return a copy of the preview image data. The caller owns
                 this copy and %DataBuf ensures that it will be deleted.
         */
        DataBuf copy() const;
        /*!
          @brief Return a pointer to the image data for read-only access.
         */
        const byte* pData() const;
        /*!
          @brief Return the size of the preview image in bytes.
         */
        uint32_t size() const;
        /*!
          @brief Write the thumbnail image to a file.

          A filename extension is appended to \em path according to the image
          type of the preview image, so \em path should not include an extension.
          The function will overwrite an existing file of the same name.

          @param path File name of the preview image without extension.
          @return The number of bytes written.
        */
        long writeFile(const std::string& path) const;
        /*!
          @brief Return the MIME type of the preview image, usually either
                 \c "image/tiff" or \c "image/jpeg".
         */
        std::string mimeType() const;
        /*!
          @brief Return the file extension for the format of the preview image
                 (".tif" or ".jpg").
         */
        std::string extension() const;
        /*!
          @brief Return the width of the preview image in pixels.
        */
        uint32_t width() const;
        /*!
          @brief Return the height of the preview image in pixels.
        */
        uint32_t height() const;
        /*!
          @brief Return the preview image type identifier.
        */
        PreviewId id() const;
        //@}

    private:
        //! Private constructor
        PreviewImage(PreviewProperties properties, DataBuf&& data);

        PreviewProperties properties_;          //!< Preview image properties
        DataBuf preview_;                       //!< Preview image data

    }; // class PreviewImage

    /*!
      @brief Class for extracting preview images from image metadata.
     */
    class EXIV2API PreviewManager {
    public:
        //! @name Constructors
        //@{
        //! Constructor.
        explicit PreviewManager(const Image& image);
        //@}

        //! @name Accessors
        //@{
        /*!
          @brief Return the properties of all preview images in a list
                 sorted by preview width * height, starting with the smallest
                 preview image.
         */
        PreviewPropertiesList getPreviewProperties() const;
        /*!
          @brief Return the preview image for the given preview properties.
         */
        PreviewImage getPreviewImage(const PreviewProperties& properties) const;
        //@}

    private:
    const Image& image_;

    }; // class PreviewManager
}                                       // namespace Exiv2

#endif                                  // #ifndef PREVIEW_HPP_
