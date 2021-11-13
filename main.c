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

#include <stdio.h>

#include "system.h"
#include "setup.h"

#if defined (PROTOCOL_MULTOS)
# include "multos/apdu.h"
# include "pcsc/reader.h"
# include "controllers/multos/user.h"
#else
# include "controllers/user.h"
#endif

int main(int argc, char *argv[])
{
    system_par_t sys_parameters = {0};

    user_identifier_t ue_identifier = {0};

    int r;

#if defined (PROTOCOL_MULTOS)
    uint8_t pbRecvBuffer[MAX_APDU_LENGTH_T0] = {0};
    uint32_t dwRecvLength;
    reader_t reader;

    r = sc_get_card_connection(&reader);
    if (r < 0)
    {
        fprintf(stderr, "Error: %s\n", sc_get_error(r));
        return 1;
    }

# ifndef NDEBUG
    fprintf(stdout, "[-] Command: APDU_SCARD_SELECT_APPLICATION\n");
# endif

    dwRecvLength = sizeof(pbRecvBuffer);
    r = sc_transmit_data(reader, APDU_SCARD_SELECT_APPLICATION, sizeof(APDU_SCARD_SELECT_APPLICATION), pbRecvBuffer, &dwRecvLength, NULL);
    if (r < 0)
    {
        fprintf(stderr, "Error: %s\n", sc_get_error(r));
        return 1;
    }
#else
    reader_t reader = NULL;
#endif

    // system - setup
    r = sys_setup(&sys_parameters);
    if (r < 0)
    {
        fprintf(stderr, "Error: cannot initialize the system!\n");
        return 1;
    }

    // user - get user identifier
    r = ue_get_user_identifier(reader, &ue_identifier);
    if (r < 0)
    {
        fprintf(stderr, "Error: cannot get the user identifier!\n");
        return 1;
    }

#if defined (PROTOCOL_MULTOS)
    sc_cleanup(reader);
#endif

    fprintf(stdout, "OK!\n");

    return 0;
}
