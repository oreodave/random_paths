/* strats.cpp: Strategy implementations
 * Created: 2026-02-13
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary:
 */

#include "strats.hpp"

#include <stdlib.h>
#include <string.h>

void actor_random_strat(Grid &grid, Actor &actor)
{
  std::array<pair<Coord, u32>, 4> neighbours;
  u64 size  = grid.get_neighbours(actor.pos.x, actor.pos.y, neighbours);
  u64 start = 0;
#if !ACTORS_BACKWARD_MOVEMENT
  for (u64 i = start; i < size; ++i)
  {
    if (neighbours[i].second == actor.id)
    {
      std::swap(neighbours[start], neighbours[i]);
      ++start;
    }
  }
#endif
#if !ACTORS_REPLACE_CELLS
  for (u64 i = start; i < size; ++i)
  {
    if (!(neighbours[i].second == actor.id || neighbours[i].second == 0))
    {
      std::swap(neighbours[start], neighbours[i]);
      ++start;
    }
  }
#endif
  if (start == size)
    return;

  u64 ind            = rand() % (size - start);
  auto [coord, cell] = neighbours[ind + start];

  grid[coord] = actor.id;
  actor.pos   = coord;
}

/* Copyright (C) 2026 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the Unlicense for details.

 * You may distribute and modify this code under the terms of the Unlicense,
 * which you should have received a copy of along with this program.  If not,
 * please go to <https://unlicense.org/>.

 */
