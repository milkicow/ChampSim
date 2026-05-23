#include "GAp.h"

bool GAp::predict_branch(champsim::address ip)
{
  auto address_lower_part = ip.slice_lower<champsim::data::bits{ADDRESS_MASKED_BITS_SIZE}>().to<std::size_t>();
  auto counter = m_pattern_history_table[address_lower_part][m_branch_history_register.to_ulong()];
  return counter.value() > (counter.maximum / 2);
}

void GAp::last_branch_result(champsim::address ip, [[maybe_unused]] champsim::address branch_target, bool taken, [[maybe_unused]] uint8_t branch_type)
{
  auto address_lower_part = ip.slice_lower<champsim::data::bits{ADDRESS_MASKED_BITS_SIZE}>().to<std::size_t>();
  m_pattern_history_table[address_lower_part][m_branch_history_register.to_ulong()] += taken ? 1 : -1;

  m_branch_history_register <<= 1;
  m_branch_history_register[0] = taken;
}
