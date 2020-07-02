// Copyright (c) 2019-2020 Xenios SEZC
// https://www.veriblock.org
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ALT_INTEGRATION_INCLUDE_VERIBLOCK_STORAGE_STORAGE_MANAGER_HPP_
#define ALT_INTEGRATION_INCLUDE_VERIBLOCK_STORAGE_STORAGE_MANAGER_HPP_

#include <veriblock/storage/payloads_storage.hpp>
#include <veriblock/storage/pop_storage.hpp>

namespace altintegration {

struct StorageManager {
  virtual ~StorageManager() = default;

  virtual void flush() = 0;
  virtual PayloadsStorage newPayloadsStorage() = 0;
  virtual PopStorage newPopStorage() = 0;
};

}  // namespace altintegration

#endif  // ALT_INTEGRATION_INCLUDE_VERIBLOCK_STORAGE_STORAGE_MANAGER_HPP_
