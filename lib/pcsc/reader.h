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

#ifndef __PROTOCOL_PCSC_READER_H_
#define __PROTOCOL_PCSC_READER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NDEBUG
# include <stdio.h>
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <time.h>

#include <PCSC/pcsclite.h>
#include <PCSC/winscard.h>

typedef struct
{
    SCARDHANDLE hCard;
    SCARDCONTEXT hContext;
} reader_t;

/**
 * Gets the stringified error response.
 *
 * @param err the error code
 * @return the stringified error response
 */
extern const char *sc_get_error(int32_t err);

/**
 * Gets a connection to a smart card by waiting for a working reader
 * and a card to be inserted.
 *
 * @param reader the reader used to interact with the smart card
 * @return 0 if success else an error code
 */
extern int32_t sc_get_card_connection(reader_t *reader);

/**
 * Sends data to the smart card and gets the response.
 *
 * @param reader the reader used to interact with the smart card
 * @param pbSendBuffer the buffer for sending data
 * @param dwSendLength the length of the buffer for sending data
 * @param pbRecvBuffer the buffer for receiving data
 * @param dwRecvLength the length of the buffer for receiving data
 * @param elapsed_time the elapsed time if requested
 * @return 0 if success else an error code
 */
extern int32_t sc_transmit_data(reader_t reader, const uint8_t *pbSendBuffer, uint32_t dwSendLength, uint8_t *pbRecvBuffer, uint32_t *dwRecvLength, double *elapsed_time);

/**
 * Cleans the reader data.
 *
 * @param reader the reader used to interact with the smart card
 */
extern void sc_cleanup(reader_t reader);

#ifdef __cplusplus
}
#endif

#endif /* __PROTOCOL_PCSC_READER_H_ */
