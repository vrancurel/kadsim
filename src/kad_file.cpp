#include "kadsim.h"

KadFile::KadFile(const CBigNum& id, KadNode* referencer)
    : KadRoutable(id, KAD_ROUTABLE_FILE)
{
    this->referencer = referencer;
}

KadFile::~KadFile() {}

KadNode* KadFile::get_referencer()
{
    return referencer;
}
