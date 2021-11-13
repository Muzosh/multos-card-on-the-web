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

#include "mcl_helper.h"

/**
 * Displays the contents of buffer.
 *
 * @param name text to be displayed before the data
 * @param buffer the buffer containing the data
 * @param buffer_length the length of the buffer
 */
void mcl_display_bytes(const char *name, const void *buffer, size_t buffer_length)
{
    size_t it;

    fprintf(stdout, "%-15s: ", name != NULL ? name : "");

    for (it = 0; it < buffer_length - 1; it++)
    {
        fprintf(stdout, "%02x ", ((uint8_t *) buffer)[it]);
    }
    fprintf(stdout, "%02x\n", ((uint8_t *) buffer)[it]);
}

/**
 * Displays the contents of mclBnFr.
 *
 * @param name text to be displayed before the data
 * @param x mclBnFr data
 */
void mcl_display_Fr(const char *name, mclBnFr x)
{
    char data[128] = {0};
    size_t data_length;
    size_t it;

    fprintf(stdout, "%-15s: ", name != NULL ? name : "");

    data_length = mclBnFr_getStr(data, 128, &x, 16);
    for (it = 0; it < data_length; it++)
    {
        fprintf(stdout, "%c", toupper(data[it]));
    }
    fprintf(stdout, "%c\n", toupper(data[it]));
}

/**
 * Displays the contents of mclBnG1.
 *
 * @param name text to be displayed before the data
 * @param x mclBnG1 data
 */
void mcl_display_G1(const char *name, mclBnG1 x)
{
    char data[256] = {0};
    size_t data_length;
    size_t it;

    fprintf(stdout, "%-15s: ", name != NULL ? name : "");

    data_length = mclBnG1_getStr(data, 256, &x, 16);
    for (it = 0; it < data_length; it++)
    {
        fprintf(stdout, "%c", toupper(data[it]));
    }
    fprintf(stdout, "%c\n", toupper(data[it]));
}

/**
 * Converts an array of bytes into the mclBnFr type.
 *
 * @param x mclBnFr data
 * @param buffer the buffer to be converted
 * @param buffer_length the length of the buffer
 * @return 0 if success else -1
 */
int mcl_bytes_to_Fr(mclBnFr *x, const void *buffer, size_t buffer_length)
{
    char fr_value[65] = {0};

    char data[65] = {0};
    mclSize data_length;

    int r;

    if (x == NULL || buffer == NULL || buffer_length != sizeof(mclBnFr))
    {
        return -1;
    }

    // fr to hex
    mem2hex(fr_value, buffer, sizeof(mclBnFr));

    // mcl fr format
    data_length = snprintf(data, 65, "%s", fr_value);

    // mcl fr conversion
    r = mclBnFr_setStr(x, (const char *) data, data_length, 16);
    if (r < 0)
    {
        return -1;
    }

    return 0;
}

/**
 * Converts the mclBnFr type into an array of bytes.
 *
 * @param buffer the buffer where the conversion will be stored
 * @param buffer_length the length of the buffer
 * @param x mclBnFr data
 * @return 0 if success else -1
 */
int mcl_Fr_to_bytes(void *buffer, size_t buffer_length, mclBnFr x)
{
    char fr_value[65] = {0};

    char data[65] = {0};
    mclSize data_length;

    int r;

    if (buffer == NULL || buffer_length != sizeof(mclBnFr))
    {
        return -1;
    }

    // mcl fr conversion
    r = mclBnFr_getStr(data, sizeof(data), &x, 16);
    if (r < 0)
    {
        return -1;
    }

    // fr value
    data_length = strnlen(data, 64);
    if (data_length < 64)
        snprintf(fr_value, 65, "%0*d%s", (int) (64 - data_length), 0, data);
    else
        snprintf(fr_value, 65, "%s", data);

    hex2mem(buffer, (const char *) fr_value, sizeof(elliptic_curve_fr_t)); // hex fr value

    return 0;
}

/**
 * Converts the mclBnFr type into an array of bytes (multos fr).
 *
 * @param buffer the buffer where the conversion will be stored
 * @param buffer_length the length of the buffer
 * @param x mclBnFr data
 * @return 0 if success else -1
 */
int mcl_Fr_to_multos_Fr(void *buffer, size_t buffer_length, mclBnFr x)
{
    if (buffer == NULL || buffer_length != sizeof(elliptic_curve_fr_t))
    {
        return -1;
    }

    return mcl_Fr_to_bytes(buffer, sizeof(elliptic_curve_fr_t), x);
}

/**
 * Converts the mclBnFr type into an array of bytes (multos multiplier).
 *
 * @param buffer the buffer where the conversion will be stored
 * @param buffer_length the length of the buffer
 * @param x mclBnFr data
 * @return 0 if success else -1
 */
int mcl_Fr_to_multos_Multiplier(void *buffer, size_t buffer_length, mclBnFr x)
{
    if (buffer == NULL || buffer_length != sizeof(elliptic_curve_multiplier_t))
    {
        return -1;
    }

    ((elliptic_curve_multiplier_t *) buffer)->x = 0x00; // multiplier
    return mcl_Fr_to_multos_Fr((void *) &((elliptic_curve_multiplier_t *) buffer)->ecm, sizeof(elliptic_curve_fr_t), x);
}

/**
 * Converts the mclBnG1 type into an array of bytes (multos point).
 *
 * @param buffer the buffer where the conversion will be stored
 * @param buffer_length the length of the buffer
 * @param x mclBnG1 data
 * @return 0 if success else -1
 */
int mcl_G1_to_multos_G1(void *buffer, size_t buffer_length, mclBnG1 x)
{
    char x_coordinate[65] = {0};
    char y_coordinate[65] = {0};

    char data[192] = {0};

    char *token;
    size_t token_length;

    int r;

    if (buffer == NULL || buffer_length != sizeof(elliptic_curve_point_t))
    {
        return -1;
    }

    // mcl point conversion
    r = mclBnG1_getStr(data, sizeof(data), &x, 16);
    if (r < 0)
    {
        return -1;
    }

    // 1 (affine coordinate)
    token = strtok(data, " ");
    assert(token != NULL);

    // x coordinate
    token = strtok(NULL, " ");
    token_length = strnlen(token, 64);
    if (token_length < 64)
        snprintf(x_coordinate, 65, "%0*d%s", (int) (64 - token_length), 0, token);
    else
        snprintf(x_coordinate, 65, "%s", token);

    // y coordinate
    token = strtok(NULL, " ");
    token_length = strnlen(token, 64);
    if (token_length < 64)
        snprintf(y_coordinate, 65, "%0*d%s", (int) (64 - token_length), 0, token);
    else
        snprintf(y_coordinate, 65, "%s", token);

    ((elliptic_curve_point_t *) buffer)->form = 0x04; // affine space
    hex2mem((unsigned char *) &((elliptic_curve_point_t *) buffer)->x, (const char *) x_coordinate, sizeof(elliptic_curve_fp_t)); // hex x coordinate
    hex2mem((unsigned char *) &((elliptic_curve_point_t *) buffer)->y, (const char *) y_coordinate, sizeof(elliptic_curve_fp_t)); // hex y coordinate

    return 0;
}
