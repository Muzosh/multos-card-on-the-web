/**
 *
 *  Copyright (C) 2020  Raul Casanova Marques
 *
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "hex_helper.h"

/**
 * Converts memory bytes into a hexadecimal string.
 *
 * @param s1 pointer to the destination array where the content is to be converted
 * @param s2 pointer to the source of data to be converted
 * @param n number of bytes to convert
 * @return pointer to the destination
 */
char *mem2hex(char *s1, const unsigned char *s2, size_t n)
{
    register int i, j;
    for (i = 0, j = 0; i < n; ++i, j += 2)
    {
        sprintf(s1 + j, "%02x", s2[i] & 0xffu);
    }
    return s1;
}

/**
 * Converts a hexadecimal string into memory bytes.
 *
 * @param s1 pointer to the destination array where the content is to be converted
 * @param s2 pointer to the source of data to be converted
 * @param n number of bytes to convert
 * @return pointer to the destination
 */
unsigned char *hex2mem(unsigned char *s1, const char *s2, size_t n)
{
    register unsigned char *ret = s1;
    while (n)
    {
        if (*s2 >= '0' && *s2 <= '9')
            *s1 = (*s2 - 48) * 16;
        else // *s2 >= 'A' && *s2 <= 'F'
            *s1 = ((unsigned char) toupper(*s2) - 55) * 16;
        s2++;

        if (*s2 >= '0' && *s2 <= '9')
            *s1++ += *s2 - 48;
        else // *s2 >= 'A' && *s2 <= 'F'
            *s1++ += (unsigned char) toupper(*s2) - 55;
        s2++; n--;
    }
    return ret;
}
