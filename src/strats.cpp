/* strats.cpp: Strategy implementations
 * Created: 2026-02-13
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary:
 */

#include "strats.hpp"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <unordered_set>

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

void actor_uncoloured_strat(Grid &grid, Grid::Actor &actor)
{
  // Perform a BFS against neighbours until we find an uncoloured cell, then
  // move in the principal direction that allows us to get there.
  std::queue<std::pair<Coord, std::vector<Coord>>> queue;
  std::unordered_set<u64> visited;

  std::vector<std::vector<Coord>> best_paths;

  queue.push({actor.pos, {}});
  while (!queue.empty())
  {
    auto [coord, path] = queue.front();
    queue.pop();
    if (visited.find(coord.to_abs()) != std::end(visited))
      continue;
    visited.insert(coord.to_abs());

    std::array<pair<Coord, u32>, 4> neighbours;
    auto [start, size] =
        grid.get_valid_neighbours(coord.x, coord.y, actor.id, neighbours);

    if (start == size)
      continue;

    bool found_valid_path = false;
    for (auto i = start; i < size; ++i)
    {
      auto [other_coord, colour] = neighbours[i];
      auto new_path              = path;
      new_path.push_back(other_coord);
      if (colour != actor.id)
      {
        best_paths.push_back(std::move(new_path));
        found_valid_path = true;
      }
    }

    if (!found_valid_path)
    {
      for (auto i = start; i < size; ++i)
      {
        auto [other_coord, colour] = neighbours[i];
        auto new_path              = path;
        new_path.push_back(other_coord);
        if (colour == actor.id)
        {
          queue.push({other_coord, std::move(new_path)});
        }
      }
    }
  }

  if (best_paths.size() == 0)
    return;

  auto best_path     = best_paths[rand() % best_paths.size()];
  grid[best_path[0]] = actor.id;
  actor.pos          = best_path[0];
}

/* Copyright (C) 2026 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the Unlicense for details.

 * You may distribute and modify this code under the terms of the Unlicense,
 * which you should have received a copy of along with this program.  If not,
 * please go to <https://unlicense.org/>.

 */
