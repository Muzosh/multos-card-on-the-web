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

#ifndef __PROTOCOL_MULTOS_HELPER_H_
#define __PROTOCOL_MULTOS_HELPER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include <mcl/bn_c256.h>

#include "helpers/hex_helper.h"
#include "helpers/mcl_helper.h"
#include "types.h"

/**
 * Converts an array of bytes (multos fr) into the mclBnFr type.
 *
 * @param x mclBnFr data
 * @param buffer the buffer to be converted
 * @param buffer_length the length of the buffer
 * @return 0 if success else -1
 */
extern int multos_Fr_to_mcl_Fr(mclBnFr *x, const void *buffer, size_t buffer_length);

/**
 * Converts an array of bytes (multos multiplier) into the mclBnFr type.
 *
 * @param x mclBnFr data
 * @param buffer the buffer to be converted
 * @param buffer_length the length of the buffer
 * @return 0 if success else -1
 */
extern int multos_Multiplier_to_mcl_Fr(mclBnFr *x, const void *buffer, size_t buffer_length);

/**
 * Converts an array of bytes (multos point) into the mclBnG1 type.
 *
 * @param x mclBnG1 data
 * @param buffer the buffer to be converted
 * @param buffer_length the length of the buffer
 * @return 0 if success else -1
 */
extern int multos_G1_to_mcl_G1(mclBnG1 *x, const void *buffer, size_t buffer_length);

#ifdef __cplusplus
}
#endif

#endif /* __PROTOCOL_MULTOS_HELPER_H_ */
