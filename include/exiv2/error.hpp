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
/*!
  @file    error.hpp
  @brief   Error class for exceptions, log message class
  @author  Andreas Huggel (ahu)
           <a href="mailto:ahuggel@gmx.net">ahuggel@gmx.net</a>
  @date    15-Jan-04, ahu: created<BR>
           11-Feb-04, ahu: isolated as a component
 */
#ifndef ERROR_HPP_
#define ERROR_HPP_

// *****************************************************************************
#include "exiv2lib_export.h"

// included header files
#include "types.hpp"

// *****************************************************************************
// namespace extensions
namespace Exiv2 {

// *****************************************************************************
// class definitions

    /*!
      @brief Class for a log message, used by the library. Applications can set
             the log level and provide a customer log message handler (callback
             function).

             This class is meant to be used as a temporary object with the
             related macro-magic like this:

             <code>
             EXV_WARNING << "Warning! Something looks fishy.\n";
             </code>

             which translates to

             <code>
             if (LogMsg::warn >= LogMsg::level() && LogMsg::handler())
                 LogMsg(LogMsg::warn).os() << "Warning! Something looks fishy.\n";
             </code>

             The macros EXV_DEBUG, EXV_INFO, EXV_WARNING and EXV_ERROR are
             shorthands and ensure efficient use of the logging facility: If a
             log message doesn't need to be generated because of the log level
             setting, the temp object is not even created.

             Caveat: The entire log message is not processed in this case. So don't
             make that call any logic that always needs to be executed.
     */
    class EXIV2API LogMsg {
    public:
        //! Prevent copy-construction: not implemented.
        LogMsg(const LogMsg&) = delete;
        //! Prevent assignment: not implemented.
        LogMsg& operator=(const LogMsg&) = delete;
        /*!
          @brief Defined log levels. To suppress all log messages, either set the
                 log level to \c mute or set the log message handler to 0.
         */
        enum Level { debug = 0, info = 1, warn = 2, error = 3, mute = 4 };
        /*!
          @brief Type for a log message handler function. The function receives
                 the log level and message and can process it in an application
                 specific way.  The default handler sends the log message to
                 standard error.
         */
        typedef void (*Handler)(int, const char*);

        //! @name Creators
        //@{
        //! Constructor, takes the log message type as an argument
        explicit LogMsg(Level msgType);

        //! Destructor, passes the log message to the message handler depending on the log level
        ~LogMsg();
        //@}

        //! @name Manipulators
        //@{
        //! Return a reference to the ostringstream which holds the log message
        std::ostringstream& os();
        //@}

        /*!
          @brief Set the log level. Only log messages with a level greater or
                 equal \em level are sent to the log message handler. Default
                 log level is \c warn. To suppress all log messages, set the log
                 level to \c mute (or set the log message handler to 0).
        */
        static void setLevel(Level level);
        /*!
          @brief Set the log message handler. The default handler writes log
                 messages to standard error. To suppress all log messages, set
                 the log message handler to 0 (or set the log level to \c mute).
         */
        static void setHandler(Handler handler);
        //! Return the current log level
        static Level level();
        //! Return the current log message handler
        static Handler handler();
        //! The default log handler. Sends the log message to standard error.
        static void defaultHandler(int level, const char* s);

    private:
        // DATA
        // The output level. Only messages with type >= level_ will be written
        static Level level_;
        // The log handler in use
        static Handler handler_;
        // The type of this log message
        const Level msgType_;
        // Holds the log message until it is passed to the message handler
        std::ostringstream os_;

    }; // class LogMsg

// Macros for simple access
//! Shorthand to create a temp debug log message object and return its ostringstream
#define EXV_DEBUG   if (LogMsg::debug >= LogMsg::level() && LogMsg::handler()) LogMsg(LogMsg::debug).os()
//! Shorthand for a temp info log message object and return its ostringstream
#define EXV_INFO    if (LogMsg::info  >= LogMsg::level() && LogMsg::handler()) LogMsg(LogMsg::info).os()
//! Shorthand for a temp warning log message object and return its ostringstream
#define EXV_WARNING if (LogMsg::warn  >= LogMsg::level() && LogMsg::handler()) LogMsg(LogMsg::warn).os()
//! Shorthand for a temp error log message object and return its ostringstream
#define EXV_ERROR   if (LogMsg::error >= LogMsg::level() && LogMsg::handler()) LogMsg(LogMsg::error).os()

#ifdef _MSC_VER
// Disable MSVC warnings "non - DLL-interface classkey 'identifier' used as base
// for DLL-interface classkey 'identifier'"
# pragma warning( disable : 4275 )
#endif

    //! Generalised toString function
    template<typename charT, typename T>
    std::basic_string<charT> toBasicString(const T& arg)
    {
        std::basic_ostringstream<charT> os;
        os << arg;
        return os.str();
    }

    /*!
      @brief Error class interface. Allows the definition and use of a hierarchy
             of error classes which can all be handled in one catch block.
             Inherits from the standard exception base-class, to make life
             easier for library users (they have the option of catching most
             things via std::exception).
     */
    class EXIV2API AnyError : public std::exception {
    public:
        AnyError() = default;
        AnyError(const AnyError& o) = default;

        ~AnyError() noexcept override = default;
        ///@brief  Return the error code.
        virtual int code() const noexcept = 0;
    };

    //! %AnyError output operator
    inline std::ostream& operator<<(std::ostream& os, const AnyError& error)
    {
        return os << error.what();
    }

    //! Complete list of all Exiv2 error codes
    enum ErrorCode {
        kerGeneralError = -1,
        kerSuccess = 0,
        kerErrorMessage,
        kerCallFailed,
        kerNotAnImage,
        kerInvalidDataset,
        kerInvalidRecord,
        kerInvalidKey,
        kerInvalidTag,
        kerValueNotSet,
        kerDataSourceOpenFailed,
        kerFileOpenFailed,
        kerFileContainsUnknownImageType,
        kerMemoryContainsUnknownImageType,
        kerUnsupportedImageType,
        kerFailedToReadImageData,
        kerNotAJpeg,
        kerFailedToMapFileForReadWrite,
        kerFileRenameFailed,
        kerTransferFailed,
        kerMemoryTransferFailed,
        kerInputDataReadFailed,
        kerImageWriteFailed,
        kerNoImageInInputData,
        kerInvalidIfdId,
        //! Entry::setValue: Value too large
        kerValueTooLarge,
        //! Entry::setDataArea: Value too large
        kerDataAreaValueTooLarge,
        kerOffsetOutOfRange,
        kerUnsupportedDataAreaOffsetType,
        kerInvalidCharset,
        kerUnsupportedDateFormat,
        kerUnsupportedTimeFormat,
        kerWritingImageFormatUnsupported,
        kerInvalidSettingForImage,
        kerNotACrwImage,
        kerFunctionNotSupported,
        kerNoNamespaceInfoForXmpPrefix,
        kerNoPrefixForNamespace,
        kerTooLargeJpegSegment,
        kerUnhandledXmpdatum,
        kerUnhandledXmpNode,
        kerXMPToolkitError,
        kerDecodeLangAltPropertyFailed,
        kerDecodeLangAltQualifierFailed,
        kerEncodeLangAltPropertyFailed,
        kerPropertyNameIdentificationFailed,
        kerSchemaNamespaceNotRegistered,
        kerNoNamespaceForPrefix,
        kerAliasesNotSupported,
        kerInvalidXmpText,
        kerTooManyTiffDirectoryEntries,
        kerMultipleTiffArrayElementTagsInDirectory,
        kerWrongTiffArrayElementTagType,
        kerInvalidKeyXmpValue,
        kerInvalidIccProfile,
        kerInvalidXMP,
        kerTiffDirectoryTooLarge,
        kerInvalidTypeValue,
        kerInvalidLangAltValue,
        kerInvalidMalloc,
        kerCorruptedMetadata,
        kerArithmeticOverflow,
        kerMallocFailed,
    };

    /*!
      @brief Simple error class used for exceptions. An output operator is
             provided to print errors to a stream.
     */
    template<typename charT>
    class EXIV2API BasicError : public AnyError {
    public:
        //! @name Creators
        //@{
        //! Constructor taking only an error code
        explicit inline BasicError(ErrorCode code);

        //! Constructor taking an error code and one argument
        template<typename A>
        inline BasicError(ErrorCode code, const A& arg1);

        //! Constructor taking an error code and two arguments
        template<typename A, typename B>
        inline BasicError(ErrorCode code, const A& arg1, const B& arg2);

        //! Constructor taking an error code and three arguments
        template<typename A, typename B, typename C>
        inline BasicError(ErrorCode code, const A& arg1, const B& arg2, const C& arg3);

        //! Virtual destructor. (Needed because of throw())
        inline ~BasicError() noexcept override;
        //@}

        //! @name Accessors
        //@{
        inline int code() const noexcept override;
        /*!
          @brief Return the error message as a C-string. The pointer returned by what()
                 is valid only as long as the BasicError object exists.
         */
        inline const char* what() const noexcept override;
        //@}

    private:
        //! @name Manipulators
        //@{
        //! Assemble the error message from the arguments
        void setMsg();
        //@}

        // DATA
        ErrorCode code_;                       //!< Error code
        int count_;                             //!< Number of arguments
        std::basic_string<charT> arg1_;         //!< First argument
        std::basic_string<charT> arg2_;         //!< Second argument
        std::basic_string<charT> arg3_;         //!< Third argument
        std::string              msg_;          //!< Complete error message
    }; // class BasicError

    //! Error class used for exceptions (std::string based)
    typedef BasicError<char> Error;

// *****************************************************************************
// free functions, template and inline definitions

    //! Return the error message for the error with code \em code.
    const char* errMsg(int code);

    template<typename charT>
    BasicError<charT>::BasicError(ErrorCode code)
        : code_(code), count_(0)
    {
        setMsg();
    }

    template<typename charT> template<typename A>
    BasicError<charT>::BasicError(ErrorCode code, const A& arg1)
        : code_(code), count_(1), arg1_(toBasicString<charT>(arg1))
    {
        setMsg();
    }

    template<typename charT> template<typename A, typename B>
    BasicError<charT>::BasicError(ErrorCode code, const A& arg1, const B& arg2)
        : code_(code), count_(2),
          arg1_(toBasicString<charT>(arg1)),
          arg2_(toBasicString<charT>(arg2))
    {
        setMsg();
    }

    template<typename charT> template<typename A, typename B, typename C>
    BasicError<charT>::BasicError(ErrorCode code, const A& arg1, const B& arg2, const C& arg3)
        : code_(code), count_(3),
          arg1_(toBasicString<charT>(arg1)),
          arg2_(toBasicString<charT>(arg2)),
          arg3_(toBasicString<charT>(arg3))
    {
        setMsg();
    }

    template <typename charT>
    BasicError<charT>::~BasicError() noexcept = default;

    template <typename charT>
    int BasicError<charT>::code() const noexcept
    {
        return code_;
    }

    template <typename charT>
    const char* BasicError<charT>::what() const noexcept
    {
        return msg_.c_str();
    }

#ifdef _MSC_VER
# pragma warning( default : 4275 )
#endif

}                                       // namespace Exiv2
#endif                                  // #ifndef ERROR_HPP_
