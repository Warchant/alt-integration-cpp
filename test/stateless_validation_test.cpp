// Copyright (c) 2019-2020 Xenios SEZC
// https://www.veriblock.org
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

#include <gtest/gtest.h>

#include "util/alt_chain_params_regtest.hpp"
#include "util/test_utils.hpp"
#include "veriblock/blockchain/btc_chain_params.hpp"
#include "veriblock/blockchain/vbk_chain_params.hpp"
#include "veriblock/literals.hpp"
#include "veriblock/stateless_validation.hpp"

using namespace altintegration;

static const BtcBlock validBtcBlock = {
    536870912,
    uint256(
        "00000000000000b345b7bbf29bda1507a679b97967f99a10ab0088899529def7"_unhex),
    uint256(
        "5e16e6cef738a2eba1fe7409318e3f558bec325392427aa3d8eaf46b028654f8"_unhex),
    1555501858,
    436279940,
    2599551022};

static const VbkBlock validVbkBlock = {
    5000,
    2,
    uint96("94E7DC3E3BE21A96ECCF0FBD"_unhex),
    uint72("F5F62A3331DC995C36"_unhex),
    uint72("B0935637860679DDD5"_unhex),
    uint128("DB0F135312B2C27867C9A83EF1B99B98"_unhex),
    1553699987,
    117586646,
    1924857207};

static const VbkPopTx validPopTx = {
    {false, 0, (uint8_t)TxType::VBK_POP_TX},
    Address::fromString("VE6MJFzmGdYdrxC8o6UCovVv7BdhdX"),
    VbkBlock::fromHex(
        "000013350002A793C872D6F6460E90BED62342BB968195F8C515D3EED7277A09EFAC4B"
        "E99F95F0A15628B06BA3B44C0190B5C0495C9B8ACD0701C5235EBBBE9C"),
    BtcTx("01000000010CE74F1FB694A001EEBB1D7D08CE6208033F5BF7263EBAD2DE07BBF518"
          "672732000000006A47304402200CF4998ABA1682ABEB777E762807A9DD2635A0B777"
          "73F66491B83EE3C87099BA022033B7CA24DC520915B8B0200CBDCF95BA6AE8663545"
          "85AF9C53EE86F27362EBEC012103E5BAF0709C395A82EF0BD63BC8847564AC201D69"
          "A8E6BF448D87AA53A1C431AAFFFFFFFF02B7270D00000000001976A9148B9EA85450"
          "59F3A922457AFD14DDF3855D8B109988AC0000000000000000536A4C500000133500"
          "02A793C872D6F6460E90BED62342BB968195F8C515D3EED7277A09EFAC4BE99F95F0"
          "A15628B06BA3B44C0190B5C0495C9B8ACD0701C5235EBBBE9CD4E943EFE1864DF042"
          "16615CF92083F400000000"_unhex),
    {1659,
     uint256(
         "94E097B110BA3ADBB7B6C4C599D31D675DE7BE6E722407410C08EF352BE585F1"_unhex),
     {uint256(
          "4D66077FDF24246FFD6B6979DFEDEF5D46588654ADDEB35EDB11E993C131F612"_unhex),
      uint256(
          "023D1ABE8758C6F917EC0C65674BBD43D66EE14DC667B3117DFC44690C6F5AF1"_unhex),
      uint256(
          "096DDBA03CA952AF133FB06307C24171E53BF50AB76F1EDEABDE5E99F78D4EAD"_unhex),
      uint256(
          "2F32CF1BEE50349D56FC1943AF84F2D2ABDA520F64DC4DB37B2F3DB20B0ECB57"_unhex),
      uint256(
          "93E70120F1B539D0C1495B368061129F30D35F9E436F32D69967AE86031A2756"_unhex),
      uint256(
          "F554378A116E2142F9F6315A38B19BD8A1B2E6DC31201F2D37A058F03C39C06C"_unhex),
      uint256(
          "0824705685CECA003C95140434EE9D8BBBF4474B83FD4ECC2766137DB9A44D74"_unhex),
      uint256(
          "B7B9E52F3EE8CE4FBB8BE7D6CF66D33A20293F806C69385136662A74453FB162"_unhex),
      uint256(
          "1732C9A35E80D4796BABEA76AACE50B49F6079EA3E349F026B4491CFE720AD17"_unhex),
      uint256(
          "2D9B57E92AB51FE28A587050FD82ABB30ABD699A5CE8B54E7CD49B2A827BCB99"_unhex),
      uint256(
          "DCBA229ACDC6B7F028BA756FD5ABBFEBD31B4227CD4137D728EC5EA56C457618"_unhex),
      uint256(
          "2CF1439A6DBCC1A35E96574BDDBF2C5DB9174AF5AD0D278FE92E06E4AC349A42"_unhex)}},
    BtcBlock::fromRaw(
        "0000C020134F09D43659EB53982D9AFB444B96FA4BB58C037D2914000000000000000000CE0B1A9A77DD0DB127B5DF4BC368CD6AC299A9747D991EC2DACBC0B699A2E4A5B3919B5C6C1F2C1773703BC0"_unhex),
    {BtcBlock::fromRaw(
         "00008020FC61CC9D4EAC4B2D14761A4D06AF8A9EF073DCD7FB5E0D000000000000000000A31508D4B101D0AD11E43EF9419C23FC277F67EDAE83C598EE70866DBCEF5E25268B9B5C6C1F2C17E11874AF"_unhex),
     BtcBlock::fromRaw(
         "000040203F8E3980304439D853C302F6E496285E110E251251531300000000000000000039A72C22268381BD8D9DCFE002F472634A24CF0454DE8B50F89E10891E5FFB1DE08D9B5C6C1F2C1744290A92"_unhex),
     BtcBlock::fromRaw(
         "00000020BAA42E40345A7F826A31D37DB1A5D64B67B72732477422000000000000000000A33AD6BE0634647B26633AB85FA8DE258480BBB25E59C68E48BB0B608B12362B10919B5C6C1F2C1749C4D1F0"_unhex)},
    "3045022100F4DCE45EDCC6BFC4A1F44EF04E47E90A348EFD471F742F18B882AC77A8D0E89E0220617CF7C4A22211991687B17126C1BB007A3B2A25C550F75D66B857A8FD9D75E7"_unhex,
    "3056301006072A8648CE3D020106052B8104000A03420004B3C10470C8E8E426F1937758D9FB5E97A1891176CB37D4C12D4AF4107B1AA3E8A8A754C06A22760E44C60642FBA883967C19740D5231336326F7962750C8DF99"_unhex};

const static VbkTx validVbkTx = {
    {false, 0, (uint8_t)TxType::VBK_TX},
    Address::fromString("V5Ujv72h4jEBcKnALGc4fKqs6CDAPX"),
    Coin(1000),
    std::vector<Output>(),
    7,
    PublicationData::fromRaw(
        "0100010C6865616465722062797465730112636F6E7465787420696E666F20627974657301117061796F757420696E666F206279746573"_unhex),
    "30440220398B74708DC8F8AEE68FCE0C47B8959E6FCE6354665DA3ED87A83F708E62AA6B02202E6C00C00487763C55E92C7B8E1DD538B7375D8DF2B2117E75ACBB9DB7DEB3C7"_unhex,
    "3056301006072A8648CE3D020106052B8104000A03420004DE4EE8300C3CD99E913536CF53C4ADD179F048F8FE90E5ADF3ED19668DD1DBF6C2D8E692B1D36EAC7187950620A28838DA60A8C9DD60190C14C59B82CB90319E"_unhex};

static const ATV validATV = {
    validVbkTx,
    {1,
     0,
     uint256(
         "1FEC8AA4983D69395010E4D18CD8B943749D5B4F575E88A375DEBDC5ED22531C"_unhex),
     {uint256(
          "0000000000000000000000000000000000000000000000000000000000000000"_unhex),
      uint256(
          "0000000000000000000000000000000000000000000000000000000000000000"_unhex)}},
    {5000,
     2,
     uint96(),
     uint72(),
     uint72(),
     uint128("26BBFDA7D5E4462EF24AE02D67E47D78"_unhex),
     1553699059,
     16842752,
     1},
};

static const VTB validVTB = {
    validPopTx,
    {0,
     13,
     uint256(
         "2A014E88ED7AB65CDFAA85DAEAB07EEA6CBA5E147F736EDD8D02C2F9DDF0DEC6"_unhex),
     {uint256(
          "5B977EA09A554AD56957F662284044E7D37450DDADF7DB3647712F5969399787"_unhex),
      uint256(
          "20D0A3D873EEEEE6A222A75316DCE60B53CA43EAEA09D27F0ECE897303A53AE9"_unhex),
      uint256(
          "C06FE913DCA5DC2736563B80834D69E6DFDF1B1E92383EA62791E410421B6C11"_unhex),
      uint256(
          "049F68D350EEB8B3DF630C8308B5C8C2BA4CD6210868395B084AF84D19FF0E90"_unhex),
      uint256(
          "0000000000000000000000000000000000000000000000000000000000000000"_unhex),
      uint256(
          "36252DFC621DE420FB083AD9D8767CBA627EDDEEC64E421E9576CEE21297DD0A"_unhex)}},
    VbkBlock::fromHex(
        "000013700002449C60619294546AD825AF03B0935637860679DDD55EE4FD21082E1868"
        "6EB53C1F4E259E6A0DF23721A0B3B4B7AB5C9B9211070211CAF01C3F01"),
};

struct StatelessValidationTest : public ::testing::Test {
  BtcChainParamsRegTest btc;
  VbkChainParamsRegTest vbk;
  ValidationState state;
};

TEST_F(StatelessValidationTest, checkBtcBlock_when_valid_test) {
  ASSERT_TRUE(checkBlock(validBtcBlock, state, btc));
}

TEST_F(StatelessValidationTest,
       checkBtcBlock_when_checkProofOfWork_invalid_test) {
  BtcBlock block = validBtcBlock;
  block.nonce = 1;
  ASSERT_FALSE(checkProofOfWork(block, btc));
}

TEST_F(StatelessValidationTest, check_valid_vbk_block) {
  ASSERT_TRUE(checkBlock(validVbkBlock, state, vbk));
}

TEST_F(StatelessValidationTest,
       checkVbkBlock_when_checkProofOfWork_invalid_test) {
  VbkBlock block = validVbkBlock;
  block.nonce = 1;
  ASSERT_FALSE(checkProofOfWork(block, vbk));
}

TEST_F(StatelessValidationTest, ATV_valid) {
  AltChainParamsRegTest altp;
  ASSERT_TRUE(checkATV(validATV, state, altp)) << state.GetDebugMessage();
}

TEST_F(StatelessValidationTest,
       ATV_checkMerklePath_different_transaction_invalid) {
  ATV atv = validATV;
  atv.merklePath.subject = uint256(
      "5B977EA09A554AD56957F662284044E7D37450DDADF7DB3647712F5969399787"_unhex);

  ASSERT_FALSE(checkMerklePath(atv.merklePath,
                               atv.transaction.getHash(),
                               atv.blockOfProof.merkleRoot,
                               state));
}

TEST_F(StatelessValidationTest,
       ATV_checkMerklePath_merkleRoot_dont_match_ivalid) {
  ATV atv = validATV;
  atv.blockOfProof.merkleRoot =
      uint128("0356EB39B851682679F9A0131A4E4A5F"_unhex);

  ASSERT_FALSE(checkMerklePath(atv.merklePath,
                               atv.transaction.getHash(),
                               atv.blockOfProof.merkleRoot,
                               state));
}

TEST_F(StatelessValidationTest, VTB_valid) {
  ASSERT_TRUE(checkVTB(validVTB, state, btc));
}

TEST_F(StatelessValidationTest,
       VTB_checkMerklePath_different_transaction_invalid) {
  VTB vtb = validVTB;
  vtb.merklePath.subject = uint256(
      "3A014E88ED7AB65CDFAA85DAEAB07EEA6CBA5E147F736EDD8D02C2F9DDF0DEC6"_unhex);

  ASSERT_FALSE(checkMerklePath(vtb.merklePath,
                               vtb.transaction.getHash(),
                               vtb.containingBlock.merkleRoot,
                               state));
}

TEST_F(StatelessValidationTest,
       VTB_checkMerklePath_merkleRoot_dont_match_ivalid) {
  VTB vtb = validVTB;
  vtb.containingBlock.merkleRoot =
      uint128("0356EB39B851682679F9A0131A4E4A5F"_unhex);
  ASSERT_FALSE(checkMerklePath(vtb.merklePath,
                               vtb.transaction.getHash(),
                               vtb.containingBlock.merkleRoot,
                               state));
}

TEST_F(StatelessValidationTest, checkVbkPopTx_valid) {
  ASSERT_TRUE(checkVbkPopTx(validPopTx, state, btc));
}

TEST_F(StatelessValidationTest, VbkPopTx_checkSignature_signature_invalid) {
  VbkPopTx tx = validPopTx;
  tx.signature =
      "30450220034DC73796E9870E6679F47E48F3AC794327FD19D9023C228CD134D8ED87B796"
      "022100AD0CE8A520AAE704447920CA365D57A881A82A7455293A9C10E622E0BDD732AF"_unhex;
  ASSERT_FALSE(checkSignature(tx, state));
}

TEST_F(StatelessValidationTest, VbkPopTx_different_address_invalid) {
  VbkPopTx tx = validPopTx;
  tx.publicKey =
      "3056301006072A8648CE3D020106052B8104000A03420004DE4EE8300C3CD99E913536CF53C4ADD179F048F8FE90E5ADF3ED19668DD1DBF6C2D8E692B1D36EAC7187950620A28838DA60A8C9DD60190C14C59B82CB90319E"_unhex;
  ASSERT_FALSE(checkSignature(tx, state));
}

TEST_F(StatelessValidationTest, checkBitcoinTransactionForPoPData_invalid) {
  VbkPopTx tx = validPopTx;
  tx.publishedBlock = VbkBlock::fromHex(
      "00001388000294E7DC3E3BE21A96ECCF0FBDF5F62A3331DC995C36B0935637860679DDD5"
      "DB0F135312B2C27867C9A83EF1B99B985C9B949307023AD672BAFD77");
  ASSERT_FALSE(checkBitcoinTransactionForPoPData(tx, state));
}

TEST_F(StatelessValidationTest,
       checkBitcoinMerklePath_different_transaction_invalid) {
  VbkPopTx tx = validPopTx;
  tx.merklePath.subject = uint256(
      "012A4E88ED7AB65CDFAA85DAEAB07EEA6CBA5E147F736EDD8D02C2F9DDF0DEC6"_unhex);
  ASSERT_FALSE(checkMerklePath(tx.merklePath,
                               tx.bitcoinTransaction.getHash(),
                               tx.blockOfProof.merkleRoot,
                               state));
}

TEST_F(StatelessValidationTest,
       checkBitcoinMerklePath_merkle_root_do_not_match_invalid) {
  VbkPopTx tx = validPopTx;
  tx.blockOfProof = BtcBlock::fromRaw(
      "00000020BAA42E40345A7F826A31D37DB1A5D64B67B72732477422000000000000000000A33AD6BE0634647B26633AB85FA8DE258480BBB25E59C68E48BB0B608B12362B10919B5C6C1F2C1749C4D1F0"_unhex);
  ASSERT_FALSE(checkMerklePath(tx.merklePath,
                               tx.bitcoinTransaction.getHash(),
                               tx.blockOfProof.merkleRoot,
                               state));
}

TEST_F(StatelessValidationTest, checkBitcoinBlocks_when_not_contiguous) {
  VbkPopTx tx = validPopTx;
  tx.blockOfProofContext.erase(tx.blockOfProofContext.begin() + 1);
  ASSERT_FALSE(checkBtcBlocks(tx.blockOfProofContext, state, btc));
}

TEST_F(StatelessValidationTest, checkVbkTx_valid) {
  ASSERT_TRUE(checkVbkTx(validVbkTx, state));
}

TEST_F(StatelessValidationTest, VbkTx_checkSignature_signature_invalid) {
  VbkTx tx = validVbkTx;
  tx.signature =
      "30440220398B74708DC8F8AEE68FCE0C47B8959E6FCE6354665DA3ED87583F708E62AA6B02202E6C00C00487763C55E92C7B8E1DD538B7375D8DF2B2117E75ACBB9DB7DEB3C7"_unhex;
  ASSERT_FALSE(checkVbkTx(tx, state));
}

TEST_F(StatelessValidationTest, VbkTx_different_address_invalid) {
  VbkTx tx = validVbkTx;
  tx.publicKey =
      "3056301006072A8648CE3D020106552B8104000A03420004DE4EE8300C3CD99E913536CF53C4ADD179F048F8FE90E5ADF3ED19668DD1DBF6C2D8E692B1D36EAC7187950620A28838DA60A8C9DD60190C14C59B82CB90319E"_unhex;
  ASSERT_FALSE(checkSignature(tx, state));
}

TEST_F(StatelessValidationTest, containsSplit_when_descriptor_before_chunks) {
  srand(0);

  WriteStream buffer;
  buffer.write(generateRandomBytesVector(15));

  // Descriptor bytes (3 MAGIC, 1 SIZE, 7 SECTIONALS)
  buffer.write("927A594624509D41F548C0"_unhex);

  buffer.write(generateRandomBytesVector(10));

  // First chunk of 20 bytes
  buffer.write("00000767000193093228BD2B4906F6B84BE5E618"_unhex);

  buffer.write(generateRandomBytesVector(39));

  // Second chunk of 20 bytes
  buffer.write("09C0522626145DDFB988022A0684E2110D384FE2"_unhex);

  buffer.write(generateRandomBytesVector(31));

  // Third chunk of 21 bytes
  buffer.write("BFD38549CB19C41893C258BA5B9CAB24060BA2D410"_unhex);

  buffer.write(generateRandomBytesVector(35));

  // Fourth chunk of unstated 19 bytes
  buffer.write("39DFC857801424B0F5DE63992A016F5F38FEB4"_unhex);

  buffer.write(generateRandomBytesVector(22));

  ASSERT_TRUE(containsSplit(
      "00000767000193093228BD2B4906F6B84BE5E61809C0522626145DDFB988022A0684E2110D384FE2BFD38549CB19C41893C258BA5B9CAB24060BA2D41039DFC857801424B0F5DE63992A016F5F38FEB4"_unhex,
      buffer.data()));
}

TEST_F(StatelessValidationTest, containsSplit_when_chunked) {
  srand(0);

  WriteStream buffer;
  buffer.write(generateRandomBytesVector(15));

  // Descriptor bytes (3 MAGIC, 1 SIZE, 7 SECTIONALS)
  buffer.write("927A594624509D41F548C0"_unhex);

  buffer.write(generateRandomBytesVector(10));

  buffer.write("00000767000193093228BD2B4906F6B84BE5E618"_unhex);

  buffer.write(generateRandomBytesVector(39));

  // Second chunk of 20 bytes
  buffer.write("09C0522626145DDFB988022A0684E2110D384FE2"_unhex);

  buffer.write(generateRandomBytesVector(31));

  // Third chunk of 21 bytes
  buffer.write("BFD38549CB19C41893C258BA5B9CAB24060BA2D410"_unhex);

  buffer.write(generateRandomBytesVector(35));

  // Fourth chunk of unstated 19 bytes
  buffer.write("39DFC857801424B0F5DE63992A016F5F38FEB4"_unhex);

  buffer.write(generateRandomBytesVector(22));

  ASSERT_TRUE(containsSplit(
      "00000767000193093228BD2B4906F6B84BE5E61809C0522626145DDFB988022A0684E2110D384FE2BFD38549CB19C41893C258BA5B9CAB24060BA2D41039DFC857801424B0F5DE63992A016F5F38FEB4"_unhex,
      buffer.data()));
}
