from smartcard.CardType import AnyCardType
from smartcard.CardRequest import CardRequest
from smartcard.CardConnectionObserver import CardConnectionObserver
from smartcard.util import toHexString
from smartcard.ATR import ATR
from smartcard.util import toHexString

atr = ATR([0x3b, 0x6f, 0x00, 0x00, 0x80, 0x31, 0xe0, 0x6b, 0x84, 0x31, 0x02, 0x07, 0x0f, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55])

print(atr)
print('historical bytes: ', toHexString(atr.getHistoricalBytes()))
# print('checksum: ', "0x%X" % atr.getChecksum())
print('T0  supported: ', atr.isT0Supported())
print('T1  supported: ', atr.isT1Supported())
print('T15 supported: ', atr.isT15Supported())

class ConsoleCardConnectionObserver(CardConnectionObserver):
    """This observer will interprer SELECT and GET RESPONSE bytes
    and replace them with a human readable string."""

    def update(self, cardconnection, ccevent):

        if 'connect' == ccevent.type:
            print(('connecting to ' + cardconnection.getReader()))

        elif 'disconnect' == ccevent.type:
            print(('disconnecting from ' + cardconnection.getReader()))

        elif 'command' == ccevent.type:
            str = toHexString(ccevent.args[0])
            print(('>', str))

        elif 'response' == ccevent.type:
            if [] == ccevent.args[0]:
                print(('<  []', "%-2X %-2X" % tuple(ccevent.args[-2:])))
            else:
                print(('<',
                      toHexString(ccevent.args[0]),
                      "%-2X %-2X" % tuple(ccevent.args[-2:])))


# define the apdus used in this script
#                    CLA   INS   P1    P2    Lc    AID1  AID2  AID3  AID4  Le
SELECT =            [0x00, 0xA4, 0x04, 0x00, 0x04, 0xf0, 0x00, 0x00, 0x01, 0x00]

#                    CLA   INS   P1    P2    Le
GET_CURRENT_KEY =   [0x80, 0x20, 0x00, 0x00, 0x0c]

#                    CLA   INS   P1    P2    Lc    Data
SET_KEY =           [0x80, 0x20, 0x00, 0x00, 0x0c, 0x63, 0x69, 0x73, 0x63, 0x6F, 0x32, 0x30, 0x32, 0x31, 0x61, 0x62, 0x63]


# we request any type and wait for 10s for card insertion
cardrequest = CardRequest(timeout=10, cardType=AnyCardType())
cardservice = cardrequest.waitforcard()
conn = cardservice.connection

# create an instance of our observer and attach to the connection
conn.addObserver(ConsoleCardConnectionObserver())

# connect and send APDUs
# the observer will trace on the console
conn.connect()

response, sw1, sw2 = conn.transmit(SELECT)
response, sw1, sw2 = conn.transmit(GET_CURRENT_KEY)
response, sw1, sw2 = conn.transmit(SET_KEY)


exit()