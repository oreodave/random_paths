/* strats.cpp: Strategy implementations
 * Created: 2026-02-13
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary:
 */

#include "strats.hpp"

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

bool validate_actor_path(Grid &grid, Grid::Actor &actor)
{
  if (actor.path.size() == 0)
    return false;
  // An actor path can only be composed of n valid cells followed by 1 "end
  // node" cell, where n is a positive integer.  I argue that end node cell is
  // the only thing we need to analyse in order to validate a path.  Consider
  // our preprocesser conditions:
  // ACTORS_REPLACE_CELLS -> Actors may go to any cell, claimed or unclaimed.
  // ACTORS_BACKWARDS_MOVEMENT -> Actors may only go to their cells or unclaimed
  // ones.

  // If the former macro is 1, we have no care - since all cells are valid
  // game, our end node is valid as well regardless of what has happened.
  // Backward movement is irrelevant.
  // If the latter macro is 1, it's only if the cell has been claimed by someone
  // else already that the path is invalid.
  // If neither are true (only possible case after the last two), the cell just
  // needs to be unclaimed.
  auto cell = grid[actor.path[actor.path.size() - 1]];
#if ACTORS_REPLACE_CELLS
  return true;
#elif ACTORS_BACKWARD_MOVEMENT
  return cell == 0 || cell == actor.id;
#else
  return cell == 0;
#endif
}

void actor_step_in_path(Grid &grid, Grid::Actor &actor)
{
  auto next_step  = actor.path[0];
  grid[next_step] = actor.id;
  actor.pos       = next_step;

  if (actor.path.size() == 1)
  {
    actor.path.clear();
  }
  else
  {
    // Inplace delete
    std::swap(actor.path[0], actor.path[actor.path.size() - 1]);
    actor.path.resize(actor.path.size() - 1);
  }
}

/* Copyright (C) 2026 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the Unlicense for details.

 * You may distribute and modify this code under the terms of the Unlicense,
 * which you should have received a copy of along with this program.  If not,
 * please go to <https://unlicense.org/>.

 */
