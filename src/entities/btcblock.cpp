// Copyright (c) 2019-2020 Xenios SEZC
// https://www.veriblock.org
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

#include "veriblock/entities/btcblock.hpp"

using namespace altintegration;

const std::string BtcBlock::_name = "BTC";

BtcBlock BtcBlock::fromRaw(const std::vector<uint8_t>& bytes) {
  ReadStream stream(bytes);
  return fromRaw(stream);
}

BtcBlock BtcBlock::fromRaw(ReadStream& stream) {
  BtcBlock block{};
  block.version = stream.readLE<uint32_t>();
  block.previousBlock = stream.readSlice(SHA256_HASH_SIZE).reverse();
  block.merkleRoot = stream.readSlice(SHA256_HASH_SIZE).reverse();
  block.timestamp = stream.readLE<uint32_t>();
  block.bits = stream.readLE<uint32_t>();
  block.nonce = stream.readLE<uint32_t>();
  return block;
}

BtcBlock BtcBlock::fromVbkEncoding(ReadStream& stream) {
  ReadStream valStream(
      readSingleByteLenValue(stream, BTC_HEADER_SIZE, BTC_HEADER_SIZE));
  return BtcBlock::fromRaw(valStream);
}

void BtcBlock::toRaw(WriteStream& stream) const {
  stream.writeLE<uint32_t>(version);
  stream.write(previousBlock.reverse());
  stream.write(merkleRoot.reverse());
  stream.writeLE<uint32_t>(timestamp);
  stream.writeLE<uint32_t>(bits);
  stream.writeLE<uint32_t>(nonce);
}

void BtcBlock::toVbkEncoding(WriteStream& stream) const {
  WriteStream blockStream;
  toRaw(blockStream);
  writeSingleByteLenValue(stream, blockStream.data());
}

uint32_t BtcBlock::getDifficulty() const { return bits; }

uint32_t BtcBlock::getBlockTime() const { return timestamp; }

uint256 BtcBlock::getHash() const {
  WriteStream stream;
  toRaw(stream);
  return sha256twice(stream.data()).reverse();
}

BtcBlock BtcBlock::fromHex(const std::string& hex) {
  auto v = ParseHex(hex);
  return BtcBlock::fromRaw(v);
}

std::string BtcBlock::toHex() const { return HexStr(this->toRaw()); }

std::vector<uint8_t> BtcBlock::toRaw() const {
  WriteStream stream;
  this->toRaw(stream);
  return stream.data();
}

std::string BtcBlock::toPrettyString() const {
  return fmt::sprintf(
      "BtcBlock{version=%lu, prev=%s, merkleRoot=%s, timestamp=%lu, "
      "bits=%lu, nonce=%lu}",
      version,
      previousBlock.toHex(),
      merkleRoot.toHex(),
      timestamp,
      bits,
      nonce);
}
