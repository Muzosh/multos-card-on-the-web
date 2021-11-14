from smartcard.CardType import AnyCardType
from smartcard.CardRequest import CardRequest
from smartcard.CardConnectionObserver import CardConnectionObserver
from smartcard.util import toHexString
import secrets
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
default_key = [0x70, 0x61, 0x73, 0x73, 0x77, 0x6F, 0x72, 0x64, 0x31, 0x32, 0x33, 0x34]
new_key_to_set = [0x63, 0x69, 0x73, 0x63, 0x6F, 0x32, 0x30, 0x32, 0x31, 0x61, 0x62, 0x63]

# random 52 bytes challenge = 104 hexadecimals
challenge = [int(x, 16) for x in sliced(secrets.token_hex(104), 2)]


# define the apdus used in this script
#                    CLA   INS   P1    P2    Lc    AID1  AID2  AID3  AID4  Le
SELECTAPP =         [0x00, 0xA4, 0x04, 0x00, 0x04, 0xF0, 0x00, 0x00, 0x01, 0x00]
#                    CLA   INS   P1    P2    Le
GET_CURRENT_KEY =   [0x80, 0x40, 0x00, 0x00, 0x0C]
#                    CLA   INS   P1    P2    Lc    Data
SET_KEY =           [0x80, 0x20, 0x00, 0x00, 0x0C] + new_key_to_set
#                    CLA   INS   P1    P2
RESET_KEY =         [0x80, 0x30, 0x00, 0x00]
#                    CLA   INS   P1    P2    Lc      challenge    Le
GET_RESPONSE =      [0x80, 0x10, 0x00, 0x00, 0x34] + challenge + [0x14]


# we request any type and wait for 10s for card insertion
cardrequest = CardRequest(timeout=10, cardType=AnyCardType())
cardservice = cardrequest.waitforcard()
conn = cardservice.connection

# create an instance of our observer and attach to the connection
conn.addObserver(ConsoleCardConnectionObserver())

# connect and send APDUs
# the observer will trace on the console
conn.connect()

response, sw1, sw2 = conn.transmit(SELECTAPP)
response, sw1, sw2 = conn.transmit(GET_CURRENT_KEY)
response, sw1, sw2 = conn.transmit(SET_KEY)


exit()
