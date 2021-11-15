/**
 *
 *  Copyright (C) 2021  Petr Muzikant
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

#pragma attribute("aid", "f0 00 00 01")
#pragma attribute("dir", "61 0d 4f 04 f0 00 00 01 50 05 68 77 61 70 70")

#define CLA_APP 0x80
#define INS_GET_RESPONSE 0x10
#define INS_SET_KEY 0x20
#define INS_RESET_KEY 0x30
#define INS_GET_CURRENT_KEY 0x40

#define APDU_L_MAX 256
// #define SHA_DIGEST_LENGTH 20
// #define SHA_DIGEST_PADDING 1,

#define HASHSIZE 20
#define KEYSIZE 12
#define CHALLENGE_SIZE 52
// #define DESBLOCK 8
// #define MSGSIZE 255
// #define MODLEN 128
// #define EXPLEN 2
// #define BLOCK 128
// #define PADSZ 8

// ISO Case Definitions
#define NODATAIN_NODATAOUT    1
#define NODATAIN_DATAOUT      2
#define DATAIN_NODATAOUT      3
#define DATAIN_DATAOUT        4

// ISO codes
#include <ISO7816.h>
// SmartDeck comms support
#include <multoscomms.h>
// SmartDeck crypto support
#include <multoscrypto.h>

// Standard libraries
#include <string.h>
#include <stdint.h>


/// Global values - RAM (Public memory)
#pragma melpublic
unsigned char apdu_data[APDU_L_MAX];

/// Session values - RAM (Dynamic memory)
#pragma melsession
unsigned char hash_result[KEYSIZE+CHALLENGE_SIZE];

/// Static values - EEPROM (Static memory)
#pragma melstatic
// 12 bytes = 12 characters
unsigned char current_key[KEYSIZE] = {0x70, 0x61, 0x73, 0x73, 0x77, 0x6F, 0x72, 0x64, 0x31, 0x32, 0x33, 0x34};
// 12 bytes = 12 characters
unsigned char default_key[KEYSIZE] = {0x70, 0x61, 0x73, 0x73, 0x77, 0x6F, 0x72, 0x64, 0x31, 0x32, 0x33, 0x34};

void main(void)
{
    if (CLA != CLA_APP)
        ExitSW(ISO7816_SW_CLA_NOT_SUPPORTED);

    switch (INS)
    {
    case INS_GET_RESPONSE:
        if (!CheckCase(DATAIN_DATAOUT))
            ExitSW(ISO7816_SW_CONDITIONS_NOT_SATISFIED);
        if (Lc != CHALLENGE_SIZE)
            ExitSW(ISO7816_SW_WRONG_LENGTH);
        // if (Le != HASHSIZE)
        //     ExitSW(ISO7816_SW_WRONG_LENGTH);

        // TODO: add current key concat
        SHA1(Lc, apdu_data, apdu_data);

        // ExitSWLa(ISO7816_SW_NO_ERROR, HASHSIZE);
        ExitLa(HASHSIZE);
        break;

    case INS_SET_KEY:
        if (!CheckCase(DATAIN_NODATAOUT))
            ExitSW(ISO7816_SW_CONDITIONS_NOT_SATISFIED);
        if (Lc != KEYSIZE)
            ExitSW(ISO7816_SW_WRONG_LENGTH);
        memcpy(current_key, apdu_data, KEYSIZE);
        ExitSW(ISO7816_SW_NO_ERROR);
        break;

    case INS_RESET_KEY:
        if (!CheckCase(NODATAIN_NODATAOUT))
            ExitSW(ISO7816_SW_CONDITIONS_NOT_SATISFIED);
        memcpy(current_key, default_key, KEYSIZE);
        ExitSW(ISO7816_SW_NO_ERROR);
        break;

    case INS_GET_CURRENT_KEY:
        if (!CheckCase(NODATAIN_DATAOUT))
            ExitSW(ISO7816_SW_CONDITIONS_NOT_SATISFIED);
        if (Le != KEYSIZE)
            ExitSW(ISO7816_SW_WRONG_LENGTH);
        memcpy(apdu_data, current_key, KEYSIZE);
        ExitSWLa(ISO7816_SW_NO_ERROR, KEYSIZE);
        break;


    default:
        ExitSW(ISO7816_SW_INS_NOT_SUPPORTED);
    }
}
