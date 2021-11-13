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

#include "reader.h"

/**
 * Gets the stringified error response.
 *
 * @param err the error code
 * @return the stringified error response
 */
const char *sc_get_error(int32_t err)
{
    return pcsc_stringify_error(err);
}

/**
 * Gets a connection to a smart card by waiting for a working reader
 * and a card to be inserted.
 *
 * @param reader the reader used to interact with the smart card
 * @return 0 if success else an error code
 */
int32_t sc_get_card_connection(reader_t *reader)
{
    SCARD_READERSTATE rgReaderStates;

    uint32_t dwReaders, dwActiveProtocol;
    const char *mszGroups;
    char *mszReaders;

    int32_t rv;

    if (reader == NULL)
    {
        return SCARD_E_INVALID_VALUE;
    }

    rv = SCardEstablishContext(SCARD_SCOPE_SYSTEM, NULL, NULL, &reader->hContext);
    if (rv != SCARD_S_SUCCESS)
    {
        return rv;
    }

#ifndef NDEBUG
    fprintf(stdout, "[!] Please insert a working reader...\n");
#endif

    rv = SCardGetStatusChange(reader->hContext, INFINITE, 0, 0);
    if (rv != SCARD_S_SUCCESS)
    {
        SCardReleaseContext(reader->hContext);
        return rv;
    }

    mszGroups = 0;
    rv = SCardListReaders(reader->hContext, mszGroups, 0, &dwReaders);
    if (rv != SCARD_S_SUCCESS)
    {
        SCardReleaseContext(reader->hContext);
        return rv;
    }

    mszReaders = (char *) malloc(sizeof(char) * dwReaders);
    rv = SCardListReaders(reader->hContext, mszGroups, mszReaders, &dwReaders);
    if (rv != SCARD_S_SUCCESS)
    {
        SCardReleaseContext(reader->hContext);
        return rv;
    }

#ifndef NDEBUG
    fprintf(stdout, "[+] OK, using reader %s\n", &mszReaders[0]);
#endif

    rgReaderStates.szReader = &mszReaders[0];
    rgReaderStates.dwCurrentState = SCARD_STATE_EMPTY;

#ifndef NDEBUG
    fprintf(stdout, "[!] Waiting for card insertion...\n");
#endif

    rv = SCardGetStatusChange(reader->hContext, INFINITE, &rgReaderStates, 1);
    if (rv != SCARD_S_SUCCESS)
    {
        SCardReleaseContext(reader->hContext);
        return rv;
    }

    // Connect to the card
    rv = SCardConnect(reader->hContext, &mszReaders[0], SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0, &reader->hCard, &dwActiveProtocol);
    if (rv != SCARD_S_SUCCESS)
    {
        SCardReleaseContext(reader->hContext);
        return rv;
    }

#ifndef NDEBUG
    fprintf(stdout, "[+] OK, connected!\n\n");
#endif

    return 0;
}

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
int32_t sc_transmit_data(reader_t reader, const uint8_t *pbSendBuffer, uint32_t dwSendLength, uint8_t *pbRecvBuffer, uint32_t *dwRecvLength, double *elapsed_time)
{
    struct timespec ts_start = {0, 0};
    struct timespec ts_end = {0, 0};

    const SCARD_IO_REQUEST *pioSendPci;
    SCARD_IO_REQUEST pioRecvPci;

#ifndef NDEBUG
    uint32_t i;
#endif
    int32_t rv;

    // Send data to select the app
    pioSendPci = SCARD_PCI_T0;
    pioRecvPci.dwProtocol = SCARD_PROTOCOL_T0;

#ifndef NDEBUG
    fprintf(stdout, "[+] SCard request :\n");
    for (i = 0; i < dwSendLength; i++)
    {
        fprintf(stdout, "%02X ", pbSendBuffer[i]);
    }
    fprintf(stdout, "\n");
#endif

    // Exchange APDU message
    clock_gettime(CLOCK_MONOTONIC, &ts_start);
    rv = SCardTransmit(reader.hCard, pioSendPci, (const unsigned char *) pbSendBuffer, dwSendLength, &pioRecvPci, (unsigned char *) pbRecvBuffer, dwRecvLength);
    clock_gettime(CLOCK_MONOTONIC, &ts_end);
    if (rv != SCARD_S_SUCCESS)
    {
        SCardDisconnect(reader.hCard, SCARD_RESET_CARD);
        SCardReleaseContext(reader.hContext);
        return rv;
    }

    // calculate the elapsed time if requested
    if (elapsed_time != NULL)
    {
        *elapsed_time = ((double) ts_end.tv_sec + 1.0e-9 * (double) ts_end.tv_nsec) - ((double) ts_start.tv_sec + 1.0e-9 * (double) ts_start.tv_nsec);
    }

#ifndef NDEBUG
    fprintf(stdout, "[+] SCard response:\n");
    for (i = 0; i < *dwRecvLength; i++)
    {
        fprintf(stdout, "%02X ", pbRecvBuffer[i]);
    }
    fprintf(stdout, "\n\n");
#endif

    return 0;
}

/**
 * Cleans the reader data.
 *
 * @param reader the reader used to interact with the smart card
 */
void sc_cleanup(reader_t reader)
{
    SCardDisconnect(reader.hCard, SCARD_RESET_CARD);
    SCardReleaseContext(reader.hContext);
}