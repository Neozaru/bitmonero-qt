bitmonero-qt
============

Monero GUI Wallet


**DISCLAIMER : This is not an official Wallet for Monero. Use this software at your own risk.**


Current status : Experimental / First draft

Screenshots :

* [https://imgur.com/4WA7VCH,Hq15yUW,SBAPxSW#2]
* [https://imgur.com/4WA7VCH,Hq15yUW,SBAPxSW#1]
* [https://imgur.com/4WA7VCH,Hq15yUW,SBAPxSW#0]

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

**Monero Wallet** depends on core **bitmonerod** and **simplewallet**. These two programs should be started before running **bitmonero-qt**.

    bitmonerod
    simplewallet --wallet=<your_wallet_location> --pass=<your_pass> --rpc-bind-port=19091

By default, bitmonero-qt will connect to daemons ports 18081 (bitmonerod) and 19091 (simplewallet).

EDIT : You can now auto-spawn the Wallet process from the GUI, using 'wallet_file', 'wallet_password' and 'wallet_program' options in configuration. If 'wallet_password' is not set, the GUI will attempt to connect to a running *simplewallet*. You can also disable auto-spawning using the option 'spawn_wallet=false' (default: true).


Your can change the configuration with the file `$HOME/.bitmonero-qt/bitmonero-qt.conf`


Configuration file example
--------------------------

    
    [General]
    
    daemon_uri=http://localhost/json_rpc
    daemon_port=18081
    
    wallet_uri=http://localhost
    wallet_port=19091

    # Wallet auto-spawn. Settings will depend on your own system.
    wallet_file=/home/myself/.bitmonero/wallet.bin
    wallet_password=mysuperpassword85
    wallet_program=/home/myself/gitcode/bitmonero/simplewallet
    
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

Known Issues
-----------

* Window is not repainting correctly when it is not the active window (ArchLinux)

Alternatives
------------

* CryptoNote Wallet ('.NET' based) : [https://github.com/BitKoot/CryptoNoteWallet]
* cryptonoteRPCwalletGUI ('python-kivy' based) : [https://github.com/jwinterm/cryptonoteRPCwalletGUI]
