import hashlib
import os
import secrets
import time

from more_itertools import sliced
from simple_term_menu import TerminalMenu
from smartcard.CardConnectionObserver import CardConnectionObserver
from smartcard.CardRequest import CardRequest
from smartcard.CardType import AnyCardType
from smartcard.util import toHexString

# define keys - 12 ASCII characters in hexadecimal
# default: password1234
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


# define the apdus used in this script
def APDU_SELECT_APP():
    #       CLA   INS   P1    P2    Lc    AID1  AID2  AID3  AID4  Le
    return [0x00, 0xA4, 0x04, 0x00, 0x04, 0xF0, 0x00, 0x00, 0x01, 0x00]


def APDU_GET_CURRENT_KEY():
    #       CLA   INS   P1    P2    Le
    return [0x80, 0x40, 0x00, 0x00, 0x0C]


def APDU_SET_KEY(new_key_to_set):
    #       CLA   INS   P1    P2    Lc    Data
    return [0x80, 0x20, 0x00, 0x00, 0x0C] + new_key_to_set


def APDU_RESET_KEY():
    #       CLA   INS   P1    P2
    return [0x80, 0x30, 0x00, 0x00]


def APDU_GET_HASH(challenge):
    #       CLA   INS   P1    P2    Lc               challenge    Le
    return [0x80, 0x10, 0x00, 0x00, challenge_len] + challenge + [0x20]


def APDU_GET_RESPONSE():
    #       CLA   INS   P1    P2    Le
    return [0x00, 0xC0, 0x00, 0x00, 0x14]


# random 52 bytes challenge
challenge_len = 52


def get_challenge():
    return [int(x, 16) for x in sliced(secrets.token_hex(challenge_len), 2)]


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


# we request any type and wait for 10s for card insertion
cardrequest = CardRequest(timeout=10, cardType=AnyCardType())
cardservice = cardrequest.waitforcard()
conn = cardservice.connection

# create an instance of our observer and attach to the connection
conn.addObserver(ConsoleCardConnectionObserver())

# connect and send APDUs
# the observer will trace on the console
conn.connect()
conn.transmit(APDU_SELECT_APP())


def cls():
    os.system("cls" if os.name == "nt" else "clear")


def preview(key):
    return "Default key:" + str(bytes(default_key))


def get_current_key():
    response, sw1, _ = conn.transmit(APDU_GET_CURRENT_KEY())
    if sw1 == 144:
        print("Current key:\t", "".join(map(chr, response)))
        print("Source:\t\t", response)


def set_new_key():
    while True:
        print("\n\n")
        new_key_to_set = input("Input 12 char long password: ")

        if len(new_key_to_set) != 12:
            print("Password needs to have 12 characters!")
            continue

        if input("Repeat: ") != new_key_to_set:
            print("Passwords do not match!")
            continue

        try:
            output = [ord(c) for c in new_key_to_set]
        except:
            print("Characters need to be convertable to ascii!")
            continue

        _, sw1, _ = conn.transmit(APDU_SET_KEY(output))
        if sw1 == 144:
            print("Password has been successfully set to: ", new_key_to_set)
            print("Source: ", output)
            break
        else:
            print("Error!")


def reset_key():
    _, sw1, _ = conn.transmit(APDU_RESET_KEY())
    if sw1 == 144:
        print("Key was successfully reset to 'password1234'")
    else:
        print("Error!")


def authenticate():
    password = input("Input password: ")
    if conn.transmit(APDU_GET_HASH(challenge := get_challenge()))[1] == 97:
        response, sw1, _ = conn.transmit(APDU_GET_RESPONSE())

        if sw1 == 144:
            hash = hashlib.sha1()
            hash.update(bytes(challenge + [ord(c) for c in password]))
            result = list(hash.digest())

            if response == result:
                print("YES - Authentication successfull!")
            else:
                print("NO - Authentication not successfull!")
        else:
            print("Error!")
    else:
        print("Error!")


def mainmenu():
    main_menu_title = "\n\n\n______________________\nCard communicator\n"
    main_menu_items = [
        "[g] Get current key",
        "[s] Set new key",
        "[r] Reset key to default",
        "[a] Authenticate",
        "[q] Quit",
    ]
    main_menu_cursor = "> "
    main_menu_cursor_style = ("fg_red", "bold")
    main_menu_style = ("bg_red", "fg_yellow")
    main_menu_exit = False

    main_menu = TerminalMenu(
        menu_entries=main_menu_items,
        title=main_menu_title,
        menu_cursor=main_menu_cursor,
        menu_cursor_style=main_menu_cursor_style,
        menu_highlight_style=main_menu_style,
        cycle_cursor=True,
        clear_screen=False,
        clear_menu_on_exit=False,
        status_bar_below_preview=True,
    )

    while not main_menu_exit:
        main_sel = main_menu.show()
        print("______________________\n\n\n")

        if main_sel == 0:
            get_current_key()
        elif main_sel == 1:
            set_new_key()
        elif main_sel == 2:
            reset_key()
        elif main_sel == 3:
            authenticate()
        elif main_sel == 4 or main_sel is None:
            main_menu_exit = True


if __name__ == "__main__":
    os.system("stty sane")
    mainmenu()
