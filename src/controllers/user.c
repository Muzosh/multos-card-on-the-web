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

#include "user.h"

/**
 * Gets the user identifier using the specified reader.
 *
 * @param reader the reader to be used
 * @param identifier the user identifier
 * @return 0 if success else -1
 */
int ue_get_user_identifier(reader_t reader, user_identifier_t *identifier)
{
    if (identifier == NULL)
    {
        return -1;
    }

    memcpy(identifier->buffer, (uint8_t[]) {
            0x02, 0x0F, 0x84, 0x31, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0x1A, 0x2B, 0x3C, 0x4D,
            0x5E, 0x6F, 0x00, 0x40, 0x00, 0xFF, 0x01
    }, USER_MAX_ID_LENGTH);
    identifier->buffer_length = USER_MAX_ID_LENGTH;

    return 0;
}
