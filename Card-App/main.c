/*
MIT License

Copyright (c) 2022 Petr Muzikant

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma attribute("aid", "f0 00 00 01")
#pragma attribute("dir", "61 0d 4f 04 f0 00 00 01 50 05 68 77 61 70 70")

#define CLA_APP 0x80
#define INS_AUTHENTICATE 0x10
#define INS_SET_KEY 0x20
#define INS_RESET_KEY 0x30
#define INS_GET_CURRENT_KEY 0x40

#define APDU_L_MAX 256

#define HASHSIZE 20
#define KEYSIZE 12
#define CHALLENGE_SIZE 52

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
unsigned char hash_input[KEYSIZE+CHALLENGE_SIZE];

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
    case INS_AUTHENTICATE:
        if (!CheckCase(DATAIN_DATAOUT))
            ExitSW(ISO7816_SW_CONDITIONS_NOT_SATISFIED);
        if (Lc != CHALLENGE_SIZE)
            ExitSW(ISO7816_SW_WRONG_LENGTH);
        // if (Le != HASHSIZE)
        //     ExitSW(ISO7816_SW_WRONG_LENGTH);

        memcpy(hash_input, apdu_data, CHALLENGE_SIZE);
        memcpy(hash_input+CHALLENGE_SIZE, current_key, KEYSIZE);
        SHA1(Lc+KEYSIZE, apdu_data, hash_input);

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
