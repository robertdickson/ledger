//------------------------------------------------------------------------------
//
//   Copyright 2018-2019 Fetch.AI Limited
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

#include "ledger/chain/main_chain_miner.hpp"

namespace fetch {
namespace ledger {

MainChainMiner::MainChainMiner(std::size_t num_lanes, std::size_t num_slices, MainChain &mainChain,
                               BlockCoordinator &block_coordinator, MinerInterface &miner,
                               ConsensusMinerInterface &           consensus_miner,
                               ConstByteArray                      miner_identity,
                               std::chrono::steady_clock::duration block_interval)
  : num_lanes_{num_lanes}
  , num_slices_{num_slices}
  , main_chain_{mainChain}
  , blockCoordinator_{block_coordinator}
  , miner_{miner}
  , consensus_miner_{consensus_miner}
  , miner_identity_{std::move(miner_identity)}
  , block_interval_{block_interval}
{}

MainChainMiner::~MainChainMiner()
{
  Stop();
}

void MainChainMiner::Start()
{
  stop_   = false;
  thread_ = std::thread{&MainChainMiner::MinerThreadEntrypoint, this};
}

void MainChainMiner::Stop()
{
  stop_ = true;
  if (thread_.joinable())
  {
    thread_.join();
  }
}

void MainChainMiner::OnBlockComplete(BlockCompleteCallback const &func)
{
  on_block_complete_ = func;
}

void MainChainMiner::SetConsensusMiner(ConsensusMinerInterface consensus_miner)
{
  consensus_miner_ = consensus_miner;
}

void MainChainMiner::MinerThreadEntrypoint()
{
  SetThreadName("MChainMiner");

  // schedule the next block time
  Timestamp next_block_time = Clock::now() + block_interval_;

  BlockHash previous_heaviest;

  Block       next_block;
  Block::Body next_block_body;

  bool searching_for_hash = false;

  while (!stop_)
  {
    // determine the heaviest block
    auto &block = main_chain_.HeaviestBlock();

    // if the heaviest block has changed then we need to schedule the next block time
    if (block.body.hash != previous_heaviest)
    {
      FETCH_LOG_INFO(LOGGING_NAME,
                     "==> New heaviest block: ", byte_array::ToBase64(block.body.hash),
                     " from: ", byte_array::ToBase64(block.body.previous_hash));

      // new heaviest has been detected
      next_block_time    = Clock::now() + block_interval_;
      previous_heaviest  = block.body.hash.Copy();
      searching_for_hash = false;
    }

    if (searching_for_hash)
    {
      if (consensus_miner_->Mine(next_block, 100))
      {
        // Add the block
        blockCoordinator_.AddBlock(next_block);

        // TODO(EJF): Feels like this needs to be reworked into the block coordinator
        if (on_block_complete_)
        {
          on_block_complete_(next_block);

          FETCH_METRIC_BLOCK_GENERATED(next_block.body.hash);
        }

        // stop searching for the hash and schedule the next time to generate a block
        next_block_time    = Clock::now() + block_interval_;
        searching_for_hash = false;
      }
    }
    else if (Clock::now() >= next_block_time)  // if we are ready to generate a new block
    {
      // update the metadata for the block
      next_block_body.block_number  = block.body.block_number + 1;
      next_block_body.previous_hash = block.body.hash;
      next_block_body.miner         = miner_identity_;

      // Reset previous state
      next_block_body.slices.clear();

      FETCH_LOG_INFO(LOGGING_NAME, "Generate new block: ", num_lanes_, " x ", num_slices_);

      // Pack the block with transactions
      miner_.GenerateBlock(next_block_body, num_lanes_, num_slices_, main_chain_);
      next_block.body = next_block_body;
      next_block.UpdateDigest();

#ifdef FETCH_ENABLE_METRICS
      metrics::Metrics::Timestamp const now = metrics::Metrics::Clock::now();

      for (auto const &slice : next_block_body.slices)
      {
        for (auto const &tx : slice)
        {
          FETCH_METRIC_TX_PACKED_EX(tx.transaction_hash, now);
        }
      }
#endif  // FETCH_ENABLE_METRICS

      // Mine the block
      next_block.proof.SetTarget(target_);
      searching_for_hash = true;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds{10});
  }
}

}  // namespace ledger
}  // namespace fetch