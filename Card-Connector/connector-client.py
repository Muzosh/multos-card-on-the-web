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

import socket
import secrets
import hashlib
from more_itertools import sliced

# CONFIG
HOST = "localhost"
PORT = 5050
CHALLENGE_LEN = 52
RESPONSE_LEN = 20


def get_challenge():
    return [int(x, 16) for x in sliced(secrets.token_hex(CHALLENGE_LEN), 2)]


def authenticate(password):
    challenge = get_challenge()

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client:
        client.connect((HOST, PORT))
        print(f"Connected to server {HOST, PORT}")
        print(f"Sending challenge {challenge}")
        client.sendall(bytes(challenge))

        if not client.recv(1):
            raise Exception("Server returned error")

        response = client.recv(RESPONSE_LEN)
        response = list(response)

        h = hashlib.sha1(
            bytes(challenge + [ord(c) for c in password])
        ).digest()
        result = list(h)

        if response == result:
            print("YES - Authentication successfull!")
        else:
            print("NO - Authentication not successfull!")


if __name__ == "__main__":
    authenticate(input("Input password: "))
