# multos-card-nextcloud

This is a sample project for MultOS card programming using SmartDeck (for libraries and Mel API) and MUtil (for application loading).

## Card-App
`<Smart-Deck-install-location>/bin` must be in system $PATH variable in order to successfully build `.hlx` and `.alu` files using `make`.

`hls -t nextcloud.hzx` can be used to determine, how much dynamic memory shall be used while loading the app (.DB = dynamic memory data size).

Use `nextcloud.alu` for test load in MUtil and specify .DB amount of memory before loading.

## Python-App
This is a simple app used for testing.

`pip install pyscard` is needed (might require to install `swig`)

`test.py` should fininsh successfully, `main.py` is interactive cli for repeated commands
