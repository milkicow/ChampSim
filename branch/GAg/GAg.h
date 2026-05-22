#ifndef BRANCH_GAG_H
#define BRANCH_GAG_H

#include <array>
#include <bitset>

#include "modules.h"
#include "msl/fwcounter.h"

struct GAg : champsim::modules::branch_predictor {
  static constexpr std::size_t BRANCH_HISTORY_REGISTER_SIZE = 14;
  static constexpr std::size_t COUNTER_BITS = 2;
  static constexpr std::size_t GS_HISTORY_TABLE_SIZE = 1 << BRANCH_HISTORY_REGISTER_SIZE;

  std::bitset<BRANCH_HISTORY_REGISTER_SIZE> m_branch_history_register{};
  std::array<champsim::msl::fwcounter<COUNTER_BITS>, GS_HISTORY_TABLE_SIZE> m_pattern_history_table{};

  using branch_predictor::branch_predictor;

  bool predict_branch(champsim::address ip);
  void last_branch_result(champsim::address ip, champsim::address branch_target, bool taken, uint8_t branch_type);
};

#endif // BRANCH_GAG_H
