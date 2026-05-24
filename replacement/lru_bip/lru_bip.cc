#include "lru_bip.h"

#include <algorithm>
#include <cassert>

lru_bip::lru_bip(CACHE* cache) : lru_bip(cache, cache->NUM_SET, cache->NUM_WAY) {}

lru_bip::lru_bip(CACHE* cache, long sets, long ways) : replacement(cache), NUM_WAY(ways), last_used_cycles(static_cast<std::size_t>(sets * ways), 0) {}

long lru_bip::find_victim(uint32_t triggering_cpu, uint64_t instr_id, long set, const champsim::cache_block* current_set, champsim::address ip,
                          champsim::address full_addr, access_type type)
{
  auto invalid = std::find_if(current_set, current_set + NUM_WAY, [](const auto& cache_block) { return !cache_block.valid; });
  if (invalid != current_set + NUM_WAY)
    return std::distance(current_set, invalid);

  auto begin = std::next(std::begin(last_used_cycles), set * NUM_WAY);
  auto end = std::next(begin, NUM_WAY);

  // Find the way whose last use cycle is most distant
  auto victim = std::min_element(begin, end);
  assert(begin <= victim);
  assert(victim < end);
  return std::distance(begin, victim);
}

void lru_bip::replacement_cache_fill(uint32_t triggering_cpu, long set, long way, champsim::address full_addr, champsim::address ip,
                                     champsim::address victim_addr, access_type type)
{
  if (++fill_counter % EPSILON == 0) {
    last_used_cycles.at(static_cast<std::size_t>(set * NUM_WAY + way)) = ++cycle;
  } else {
    last_used_cycles.at(static_cast<std::size_t>(set * NUM_WAY + way)) = 0;
  }
}

void lru_bip::update_replacement_state(uint32_t triggering_cpu, long set, long way, champsim::address full_addr, champsim::address ip,
                                       champsim::address victim_addr, access_type type, uint8_t hit)
{
  // Mark the way as being used on the current cycle
  if (hit && access_type{type} != access_type::WRITE) // Skip this for writeback hits
    last_used_cycles.at(static_cast<std::size_t>(set * NUM_WAY + way)) = ++cycle;
}
