/* strats.cpp: Strategy implementations
 * Created: 2026-02-13
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary:
 */

#include "strats.hpp"

#include <algorithm>
#include <cstdlib>

void actor_random_strat(Grid &grid, Grid::Actor &actor)
{
  std::array<pair<Coord, colour_t>, 4> neighbours;
  auto [start, size] =
      grid.get_valid_neighbours(actor.pos.x, actor.pos.y, actor.id, neighbours);
  if (start == size)
    return;

  u64 ind            = rand() % (size - start);
  auto [coord, cell] = neighbours[ind + start];

  grid[coord] = actor.id;
  actor.pos   = coord;
}

void actor_teleport_strat(Grid &grid, Grid::Actor &actor)
{
  auto targets = grid.bfs_find_all_candidates(actor, end_unclaimed_node);
  if (targets.size() == 0)
  {
    // can't do anything then
    return;
  }

  // Filter for closest targets
  auto min_element =
      *std::min_element(std::begin(targets), std::end(targets),
                        [&actor](Coord a, Coord b)
                        {
                          return (actor.pos || a) < (actor.pos || b);
                        });
  auto min_distance = actor.pos || min_element;
  std::vector<Coord> closest{};
  closest.reserve(targets.size());
  for (const auto target : targets)
  {
    if (actor.pos || target == min_distance)
    {
      closest.push_back(target);
    }
  }

  if (closest.size() == 0)
    return;

  // Randomly pick a target from the closest ones
  auto target  = closest[rand() % closest.size()];
  grid[target] = actor.id;
  actor.pos    = target;
}

/* Copyright (C) 2026 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the Unlicense for details.

 * You may distribute and modify this code under the terms of the Unlicense,
 * which you should have received a copy of along with this program.  If not,
 * please go to <https://unlicense.org/>.

 */
