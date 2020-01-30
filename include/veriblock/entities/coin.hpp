#ifndef ALT_INTEGRATION_INCLUDE_VERIBLOCK_ENTITIES_COIN_HPP_
#define ALT_INTEGRATION_INCLUDE_VERIBLOCK_ENTITIES_COIN_HPP_

#include <cstdint>

#include "veriblock/serde.hpp"

namespace VeriBlock {

struct Coin {
  int64_t units{};

  Coin() = default;

  explicit Coin(int64_t atomicUnits) : units(atomicUnits) {}

  static Coin fromVbkEncoding(ReadStream& stream) {
    return Coin(readSingleBEValue<int64_t>(stream));
  }

  void toVbkEncoding(WriteStream& stream) const {
    writeSingleBEValue(stream, units);
  }

  bool operator==(const Coin& other) const noexcept {
    return units == other.units;
  }
};

}  // namespace VeriBlock
#endif  // ALT_INTEGRATION_INCLUDE_VERIBLOCK_ENTITIES_COIN_HPP_