#include "InfoWalletModel.h"

InfoWalletModel::InfoWalletModel(const QString& pName, const QString& pPath, const QString& pAddress, int pStatus)
    : name(pName), file_path(pPath), address(pAddress), status(pStatus)
{
}
