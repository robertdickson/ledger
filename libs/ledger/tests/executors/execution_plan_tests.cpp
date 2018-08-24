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

//#include "testing/unittest.hpp"
//#include "ledger/execution_manager.hpp"
//#include "ledger/protocols/execution_manager_rpc_service.hpp"
//#include "ledger/protocols/execution_manager_rpc_client.hpp"
//
//
//#include <string>
//
// namespace {
//
//} // namespace
//
//
//#if 0
// int main()
//{
//  fetch::network::ThreadManager tm;
//  fetch::ledger::ExecutionManagerService service(3000, tm);
//  fetch::ledger::ExecutionManagerRPCClient client("127.0.0.1", 3000, tm);
//
//
//  static constexpr uint64_t IV = uint64_t(-1);
//
//  fetch::ledger::ExecutionManager::tx_index_type index;
//  fetch::ledger::ExecutionManager::block_map_type map = {
//    0, IV,  4,  6,
//    0, IV,  5,  6,
//    1,  2,  5,  6,
//    1,  3, IV, IV
//  };
//
//  for (std::size_t i = 1; i <= 7; ++i) {
//    std::string transaction = "transaction " + std::to_string(i);
//    index.emplace_back(transaction);
//  }
//
//  tm.Start();
//
//  client.Execute(index, map, 4, 4);
//
//#if 0
//  std::size_t plan_idx = 1;
//  for (auto &item : plan) {
//    std::cout << "Plan " << plan_idx << ": ";
//    std::cout << item.hash << " : " << item.slice << " : ";
//
//    bool not_first_loop = false;
//    for (auto lane : item.lanes) {
//      if (not_first_loop) std::cout << ',';
//      std::cout << lane;
//      not_first_loop = true;
//    }
//
//    std::cout << std::endl;
//  }
//#endif
//
//  tm.Stop();
//
//  return 0;
//}
//#endif
//