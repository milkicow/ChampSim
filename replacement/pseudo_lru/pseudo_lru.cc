#include "pseudo_lru.h"

pseudo_lru::pseudo_lru(CACHE* cache)
    : replacement(cache), NUM_SET(cache->NUM_SET), NUM_WAY(cache->NUM_WAY), m_tree(static_cast<std::size_t>(cache->NUM_SET * (cache->NUM_WAY - 1)), 0)
{
  assert((NUM_WAY & (NUM_WAY - 1)) == 0 && "tree-PLRU requires NUM_WAY == power of 2");
}

long pseudo_lru::find_victim(uint32_t triggering_cpu, uint64_t instr_id, long set, const champsim::cache_block* current_set, champsim::address ip,
                             champsim::address full_addr, access_type type)
{
  long node = 0;
  const long num_nodes = NUM_WAY - 1;
  while (node < num_nodes) {
    if (m_tree[static_cast<std::size_t>(set * num_nodes + node)] == 0)
      node = 2 * node + 1; // go left
    else
      node = 2 * node + 2; // go right
  }
  return node - num_nodes;
}

void pseudo_lru::update_tree(long set, long way)
{
  const long num_nodes = NUM_WAY - 1;
  long node = num_nodes + way;
  while (node > 0) {
    long parent = (node - 1) / 2;
    // if node is left child, point right else point left
    m_tree[static_cast<std::size_t>(set * num_nodes + parent)] = (node == 2 * parent + 1) ? 1 : 0;
    node = parent;
  }
}

void pseudo_lru::replacement_cache_fill(uint32_t triggering_cpu, long set, long way, champsim::address full_addr, champsim::address ip,
                                        champsim::address victim_addr, access_type type)
{
  update_tree(set, way);
}

void pseudo_lru::update_replacement_state(uint32_t triggering_cpu, long set, long way, champsim::address full_addr, champsim::address ip,
                                          champsim::address victim_addr, access_type type, uint8_t hit)
{
  if (hit && access_type{type} != access_type::WRITE)
    update_tree(set, way);
}
