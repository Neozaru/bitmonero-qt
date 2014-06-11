DEPRECATED README

bitmonero-qt
============

Monero GUI Wallet


**DISCLAIMER : This is not an official Wallet for Monero. Use this software at your own risk.**


Current status : Testing


Dependencies
------------

The project is based on **Qt** SDK. It should work version **5.2+**.
* Arch-based distributions : `sudo pacman -S qt5`

* Debian-based distributions : `sudo apt-get install qt5-default qttools5-dev-tools`
    
* Or try the **Qt**'s installer : [http://qt-project.org/downloads]


Compiling
---------

    git clone https://github.com/Neozaru/bitmonero-qt.git
    cd bitmonero-qt
    qmake
    make

The app will be installed in `./app/bitmonero-qt`


Usage
-----

**Monero Wallet** depends on core **bitmonerod** and **simplewallet**. These two programs should be installed in same folder as bitmonero-qt or in $HOME/.bitmonero-qt/


#### Running *bitmonero-qt*

Ensure that NO *bitmonerod* or *simplewallet* processes are already running, and do :

    ./app/bitmonero-qt


By default, **bitmonero-qt** will connect to daemons ports 18081 (*bitmonerod*) and 19091 (*simplewallet*).


Your can change the configuration with the file `$HOME/.bitmonero-qt/bitmonero-qt.conf`


Configuration file example
--------------------------

    
    [General]
    
    # These values are defaults
    daemon_uri=http://localhost/json_rpc
    daemon_port=18081
    
    # Simple wallet RPC port/uri. These values are defaults
    wallet_uri=http://localhost
    wallet_port=19091

    # Wallet auto-spawn. Settings will depend on your own system.
    wallet_program=/home/myself/gitcode/bitmonero/simplewallet
    
    # Same as the daemon (bitmonerod), these values are defaults
    miner_uri=http://localhost/json_rpc
    miner_port=18081


Available features
------------------
* Create/Import Wallet wizard
* Overview (balance, address)
* Transfer (choose an amount and a recipient)
* Mining (choose number of threads and your address, and start mining)


Limitations
-----------
* Transactions history is not (yet) available.
* Mining interface can get confused if mining is started from somewhere else

Known Issues
-----------
* Window is not repainting correctly when it is not the active window (ArchLinux)

Roadmap
-------
* Bitmonero C++ integration (all-in-one)
* Advanced interface

Alternatives
------------

* CryptoNote Wallet ('.NET' based) : [https://github.com/BitKoot/CryptoNoteWallet]
* cryptonoteRPCwalletGUI ('python-kivy' based) : [https://github.com/jwinterm/cryptonoteRPCwalletGUI]
