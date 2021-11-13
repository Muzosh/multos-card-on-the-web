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

#ifndef __MULTOS_APDU_H_
#define __MULTOS_APDU_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define SW_LENGTH               2
#define COMMAND_HEADER_SIZE     6

#define MAX_APDU_LENGTH_T0      256
#define MAX_APDU_LENGTH_T1      65536

#define MAX_APDU_SEND_SIZE_T0   (MAX_APDU_LENGTH_T0 - COMMAND_HEADER_SIZE)
#define MAX_APDU_RECV_SIZE_T0   (MAX_APDU_LENGTH_T0 - SW_LENGTH)

#define MAX_APDU_SEND_SIZE_T1   (MAX_APDU_LENGTH_T1 - COMMAND_HEADER_SIZE)
#define MAX_APDU_RECV_SIZE_T1   (MAX_APDU_LENGTH_T1 - SW_LENGTH)

#define CUSTOM_SW_EXPECTED_ADDITIONAL_DATA              0x91AF

#define CLA_APPLICATION                                 0x80

#define INS_HELLO_WORLD                                 0x01

#ifdef __cplusplus
}
#endif

#endif /* __MULTOS_APDU_H_ */
