#ifndef __KAD_CONF_H__
#define __KAD_CONF_H__

#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

#include <jsonrpccpp/client/connectors/httpclient.h>

#include "gethclient.h"

class KadConf {
  public:
    KadConf(
        uint32_t n_bits,
        uint32_t k,
        uint32_t alpha,
        uint32_t n_nodes,
        const std::string& geth_addr,
        std::vector<std::string> bstraplist);

    void save(std::ostream& fout);

    uint32_t n_bits;
    uint32_t k;
    uint32_t alpha;
    uint32_t n_nodes;

    jsonrpc::HttpClient httpclient;
    GethClient geth;
    std::vector<std::string> bstraplist;
};

#endif
