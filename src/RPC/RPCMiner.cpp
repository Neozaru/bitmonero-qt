#include "RPCMiner.h"

RPCMiner::RPCMiner(MinerModel& pMinerModel, const QString& pHost, unsigned int pPort)
    : MinerInterface(pMinerModel), rpc(pHost,pPort)
{
}
