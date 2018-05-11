/*
 * Copyright 2017-2018 the QuadIron authors
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef __KAD_NODE_COM_H__
#define __KAD_NODE_COM_H__

#include "dht/dht.h"

namespace kad {

class Network;

/// Communication module for "fake" node.
class NodeLocalCom : public dht::NodeComBase {
  public:
    explicit NodeLocalCom(const Network* network) : m_network(network) {}

    bool ping(const dht::NodeAddress& addr) override;

    std::vector<dht::NodeAddress> find_node(
        const dht::NodeAddress& addr,
        const UInt160& target_id,
        uint32_t nb_nodes) override;

    NodeLocalCom() = delete;
    ~NodeLocalCom() override = default;
    NodeLocalCom(NodeLocalCom const&) = default;
    NodeLocalCom& operator=(NodeLocalCom const& x) = default;
    NodeLocalCom(NodeLocalCom&&) = delete;
    NodeLocalCom& operator=(NodeLocalCom&& x) = delete;

  private:
    // TODO: use shared_ptr?
    const Network* m_network;
};

} // namespace kad

#endif
