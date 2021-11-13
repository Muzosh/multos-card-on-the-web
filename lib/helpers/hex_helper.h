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

#ifndef __PROTOCOL_HEX_HELPER_H_
#define __PROTOCOL_HEX_HELPER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <ctype.h>
#include <stddef.h>

/**
 * Converts memory bytes into a hexadecimal string.
 *
 * @param s1 pointer to the destination array where the content is to be converted
 * @param s2 pointer to the source of data to be converted
 * @param n number of bytes to convert
 * @return pointer to the destination
 */
extern char *mem2hex(char *s1, const unsigned char *s2, size_t n);

/**
 * Converts a hexadecimal string into memory bytes.
 *
 * @param s1 pointer to the destination array where the content is to be converted
 * @param s2 pointer to the source of data to be converted
 * @param n number of bytes to convert
 * @return pointer to the destination
 */
extern unsigned char *hex2mem(unsigned char *s1, const char *s2, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* __PROTOCOL_HEX_HELPER_H_ */
