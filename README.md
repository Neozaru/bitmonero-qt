See Wiki for build instructions

bitmonero-qt
============

Monero GUI Wallet


**DISCLAIMER : This is not an official Wallet for Monero. Use this software at your own risk.**


Current status : Testing



Using a Monero GUI wallet should be reserved to experienced users at this stage.
The Monero core itself is in perpetual improvement but not yet ready of end users.
Please use with caution.


Available features
------------------
* Create/Import Wallet wizard
* Recover wallet from electrum-style seed
* Overview (balance, address)
* Transfer (choose an amount and a recipient. Optional payment ID and custom fee)
* Mining (choose number of threads and your address, and start mining)


Recent changes
--------------
* [TECH] Ui text .ts files generation (internationalization)
* [USER] Electrum-style seed added (copy it on wallet creation, paste it in wallet import)
* [TECH] Dynamic architecture : Interfaces to Daemon and Wallet have been highly abstracted
* [TECH] LibMoneroWallet mode : Uses Wallets with RPC and/or subprocesses. (more stable, with more features). This features uses an experimental Wallet C++ library (https://github.com/Neozaru/bitmonero/blob/wallet_wrapper/src/monero_wallet_wrapper/MoneroWallet.hh). The RPC-wallet mode will be deprecated as soon as build process will be made easier.



Limitations
-----------
* Transactions history is not (yet) available.
* Mining interface can get confused if mining is started from somewhere else


Roadmap
-------
* Bitmonero C++ integration (all-in-one)
* Advanced interface



Alternatives
------------

* CryptoNote Wallet ('.NET' based) : [https://github.com/BitKoot/CryptoNoteWallet]
* Monero Wallet ('.NET'/WPF based) : [https://github.com/Jojatekok/monero-client]
* cryptonoteRPCwalletGUI ('python-kivy' based) : [https://github.com/jwinterm/cryptonoteRPCwalletGUI]
