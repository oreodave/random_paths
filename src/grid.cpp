/* grid.cpp: Grid implementation
 * Created: 2026-02-13
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary:
 */

#include "./grid.hpp"

#include <algorithm>
#include <array>
#include <cstring>
#include <raylib.h>

Grid::Grid(ActorUpdateFn fn)
{
  cells                 = new u32[GRID_SIZE * GRID_SIZE];
  actor_update_function = fn;
  // NOTE: Debug builds don't seem to get cleared cells of memory from heap
  // memory.  I thought C++ did this by default?  That's wild.
  memset(cells, 0, GRID_SIZE * GRID_SIZE * sizeof(cells[0]));
}

Grid::~Grid(void)
{
  delete[] cells;
}

u32 &Grid::operator[](u64 x, u64 y) const
{
  return cells[(x * GRID_SIZE) + y];
}

u32 &Grid::operator[](Coord p) const
{
  return cells[(p.x * GRID_SIZE) + p.y];
}

tuple<u64, u64, u64> get_cell_dimensions(void)
{
  /*
    How do we want to draw cells?
    - We definitely want them square
    - We want some padding from the edges of the screen for cleanliness.
   */

  // we take the smaller of the two pixels per cell as our overall size.
  u64 cell_dimension =
      std::min((WIDTH / 1.5) / GRID_SIZE, (HEIGHT / 1.5) / GRID_SIZE);
  u64 padding_x = (WIDTH - (cell_dimension * GRID_SIZE)) / 2;
  u64 padding_y = (HEIGHT - (cell_dimension * GRID_SIZE)) / 2;

  return {cell_dimension, padding_x, padding_y};
}

void Grid::draw(void)
{
  auto [cell_dimension, padding_x, padding_y] = get_cell_dimensions();

  for (u64 x = 0; x < GRID_SIZE; ++x)
  {
    for (u64 y = 0; y < GRID_SIZE; ++y)
    {
      u64 cell_x      = (x * cell_dimension) + padding_x;
      u64 cell_y      = (y * cell_dimension) + padding_y;
      u32 cell_colour = (*this)[x, y];
      Color col       = {};
      memcpy(&col, &cell_colour, sizeof(Color));

      DrawRectangle(cell_x, cell_y, cell_dimension, cell_dimension, col);
      // DrawRectangleLines(cell_x, cell_y, cell_dimension, cell_dimension,
      //                    WHITE);
    }
  }

  for (const auto &actor : actors)
  {
    u64 x = (actor.pos.x * cell_dimension) + padding_x;
    u64 y = (actor.pos.y * cell_dimension) + padding_y;
    DrawRectangleLines(x, y, cell_dimension, cell_dimension, BLUE);
  }
}

bool Grid::add_actor(u32 id, Coord pos, Coord target)
{
  if ((*this)[pos] != 0)
    return false;

  for (const auto &actor : actors)
  {
    assert(actor.id != id);
    if (actor.id == id || (actor.pos.x == pos.x && actor.pos.y == pos.y))
      return false;
  }
  actors.push_back({id, pos, target});
  cells[(pos.x * GRID_SIZE) + pos.y] = id;
  return true;
}

u64 Grid::get_neighbours(u64 x, u64 y,
                         std::array<pair<Coord, u32>, 4> &arr) const
{
  assert(x < GRID_SIZE && y < GRID_SIZE);
  u64 size = 0;
  if (x > 0)
    arr[size++] = {{x - 1, y}, cells[((x - 1) * GRID_SIZE) + y]};
  if (x < GRID_SIZE - 1)
    arr[size++] = {{x + 1, y}, cells[((x + 1) * GRID_SIZE) + y]};
  if (y > 0)
    arr[size++] = {{x, y - 1}, cells[(x * GRID_SIZE) + (y - 1)]};
  if (y < GRID_SIZE - 1)
    arr[size++] = {{x, y + 1}, cells[(x * GRID_SIZE) + (y + 1)]};
  return size;
}

pair<u64, u64>
Grid::get_valid_neighbours(u64 x, u64 y, u32 actor_id,
                           std::array<pair<Coord, u32>, 4> &arr) const
{
  u64 size = get_neighbours(x, y, arr);

  u64 start = 0;
#if !ACTORS_BACKWARD_MOVEMENT
  for (u64 i = start; i < size; ++i)
  {
    if (arr[i].second == actor_id)
    {
      std::swap(arr[start], arr[i]);
      ++start;
    }
  }
#endif
#if !ACTORS_REPLACE_CELLS
  for (u64 i = start; i < size; ++i)
  {
    if (!(arr[i].second == actor_id || arr[i].second == 0))
    {
      std::swap(arr[start], arr[i]);
      ++start;
    }
  }
#endif
  return {start, size};
}

void Grid::update(void)
{
  std::for_each(std::begin(actors), std::end(actors),
                [this](auto &actor)
                {
                  this->actor_update_function(*this, actor);
                });
}

/* Copyright (C) 2026 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the Unlicense for details.

 * You may distribute and modify this code under the terms of the Unlicense,
 * which you should have received a copy of along with this program.  If not,
 * please go to <https://unlicense.org/>.

 */
