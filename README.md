bitmonero-qt
============

Monero GUI Wallet (state: experimental)


**DISCLAIMER : This is not an official Wallet for Monero. Use this software at your own risk.**


Current status : Experimental


Dependencies
------------

The project is based on **Qt** SDK. It should work version **5.2+**.
Arch-based distributions :
    sudo pacman -S qt5

Debian-based distributions :
    sudo apt-get install qt5-default qttools5-dev-tools
    
Or try the **Qt**'s installer :
[http://qt-project.org/downloads]


Compiling
---------

    qmake
    make

The app will be installed in `./app/bitmonero-qt`

You can run the app with the following command :
    `LD_LIBRARY_PATH=src/ app/bitmonero-qt`

('LD_LIBRARY_PATH' variable is required for the moment because I'm generating dynamic libraries for better structure, but they are not linked to the program by default. Will be fixed soon.)


Usage
-----

**Monero Wallet** depends on core **bitmonerod** and **simplewallet**. These two programs should be started before running **bitmonero-qt**.

    bitmonerod
    simplewallet --wallet=<your_wallet_location> --pass=<your_pass> --rpc-bind-port=19091

By default, bitmonero-qt will connect to daemons ports 18081 (bitmonerod) and 19091 (simplewallet).
Your can change the configuration with the file `$HOME/.bitmonero-qt/bitmonero-qt.conf`


Configuration file example
--------------------------

    
    [General]
    
    daemon_uri=http://localhost/json_rpc
    daemon_port=18081
    
    wallet_uri=http://localhost
    wallet_port=19091
    
    # Same as the daemon (bitmonerod)
    miner_uri=http://localhost/json_rpc
    miner_port=18081


Available features
------------------
* Overview (balance, address)
* Transfer (choose an amount and a recipient)
* Mining (choose number of threads and your address, and start mining)


Limitations
-----------
* Transactions history is not (yet) available.
* Mining interface can get confused if mining is started from somewhere else
