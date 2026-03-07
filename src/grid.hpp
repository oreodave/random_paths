/* grid.hpp: Grid definitions
 * Created: 2026-02-13
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary:
 */

#ifndef GRID_HPP
#define GRID_HPP

#include "./base.hpp"

#include <array>
#include <vector>

struct Coord
{
  u64 x, y;

  Coord();
  Coord(u64);
  Coord(u64, u64);
  u64 to_abs(void);

  bool operator==(Coord);
};

using colour_t = u32;

struct Grid
{
  struct Actor
  {
    colour_t id;
    Coord pos, target;
  };

  colour_t *cells;
  std::vector<Actor> actors;
  using ActorUpdateFn = void (*)(Grid &grid, Actor &actor);
  ActorUpdateFn actor_update_function;

  Grid(ActorUpdateFn);
  ~Grid(void);

  bool add_actor(colour_t id, Coord pos, Coord target);
  colour_t &operator[](u64 x, u64 y) const;
  colour_t &operator[](Coord p) const;

  void draw(void);
  void update(void);
  void cull(void);

  u64 get_neighbours(u64 x, u64 y,
                     std::array<pair<Coord, colour_t>, 4> &arr) const;

  pair<u64, u64>
  get_valid_neighbours(u64 x, u64 y, colour_t actor_id,
                       std::array<pair<Coord, colour_t>, 4> &arr) const;

  std::vector<Coord> bfs(Coord start, Coord end);
  std::vector<Coord> bfs_if(Coord start, bool (*end_node)(Coord, colour_t));
};

#endif

/* Copyright (C) 2026 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the Unlicense for details.

 * You may distribute and modify this code under the terms of the Unlicense,
 * which you should have received a copy of along with this program.  If not,
 * please go to <https://unlicense.org/>.

 */
