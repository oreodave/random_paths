/* main.cpp: Entrypoint
 * Created: 2026-02-12
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary:
 */

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <optional>
#include <raylib.h>
#include <raymath.h>
#include <tuple>
#include <vector>

#include "./prick_aliases.h"

template <typename T>
using opt = std::optional<T>;
using std::pair;
using std::tuple;

constexpr u64 WIDTH       = 1024;
constexpr u64 HEIGHT      = 1024;
constexpr u64 GRID_WIDTH  = 128;
constexpr u64 GRID_HEIGHT = 128;
constexpr u64 NUM_ACTORS  = 100;
constexpr f32 DELTA_TIME  = 100;

// should actors be allowed to go back on themselves?
#define ACTORS_BACKWARD_MOVEMENT 1
// should actors be able to replace other cells?
#define ACTORS_REPLACE_CELLS 0

struct Actor
{
  u32 id;
  u64 x, y;
};

struct Grid
{
  u32 *cells;
  std::vector<Actor> actors;

  Grid()
  {
    cells = new u32[GRID_WIDTH * GRID_HEIGHT];
    // NOTE: Debug builds don't seem to get cleared cells of memory from heap
    // memory.  I thought C++ did this by default?  That's wild.
    memset(cells, 0, GRID_WIDTH * GRID_HEIGHT * sizeof(cells[0]));
  }

  ~Grid(void)
  {
    delete[] cells;
  }

  u32 &operator[](u64 x, u64 y)
  {
    return cells[(x * GRID_WIDTH) + y];
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
        std::min((WIDTH / 1.5) / GRID_WIDTH, (HEIGHT / 1.5) / GRID_HEIGHT);
    u64 padding_x = (WIDTH - (cell_dimension * GRID_WIDTH)) / 2;
    u64 padding_y = (HEIGHT - (cell_dimension * GRID_HEIGHT)) / 2;

    return {cell_dimension, padding_x, padding_y};
  }

  void draw(void)
  {
    auto [cell_dimension, padding_x, padding_y] = get_cell_dimensions();

    for (u64 x = 0; x < GRID_WIDTH; ++x)
    {
      for (u64 y = 0; y < GRID_HEIGHT; ++y)
      {
        u64 cell_x      = (x * cell_dimension) + padding_x;
        u64 cell_y      = (y * cell_dimension) + padding_y;
        u32 cell_colour = cells[(x * GRID_WIDTH) + y];
        Color col       = {};
        memcpy(&col, &cell_colour, sizeof(Color));

        DrawRectangle(cell_x, cell_y, cell_dimension, cell_dimension, col);
        // DrawRectangleLines(cell_x, cell_y, cell_dimension, cell_dimension,
        //                    WHITE);
      }
    }

    for (const auto &actor : actors)
    {
      u64 x = (actor.x * cell_dimension) + padding_x;
      u64 y = (actor.y * cell_dimension) + padding_y;
      DrawRectangleLines(x, y, cell_dimension, cell_dimension, BLUE);
    }
  }

  void add_actor(u32 id, u64 x, u64 y)
  {
    actors.push_back({id, x, y});
    cells[(x * GRID_WIDTH) + y] = id;
  }

  u64 get_neighbours(u64 x, u64 y, std::array<tuple<u64, u64, u32>, 4> &arr)
  {
    assert(x < GRID_WIDTH && y < GRID_HEIGHT);
    u64 size = 0;
    if (x > 0)
      arr[size++] = {x - 1, y, cells[((x - 1) * GRID_WIDTH) + y]};
    if (x < GRID_WIDTH - 1)
      arr[size++] = {x + 1, y, cells[((x + 1) * GRID_WIDTH) + y]};
    if (y > 0)
      arr[size++] = {x, y - 1, cells[(x * GRID_WIDTH) + (y - 1)]};
    if (y < GRID_HEIGHT - 1)
      arr[size++] = {x, y + 1, cells[(x * GRID_WIDTH) + (y + 1)]};
    return size;
  }

  // random strategy: pick a random cell
  void actor_random_strat(Actor &actor)
  {
    std::array<tuple<u64, u64, u32>, 4> neighbours;
    u64 size = get_neighbours(actor.x, actor.y, neighbours);
#if !ACTORS_BACKWARD_MOVEMENT
    for (u64 i = 0; i < size; ++i)
    {
      if (std::get<2>(neighbours[i]) == actor.id)
      {
        --size;
        auto x           = neighbours[size];
        neighbours[size] = neighbours[i];
        neighbours[i]    = x;
      }
    }
#endif
#if !ACTORS_REPLACE_CELLS
    for (u64 i = 0; i < size; ++i)
    {
      if (!(std::get<2>(neighbours[i]) == actor.id ||
            std::get<2>(neighbours[i]) == 0))
      {
        --size;
        auto x           = neighbours[size];
        neighbours[size] = neighbours[i];
        neighbours[i]    = x;
      }
    }
#endif

    if (size == 0)
      return;
    u64 ind                     = rand() % size;
    auto [x, y, cell]           = neighbours[ind];
    cells[(x * GRID_WIDTH) + y] = actor.id;
    actor.x                     = x;
    actor.y                     = y;
  }

  void update(void)
  {
    std::for_each(std::begin(actors), std::end(actors),
                  [this](auto &actor)
                  {
                    actor_random_strat(actor);
                  });
  }
};

int main(void)
{
  srand(time(NULL));
  Grid grid{};

  for (u64 i = 0; i < NUM_ACTORS; ++i)
  {
    u64 x   = rand() % GRID_WIDTH;
    u64 y   = rand() % GRID_HEIGHT;
    Color c = {
        .r = (u8)(rand() % 200 + 55),
        .g = (u8)(rand() % 200 + 55),
        .b = (u8)(rand() % 200 + 55),
        .a = 255,
    };

    u32 packed = 0;
    memcpy(&packed, &c, sizeof(c));

    grid.add_actor(packed, x, y);
  }

  f32 timer = 0;
  InitWindow(WIDTH, HEIGHT, "Random Paths");
  SetTargetFPS(60);
  while (!WindowShouldClose())
  {
    timer += 1 / GetFrameTime();
    if (timer > DELTA_TIME)
    {
      grid.update();
      timer = 0;
    }
    BeginDrawing();
    ClearBackground(BLACK);
    grid.draw();
    EndDrawing();
  }
  CloseWindow();

  return 0;
}

/* Copyright (C) 2026 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the Unlicense for details.

 * You may distribute and modify this code under the terms of the Unlicense,
 * which you should have received a copy of along with this program.  If not,
 * please go to <https://unlicense.org/>.

 */
