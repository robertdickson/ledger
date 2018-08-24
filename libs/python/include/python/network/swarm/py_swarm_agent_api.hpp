#pragma once
//------------------------------------------------------------------------------
//
//   Copyright 2018 Fetch.AI Limited
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.
//
//------------------------------------------------------------------------------

#include "network/swarm/swarm_peer_location.hpp"
#include "py_swarm.hpp"
#include "python/fetch_pybind.hpp"
#include <pybind11/chrono.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace fetch {
namespace swarm {

void BuildSwarmAgentApi(pybind11::module &module)
{
  pybind11::class_<fetch::swarm::PySwarm, std::shared_ptr<fetch::swarm::PySwarm>>(module, "Swarm")
      .def(pybind11::init<uint32_t, uint16_t, uint16_t, uint32_t, uint32_t, uint32_t, uint32_t>())
      .def("Start", &PySwarm::Start)
      .def("Stop", &PySwarm::Stop)
      .def("OnPeerless", &PySwarm::OnPeerless)

      .def("OnIdle", &PySwarm::OnIdle)
      .def("OnPingSucceeded", &PySwarm::OnPingSucceeded)
      .def("OnPingFailed", &PySwarm::OnPingFailed)
      .def("OnNewPeerDiscovered", &PySwarm::OnNewPeerDiscovered)
      .def("OnPeerDiscoverFail", &PySwarm::OnPeerDiscoverFail)
      .def("DoPing", &PySwarm::DoPing)

      // We *have* an entry point for this but currently don't use it because we
      // treat Ping as DiscoverPeers
      //.def("DoDiscoverPeers", &PySwarm::DoDiscoverPeers)
      .def("OnNewPeerDiscovered", &PySwarm::OnNewPeerDiscovered)
      .def("OnPeerDiscoverFail", &PySwarm::OnPeerDiscoverFail)

      .def("DoBlockSolved", &PySwarm::DoBlockSolved)
      .def("DoDiscoverBlocks", &PySwarm::DoDiscoverBlocks)
      .def("OnNewBlockIdFound", &PySwarm::OnNewBlockIdFound)
      .def("OnBlockIdRepeated", &PySwarm::OnBlockIdRepeated)
      .def("DoGetBlock", &PySwarm::DoGetBlock)
      .def("OnNewBlockAvailable", &PySwarm::OnNewBlockAvailable)
      .def("GetBlock", &PySwarm::GetBlock)
      .def("VerifyBlock", &PySwarm::VerifyBlock)

      // TODO(katie) implement the back end of these.
      //.def("DoTransactionListBuilt", &PySwarm::DoTransactionListBuilt)
      //.def("OnNewTxnListIdFound", &PySwarm::OnNewTxnListIdFound)
      //.def("DoGetTxnList", &PySwarm::DoGetTxnList)
      //.def("OnNewTxnListAvailable", &PySwarm::OnNewTxnListAvailable)
      //.def("GetTxnList", &PySwarm::GetTxnList)

      .def("AddKarma", &PySwarm::AddKarma)
      .def("AddKarmaMax", &PySwarm::AddKarmaMax)
      .def("GetKarma", &PySwarm::GetKarma)
      .def("GetCost", &PySwarm::GetCost)
      .def("GetPeers", &PySwarm::GetPeers)
      .def("queryOwnLocation", &PySwarm::queryOwnLocation)

      .def("OnBlockSupplied", &PySwarm::PyOnBlockSupplied)
      .def("OnBlockNotSupplied", &PySwarm::PyOnBlockNotSupplied)
      .def("OnLooseBlock", &PySwarm::PyOnLooseBlock)
      .def("HeaviestBlock", &PySwarm::HeaviestBlock);
}

}  // namespace swarm
}  // namespace fetch