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

#ifndef __PROTOCOL_TYPES_H_
#define __PROTOCOL_TYPES_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#include "config/config.h"

/*
 * Fp
 */
typedef struct
{
    uint8_t d[EC_SIZE]; // 32B
} elliptic_curve_fp_t;

/*
 * G1 is isomorphism to Fr
 */
typedef struct
{
    uint8_t d[EC_SIZE]; // 32B
} elliptic_curve_fr_t;

/*
 * G1 is defined over Fp
 */
typedef struct
{
    uint8_t form;          // 1B
    elliptic_curve_fp_t x; // 32B
    elliptic_curve_fp_t y; // 32B
} elliptic_curve_point_t;

/*
 * Structure of EC multiplier
 */
typedef struct
{
    uint8_t x;               // 1B
    elliptic_curve_fr_t ecm; // 32B
} elliptic_curve_multiplier_t;

#ifdef __cplusplus
}
#endif

#endif /* __PROTOCOL_TYPES_H_ */
