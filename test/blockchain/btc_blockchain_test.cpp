#include <gtest/gtest.h>

#include <fstream>
#include <memory>

#include "block_headers.hpp"
#include "util/literals.hpp"
#include "veriblock/blockchain/block_index.hpp"
#include "veriblock/blockchain/blocktree.hpp"
#include "veriblock/blockchain/btc_blockchain_util.hpp"
#include "veriblock/blockchain/miner.hpp"
#include "veriblock/storage/block_repository_inmem.hpp"

using namespace VeriBlock;

struct BootstrapTestCase {
  using block_t = BtcBlock;
  using hash_t = BtcBlock::hash_t;
  using height_t = BtcBlock::height_t;

  BtcBlock bootstrap_block;
  height_t height;
  hash_t block_hash;
};

struct BlockchainFixture {
  using block_t = BtcBlock;
  using param_t = BtcChainParams;
  using index_t = typename BlockTree<block_t, param_t>::index_t;
  using height_t = typename BlockTree<block_t, param_t>::height_t;
  using hash_t = typename BlockTree<block_t, param_t>::hash_t;

  std::shared_ptr<param_t> params;
  std::shared_ptr<BlockRepository<index_t>> repo;
  ValidationState state;

  BlockchainFixture() {
    params = std::make_shared<BtcChainParamsRegTest>();
    repo = std::make_shared<BlockRepositoryInmem<index_t>>();
  }
};

static std::vector<BootstrapTestCase> bootstrap_test_cases = {
    // clang-format off
    // mainnet genesis block
    {
        BtcBlock::fromRaw("0100000000000000000000000000000000000000000000000000000000000000000000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a29ab5f49ffff001d1dac2b7c"_unhex),
        0, 
        uint256::fromHex("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f")
    },
    //mainnet 15687 height block
    {
        BtcBlock::fromRaw("01000000d9e9506d9b1800242adae0e97c96f77c86ec8ad3e1cca88a39f5a63b000000008563a346f8466a5d5106797aa54ccfe3829042970d78fa635427335f326c71f2857d1c4affff001d2d6f7258"_unhex),
        15687, 
        uint256::fromHex("000000002f361e7a5c28e1d11bd153830f31081445538c67495bb72b1768ab4a")
    },
    //testnet genesis block
    {
        BtcBlock::fromRaw("0100000000000000000000000000000000000000000000000000000000000000000000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4adae5494dffff001d1aa4ae18"_unhex),
        0, 
        uint256::fromHex("000000000933ea01ad0ee984209779baaec3ced90fa3f408719526f8d77f4943")
    },
    //testnet 12554 height block
    {
        BtcBlock::fromRaw("010000006ed26f617543137b35fd866d8771ce41768110c0fc089d70692e6d1c000000002ed0cde332d6c9d5fea9bf8a6a66375c2cc1d343b4285f5e38ca2aa76e0d3a1d3708c34fc0ff3f1c662b2430"_unhex),
        12554, 
        uint256::fromHex("0000000030f55553e130da0c3f8fac97a2745ae1c742f683037177dd3715460e")
    },
    // clang-format on
};

struct BootstrapTest : public testing::TestWithParam<BootstrapTestCase>,
                       public BlockchainFixture {};

TEST_P(BootstrapTest, bootstrap_test) {
  auto value = GetParam();

  params = std::make_shared<BtcChainParamsMain>();
  BlockTree<BtcBlock, BtcChainParams> block_chain(repo, params);
  ASSERT_TRUE(
      block_chain.bootstrap(value.height, value.bootstrap_block, state));
  EXPECT_TRUE(state.IsValid());

  EXPECT_EQ(block_chain.getBestChain().tip()->header, value.bootstrap_block);
  EXPECT_EQ(block_chain.getBestChain().bootstrap()->header,
            value.bootstrap_block);
  EXPECT_EQ(block_chain.getBestChain().tip()->getHash(), value.block_hash);
}

INSTANTIATE_TEST_SUITE_P(BootstrapBlocksRegression,
                         BootstrapTest,
                         testing::ValuesIn(bootstrap_test_cases));

struct BtcTestCase {
  std::string headers;
  std::shared_ptr<BtcChainParams> params;
};

struct AcceptTest : public testing::TestWithParam<BtcTestCase>,
                    public BlockchainFixture {};

static std::vector<BtcTestCase> accept_test_cases = {
    {generated::btc_blockheaders_mainnet_0_10000,
     std::make_shared<BtcChainParamsMain>()},
    {generated::btc_blockheaders_mainnet_30000_40000,
     std::make_shared<BtcChainParamsMain>()},
    {generated::btc_blockheaders_testnet_0_10000,
     std::make_shared<BtcChainParamsTest>()},
};

TEST_P(AcceptTest, CanAcceptRealBlockHeaders) {
  auto value = GetParam();

  std::istringstream file(value.headers);
  ASSERT_TRUE(!file.fail());

  uint32_t first_block_height;
  file >> first_block_height;

  std::string temp;
  EXPECT_TRUE(file >> temp);
  BtcBlock bootstrap_block = BtcBlock::fromRaw(ParseHex(temp));

  BlockTree<BtcBlock, BtcChainParams> block_chain(repo, value.params);
  ASSERT_TRUE(
      block_chain.bootstrap(first_block_height, bootstrap_block, state));
  EXPECT_TRUE(state.IsValid());

  EXPECT_EQ(block_chain.getBestChain().tip()->header, bootstrap_block);
  EXPECT_EQ(block_chain.getBestChain().tip()->height, first_block_height);

  while (file >> temp) {
    BtcBlock block = BtcBlock::fromRaw(ParseHex(temp));

    EXPECT_TRUE(block_chain.acceptBlock(block, state));
    EXPECT_TRUE(state.IsValid());
    EXPECT_EQ(block_chain.getBestChain().tip()->header, block);
    EXPECT_EQ(block_chain.getBestChain().tip()->height, ++first_block_height);
  }
}

INSTANTIATE_TEST_SUITE_P(AcceptBlocksRegression,
                         AcceptTest,
                         testing::ValuesIn(accept_test_cases));
