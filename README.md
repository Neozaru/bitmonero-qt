bitmonero-qt
============

Monero GUI Wallet (state: experimental)


**DISCLAIMER : This is not an official Wallet for Monero. Use this software at your own risk.**


Current status : Experimental


Usage :
bitmonero-qt depends on core **bitmonerod** and **simplewallet**. These two programs should be started before running **bitmonero-qt**.
    `bitmonerod`
    `simplewallet --wallet=<your_wallet_location> --pass=<your_pass> --rpc-bind-port=19091`

By default, bitmonero-qt will connect to daemons ports 18081 (bitmonerod) and 19091 (simplewallet).
Your can change the configuration with the file `$HOME/.bitmonero-qt/bitmonero-qt.conf`
Configuration file example : 
`
    [General]
    
    daemon_uri=http://localhost/json_rpc
    daemon_port=18081
    
    wallet_uri=http://localhost
    wallet_port=19091
    
    \# Same as the daemon (bitmonerod)
    miner_uri=http://localhost/json_rpc
    miner_port=18081
`

Available features :
* Overview (balance, address)
* Transfer (choose an amount and a recipient)
* Mining (choose number of threads and your address, and start mining)


Limitations :
* Transactions history is not (yet) available.
* Mining interface can get confused if mining is started from somewhere else
