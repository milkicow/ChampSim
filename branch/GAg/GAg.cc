#include "GAg.h"

bool GAg::predict_branch([[maybe_unused]] champsim::address ip)
{
  auto counter = m_pattern_history_table[m_branch_history_register.to_ulong()];
  return counter.value() > (counter.maximum / 2);
}

void GAg::last_branch_result([[maybe_unused]] champsim::address ip, [[maybe_unused]] champsim::address branch_target, bool taken,
                             [[maybe_unused]] uint8_t branch_type)
{
  m_pattern_history_table[m_branch_history_register.to_ulong()] += taken ? 1 : -1;

  m_branch_history_register <<= 1;
  m_branch_history_register[0] = taken;
}
