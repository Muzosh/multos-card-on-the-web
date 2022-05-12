# multos-card-nextcloud

This is a sample proof-of-work project for expanding my skills in:

- MultOS card programming using SmartDeck (for libraries and Mel API) and MUtil (for application loading)
- Python socket programming that allows local connector communicating with remote Nextcloud server

## Projects

### Card-App directory

Everything related to card programming

#### MultOS programming:

- `<Smart-Deck-install-location>/bin` must be in system $PATH variable in order to successfully build `.hlx` and `.alu` files using `make`.

- `hls -t nextcloud.hzx` can be used to determine, how much dynamic memory shall be used while loading the app (.DB = dynamic memory data size).

- Use `nextcloud.alu` for test load in MUtil and specify .DB amount of memory before loading.

#### Python helper scripts:

- Package requirements: `pyscard`, `more-itertools`, `simple-term-menu`

- `pyscard` might require to install `swig` on the system (can be installed via `homebrew` on MacOS, google for other systems)

- `test.py` should fininsh successfully while card is connected

- `cli.py` is interactive cli for repeated commands (can be used to test and configure card password)

### Card-Connector directory

This directory contains files using socket programming to communicate between local connector which listens for challenges from remote Nextcloud server.

`connector.py` MUST be running and listening in order to successfully login into Nextcloud via smartcard

`connector-client.py` is for testing on localhost network

`index.php` is sample PHP script which runs on remote server and tries to authenticate local user via his running connector
