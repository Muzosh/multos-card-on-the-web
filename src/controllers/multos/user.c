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
    uint8_t pbSendBuffer[MAX_APDU_LENGTH_T0] = {0};
    uint8_t pbRecvBuffer[MAX_APDU_LENGTH_T0] = {0};
    uint32_t dwSendLength;
    uint32_t dwRecvLength;

    int r;

    if (identifier == NULL)
    {
        return -1;
    }

    dwSendLength = sizeof(pbSendBuffer);
    r = apdu_build_command(CASE2, CLA_APPLICATION, INS_HELLO_WORLD, 0x00, 0x00, 0, NULL, USER_MAX_ID_LENGTH, pbSendBuffer, &dwSendLength);
    if (r < 0)
    {
        return -1;
    }

    dwRecvLength = sizeof(pbRecvBuffer);
    r = sc_transmit_data(reader, pbSendBuffer, dwSendLength, pbRecvBuffer, &dwRecvLength, NULL);
    if (r < 0)
    {
        fprintf(stderr, "Error: %s\n", sc_get_error(r));
        return r;
    }

    memcpy(identifier->buffer, pbRecvBuffer, USER_MAX_ID_LENGTH);
    identifier->buffer_length = USER_MAX_ID_LENGTH;

    return 0;
}
