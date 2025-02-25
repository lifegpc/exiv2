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

#include "image_int.hpp"

#include <cstdarg>
#include <cstddef>
#include <cstring>
#include <vector>
#include <cstdio>

namespace Exiv2
{
    namespace Internal
    {
        std::string stringFormat(const char* format, ...)
        {
            std::string result;
            std::vector<char> buffer;
            size_t need = std::strlen(format)*8;  // initial guess
            int rc = -1;

            // vsnprintf writes at most size (2nd parameter) bytes (including \0)
            //           returns the number of bytes required for the formatted string excluding \0
            // the following loop goes through:
            // one iteration (if 'need' was large enough for the for formatted string)
            // or two iterations (after the first call to vsnprintf we know the required length)
            do {
                buffer.resize(need + 1);
                va_list args;            // variable arg list
                va_start(args, format);  // args start after format
                rc = vsnprintf(&buffer[0], buffer.size(), format, args);
                va_end(args);     // free the args
                assert(rc >= 0);  // rc < 0 => we have made an error in the format string
                if (rc > 0)
                    need = static_cast<size_t>(rc);
            } while (buffer.size() <= need);

            if (rc > 0)
                result = std::string(&buffer[0], need);
            return result;
        }

        std::string indent(int32_t d)
        {
            std::string result;
            if (d > 0)
                while (d--)
                    result += "  ";
            return result;
        }

    }  // namespace Internal

}  // namespace Exiv2
