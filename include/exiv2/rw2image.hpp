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
#ifndef RW2IMAGE_HPP_
#define RW2IMAGE_HPP_

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
      @brief Class to access raw Panasonic RW2 images.  Exif metadata is
          supported directly, IPTC and XMP are read from the Exif data, if
          present.
     */
    class EXIV2API Rw2Image : public Image {
    public:
        //! @name Creators
        //@{
        /*!
          @brief Constructor to open an existing RW2 image. Since the
              constructor can not return a result, callers should check the
              good() method after object construction to determine success or
              failure.
          @param io An auto-pointer that owns a BasicIo instance used for
              reading and writing image metadata. \b Important: The constructor
              takes ownership of the passed in BasicIo instance through the
              auto-pointer. Callers should not continue to use the BasicIo
              instance after it is passed to this method.  Use the Image::io()
              method to get a temporary reference.
         */
        explicit Rw2Image(BasicIo::UniquePtr io);
        //@}

        //! @name Manipulators
        //@{
        void printStructure(std::ostream& out, PrintStructureOption option, int depth) override;
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
          @brief Not supported. RW2 format does not contain a comment.
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

        //! @name NOT implemented
        //@{
        //! Copy constructor
        Rw2Image(const Rw2Image& rhs) = delete;
        //! Assignment operator
        Rw2Image& operator=(const Rw2Image& rhs) = delete;
        //@}

    }; // class Rw2Image

    /*!
      @brief Stateless parser class for data in RW2 format. Images use this
             class to decode and encode RW2 data. Only decoding is currently
             implemented. See class TiffParser for details.
     */
    class EXIV2API Rw2Parser {
    public:
        /*!
          @brief Decode metadata from a buffer \em pData of length \em size
                 with data in RW2 format to the provided metadata containers.
                 See TiffParser::decode().
        */
        static ByteOrder decode(
                  ExifData& exifData,
                  IptcData& iptcData,
                  XmpData&  xmpData,
            const byte*     pData,
                  uint32_t  size
        );

    }; // class Rw2Parser

// *****************************************************************************
// template, inline and free functions

    // These could be static private functions on Image subclasses but then
    // ImageFactory needs to be made a friend.
    /*!
      @brief Create a new Rw2Image instance and return an auto-pointer to it.
             Caller owns the returned object and the auto-pointer ensures that
             it will be deleted.
     */
    EXIV2API Image::UniquePtr newRw2Instance(BasicIo::UniquePtr io, bool create);

    //! Check if the file iIo is a RW2 image.
    EXIV2API bool isRw2Type(BasicIo& iIo, bool advance);

}                                       // namespace Exiv2

#endif                                  // #ifndef RW2IMAGE_HPP_
