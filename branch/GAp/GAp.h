#ifndef BRANCH_GAP_H
#define BRANCH_GAP_H

#include <array>
#include <bitset>

#include "modules.h"
#include "msl/fwcounter.h"

struct GAp : champsim::modules::branch_predictor {
  static constexpr std::size_t BRANCH_HISTORY_REGISTER_SIZE = 7;

  static constexpr std::size_t ADDRESS_MASKED_BITS_SIZE = 7;

  static constexpr std::size_t PATTERN_HISTORY_TABLE_ADDRESSES = 1 << ADDRESS_MASKED_BITS_SIZE;
  static constexpr std::size_t PATTERN_HISTORY_TABLE_HISTORY = 1 << BRANCH_HISTORY_REGISTER_SIZE;

  static constexpr std::size_t COUNTER_BITS = 2;

  std::bitset<BRANCH_HISTORY_REGISTER_SIZE> m_branch_history_register{};
  std::array<std::array<champsim::msl::fwcounter<COUNTER_BITS>, PATTERN_HISTORY_TABLE_HISTORY>, PATTERN_HISTORY_TABLE_ADDRESSES> m_pattern_history_table{};

  using branch_predictor::branch_predictor;

  bool predict_branch(champsim::address ip);
  void last_branch_result(champsim::address ip, champsim::address branch_target, bool taken, uint8_t branch_type);
};

#endif // BRANCH_GAP_H
