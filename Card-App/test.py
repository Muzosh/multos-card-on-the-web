"""
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
"""

from smartcard.CardType import AnyCardType
from smartcard.CardRequest import CardRequest
from smartcard.CardConnectionObserver import CardConnectionObserver
from smartcard.util import toHexString
import secrets, hashlib
from more_itertools import sliced


class ConsoleCardConnectionObserver(CardConnectionObserver):
    """This observer will interprer SELECT and GET RESPONSE bytes
    and replace them with a human readable string."""

    def update(self, cardconnection, ccevent):

        if "connect" == ccevent.type:
            print(("connecting to " + cardconnection.getReader()))

        elif "disconnect" == ccevent.type:
            print(("disconnecting from " + cardconnection.getReader()))

        elif "command" == ccevent.type:
            str = toHexString(ccevent.args[0])
            print((">", str))

        elif "response" == ccevent.type:
            if [] == ccevent.args[0]:
                print(("<  []", "%-2X %-2X" % tuple(ccevent.args[-2:])))
            else:
                print(
                    (
                        "<",
                        toHexString(ccevent.args[0]),
                        "%-2X %-2X" % tuple(ccevent.args[-2:]),
                    )
                )


# define keys - 12 ASCII characters in hexadecimal
default_key = [
    0x70,
    0x61,
    0x73,
    0x73,
    0x77,
    0x6F,
    0x72,
    0x64,
    0x31,
    0x32,
    0x33,
    0x34,
]
new_key_to_set = [
    0x63,
    0x69,
    0x73,
    0x63,
    0x6F,
    0x32,
    0x30,
    0x32,
    0x31,
    0x61,
    0x62,
    0x63,
]

# random 52 bytes challenge
challenge_len = 52
challenge = [int(x, 16) for x in sliced(secrets.token_hex(challenge_len), 2)]


# define the apdus used in this script
#                    CLA   INS   P1    P2    Lc    AID1  AID2  AID3  AID4  Le
SELECTAPP = [0x00, 0xA4, 0x04, 0x00, 0x04, 0xF0, 0x00, 0x00, 0x01, 0x00]
#                    CLA   INS   P1    P2    Le
GET_CURRENT_KEY = [0x80, 0x40, 0x00, 0x00, 0x0C]
#                    CLA   INS   P1    P2    Lc    Data
SET_KEY = [0x80, 0x20, 0x00, 0x00, 0x0C] + new_key_to_set
#                    CLA   INS   P1    P2
RESET_KEY = [0x80, 0x30, 0x00, 0x00]
#                    CLA   INS   P1    P2    Lc               challenge    Le
GET_HASH = [0x80, 0x10, 0x00, 0x00, challenge_len] + challenge + [0x20]
#                    CLA   INS   P1    P2    Le
GET_RESPONSE = [0x00, 0xC0, 0x00, 0x00, 0x14]


# we request any type and wait for 10s for card insertion
cardrequest = CardRequest(timeout=10, cardType=AnyCardType())
cardservice = cardrequest.waitforcard()
conn = cardservice.connection

# create an instance of our observer and attach to the connection
conn.addObserver(ConsoleCardConnectionObserver())

# connect and send APDUs
# the observer will trace on the console
conn.connect()

print("Select application")
response, sw1, sw2 = conn.transmit(SELECTAPP)

print("Reset key for testing")
response, sw1, sw2 = conn.transmit(RESET_KEY)

print("Get current key and ensure it's the same as default key")
response, sw1, sw2 = conn.transmit(GET_CURRENT_KEY)
assert response == default_key

print("Set new key, get current and ensure it's the same as newly set key")
response, sw1, sw2 = conn.transmit(SET_KEY)
response, sw1, sw2 = conn.transmit(GET_CURRENT_KEY)
assert response == new_key_to_set

print("Reset key to default and ensure it's the same as current key")
response, sw1, sw2 = conn.transmit(RESET_KEY)
response, sw1, sw2 = conn.transmit(GET_CURRENT_KEY)
assert response == default_key

# First, try to obtain response with challenge, if sw1 is 97, it means there
# are data in the buffer
print("Trying challenge-response protocol")
if conn.transmit(GET_HASH)[1] == 97:
    # we can obtain these data with GET_RESPONSE apdu
    response, sw1, sw2 = conn.transmit(GET_RESPONSE)

    # compute our own sha1
    hash = hashlib.sha1()
    hash.update(bytes(challenge + default_key))
    result = list(hash.digest())

    # compare response and result
    assert response == result

print("All tasks finished successfully!")
