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
};

struct Actor
{
  bool done; // true if cannot colour any further
  u32 id;
  Coord pos, target;
};

struct Grid
{
  u32 *cells;
  std::vector<Actor> actors;
  using ActorUpdateFn = void (*)(Grid &grid, Actor &actor);
  ActorUpdateFn actor_update_function;

  Grid(ActorUpdateFn);
  ~Grid(void);
  bool add_actor(u32 id, Coord pos, Coord target);
  u32 &operator[](u64 x, u64 y) const;
  u32 &operator[](Coord p) const;

  void draw(void);
  void update(void);

  u64 get_neighbours(u64 x, u64 y, std::array<pair<Coord, u32>, 4> &arr) const;
  pair<u64, u64>
  get_valid_neighbours(u64 x, u64 y, u32 actor_id,
                       std::array<pair<Coord, u32>, 4> &arr) const;
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
