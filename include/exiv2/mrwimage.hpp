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
#ifndef MRWIMAGE_HPP_
#define MRWIMAGE_HPP_

// *****************************************************************************
#include "exiv2lib_export.h"

// included header files
#include "image.hpp"

// *****************************************************************************
// namespace extensions
namespace Exiv2 {

// *****************************************************************************
// class definitions

    /*!
      @brief Class to access raw Minolta MRW images. Exif metadata is supported
             directly, IPTC is read from the Exif data, if present.
     */
    class EXIV2API MrwImage : public Image {
    public:
        //! @name NOT Implemented
        //@{
        //! Copy constructor
        MrwImage(const MrwImage& rhs) = delete;
        //! Assignment operator
        MrwImage& operator=(const MrwImage& rhs) = delete;
        //@}

        //! @name Creators
        //@{
        /*!
          @brief Constructor that can either open an existing MRW image or create
              a new image from scratch. If a new image is to be created, any
              existing data is overwritten. Since the constructor can not return
              a result, callers should check the good() method after object
              construction to determine success or failure.
          @param io An auto-pointer that owns a BasicIo instance used for
              reading and writing image metadata. \b Important: The constructor
              takes ownership of the passed in BasicIo instance through the
              auto-pointer. Callers should not continue to use the BasicIo
              instance after it is passed to this method.  Use the Image::io()
              method to get a temporary reference.
          @param create Specifies if an existing image should be read (false)
              or if a new file should be created (true).
         */
        MrwImage(BasicIo::UniquePtr io, bool create);
        //@}

        //! @name Manipulators
        //@{
        void readMetadata() override;
        /*!
          @brief Todo: Write metadata back to the image. This method is not
              yet implemented. Calling it will throw an Error(kerWritingImageFormatUnsupported).
         */
        void writeMetadata() override;
        /*!
          @brief Todo: Not supported yet, requires writeMetadata(). Calling
              this function will throw an Error(kerInvalidSettingForImage).
         */
        void setExifData(const ExifData& exifData) override;
        /*!
          @brief Todo: Not supported yet, requires writeMetadata(). Calling
              this function will throw an Error(kerInvalidSettingForImage).
         */
        void setIptcData(const IptcData& iptcData) override;
        /*!
          @brief Not supported. MRW format does not contain a comment.
              Calling this function will throw an Error(kerInvalidSettingForImage).
         */
        void setComment(std::string_view comment) override;
        //@}

        //! @name Accessors
        //@{
        std::string mimeType() const override;
        uint32_t pixelWidth() const override;
        uint32_t pixelHeight() const override;
        //@}
    }; // class MrwImage

// *****************************************************************************
// template, inline and free functions

    // These could be static private functions on Image subclasses but then
    // ImageFactory needs to be made a friend.
    /*!
      @brief Create a new MrwImage instance and return an auto-pointer to it.
             Caller owns the returned object and the auto-pointer ensures that
             it will be deleted.
     */
    EXIV2API Image::UniquePtr newMrwInstance(BasicIo::UniquePtr io, bool create);

    //! Check if the file iIo is a MRW image.
    EXIV2API bool isMrwType(BasicIo& iIo, bool advance);

}                                       // namespace Exiv2

#endif                                  // #ifndef MRWIMAGE_HPP_
