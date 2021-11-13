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

#include "multos_helper.h"

/**
 * Converts an array of bytes (multos fr) into the mclBnFr type.
 *
 * @param x mclBnFr data
 * @param buffer the buffer to be converted
 * @param buffer_length the length of the buffer
 * @return 0 if success else -1
 */
int multos_Fr_to_mcl_Fr(mclBnFr *x, const void *buffer, size_t buffer_length)
{
    if (x == NULL || buffer == NULL || buffer_length != sizeof(elliptic_curve_fr_t))
    {
        return -1;
    }

    return mcl_bytes_to_Fr(x, buffer, sizeof(elliptic_curve_fr_t));
}

/**
 * Converts an array of bytes (multos multiplier) into the mclBnFr type.
 *
 * @param x mclBnFr data
 * @param buffer the buffer to be converted
 * @param buffer_length the length of the buffer
 * @return 0 if success else -1
 */
int multos_Multiplier_to_mcl_Fr(mclBnFr *x, const void *buffer, size_t buffer_length)
{
    if (x == NULL || buffer == NULL || buffer_length != sizeof(elliptic_curve_multiplier_t))
    {
        return -1;
    }

    // use only the ecm value
    return mcl_bytes_to_Fr(x, (const void *) &((elliptic_curve_multiplier_t *) buffer)->ecm, sizeof(elliptic_curve_fr_t));
}

/**
 * Converts an array of bytes (multos point) into the mclBnG1 type.
 *
 * @param x mclBnG1 data
 * @param buffer the buffer to be converted
 * @param buffer_length the length of the buffer
 * @return 0 if success else -1
 */
int multos_G1_to_mcl_G1(mclBnG1 *x, const void *buffer, size_t buffer_length)
{
    char x_coordinate[65] = {0};
    char y_coordinate[65] = {0};

    char data[192] = {0};
    mclSize data_length;

    int r;

    if (x == NULL || buffer == NULL || buffer_length != sizeof(elliptic_curve_point_t))
    {
        return -1;
    }

    // convert x and y coordinates to raw - affine space
    mem2hex(x_coordinate, (unsigned char *) &((elliptic_curve_point_t *) buffer)->x, sizeof(elliptic_curve_fp_t)); // hex x coordinate
    mem2hex(y_coordinate, (unsigned char *) &((elliptic_curve_point_t *) buffer)->y, sizeof(elliptic_curve_fp_t)); // hex y coordinate

    // mcl point format
    data_length = snprintf(data, 192, "1 %s %s", x_coordinate, y_coordinate);

    // mcl point conversion
    r = mclBnG1_setStr(x, (const char *) data, data_length, 16);
    if (r < 0)
    {
        return -1;
    }

    return 0;
}
