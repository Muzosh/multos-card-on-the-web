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

#ifndef __PROTOCOL_MCL_HELPER_H_
#define __PROTOCOL_MCL_HELPER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <ctype.h>

#include <stddef.h>
#include <string.h>
#include <assert.h>

#include <mcl/bn_c256.h>

#include "helpers/hex_helper.h"
#include "types.h"

/**
 * Displays the contents of buffer.
 *
 * @param name text to be displayed before the data
 * @param buffer the buffer containing the data
 * @param buffer_length the length of the buffer
 */
extern void mcl_display_bytes(const char *name, const void *buffer, size_t buffer_length);

/**
 * Displays the contents of mclBnFr.
 *
 * @param name text to be displayed before the data
 * @param x mclBnFr data
 */
extern void mcl_display_Fr(const char *name, mclBnFr x);

/**
 * Displays the contents of mclBnG1.
 *
 * @param name text to be displayed before the data
 * @param x mclBnG1 data
 */
extern void mcl_display_G1(const char *name, mclBnG1 x);

/**
 * Converts an array of bytes into the mclBnFr type.
 *
 * @param x mclBnFr data
 * @param buffer the buffer to be converted
 * @param buffer_length the length of the buffer
 * @return 0 if success else -1
 */
extern int mcl_bytes_to_Fr(mclBnFr *x, const void *buffer, size_t buffer_length);

/**
 * Converts the mclBnFr type into an array of bytes.
 *
 * @param buffer the buffer where the conversion will be stored
 * @param buffer_length the length of the buffer
 * @param x mclBnFr data
 * @return 0 if success else -1
 */
extern int mcl_Fr_to_bytes(void *buffer, size_t buffer_length, mclBnFr x);

/**
 * Converts the mclBnFr type into an array of bytes (multos fr).
 *
 * @param buffer the buffer where the conversion will be stored
 * @param buffer_length the length of the buffer
 * @param x mclBnFr data
 * @return 0 if success else -1
 */
extern int mcl_Fr_to_multos_Fr(void *buffer, size_t buffer_length, mclBnFr x);

/**
 * Converts the mclBnFr type into an array of bytes (multos multiplier).
 *
 * @param buffer the buffer where the conversion will be stored
 * @param buffer_length the length of the buffer
 * @param x mclBnFr data
 * @return 0 if success else -1
 */
extern int mcl_Fr_to_multos_Multiplier(void *buffer, size_t buffer_length, mclBnFr x);

/**
 * Converts the mclBnG1 type into an array of bytes (multos point).
 *
 * @param buffer the buffer where the conversion will be stored
 * @param buffer_length the length of the buffer
 * @param x mclBnG1 data
 * @return 0 if success else -1
 */
extern int mcl_G1_to_multos_G1(void *buffer, size_t buffer_length, mclBnG1 x);

#ifdef __cplusplus
}
#endif

#endif /* __PROTOCOL_MCL_HELPER_H_ */
