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

#include "command.h"

/**
 * Builds a valid APDU command to transmit to the smart card.
 *
 * @param apdu_iso_case APDU ISO case
 * @param cla class byte
 * @param ins instruction byte
 * @param p1 parameter byte 1
 * @param p2 parameter byte 2
 * @param lc length of command data to be sent
 * @param data command data of length lc
 * @param le the length of data expected to be returned after processing the command
 * @param pbSendBuffer buffer where the APDU command will be stored
 * @param dwSendLength total length of the APDU command
 * @return 0 if success else -1
 */
int apdu_build_command(apdu_iso_case_t apdu_iso_case, uint8_t cla, uint8_t ins, uint8_t p1, uint8_t p2, uint8_t lc, uint8_t *data, uint8_t le, uint8_t *pbSendBuffer, uint32_t *dwSendLength)
{
    if (pbSendBuffer == NULL || *dwSendLength != MAX_APDU_LENGTH_T0)
    {
        return -1;
    }

    if (data != NULL && lc > MAX_APDU_SEND_SIZE_T0) // not enough memory
    {
        return -1;
    }

    switch (apdu_iso_case)
    {
        case CASE1:
        {
            pbSendBuffer[0] = cla;
            pbSendBuffer[1] = ins;
            pbSendBuffer[2] = p1;
            pbSendBuffer[3] = p2;
            *dwSendLength = 4;
            break;
        }
        case CASE2:
        {
            pbSendBuffer[0] = cla;
            pbSendBuffer[1] = ins;
            pbSendBuffer[2] = p1;
            pbSendBuffer[3] = p2;
            pbSendBuffer[4] = le;
            *dwSendLength = 5;
            break;
        }
        case CASE3:
        {
            pbSendBuffer[0] = cla;
            pbSendBuffer[1] = ins;
            pbSendBuffer[2] = p1;
            pbSendBuffer[3] = p2;
            pbSendBuffer[4] = lc;
            memcpy(&pbSendBuffer[5], data, lc);
            *dwSendLength = 5 + lc;
            break;
        }
        case CASE4:
        {
            pbSendBuffer[0] = cla;
            pbSendBuffer[1] = ins;
            pbSendBuffer[2] = p1;
            pbSendBuffer[3] = p2;
            pbSendBuffer[4] = lc;
            memcpy(&pbSendBuffer[5], data, lc);
            pbSendBuffer[5 + lc] = le;
            *dwSendLength = 5 + lc + 1;
            break;
        }
        default:
        {
            *dwSendLength = 0;
            break;
        }
    }

    return 0;
}
