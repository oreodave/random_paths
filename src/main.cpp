/* main.cpp: Entrypoint
 * Created: 2026-02-12
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary:
 */

#include "base.hpp"
#include "grid.hpp"
#include "strats.hpp"

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <raylib.h>

// random strategy: pick a random cell

int main(void)
{
  srand(time(NULL));
  Grid grid{actor_random_strat};

  for (u64 i = 0; i < NUM_ACTORS; ++i)
  {
    while (true)
    {

      u64 x   = rand() % GRID_SIZE;
      u64 y   = rand() % GRID_SIZE;
      Color c = {
          .r = (u8)(rand() % 200 + 55),
          .g = (u8)(rand() % 200 + 55),
          .b = (u8)(rand() % 200 + 55),
          .a = 255,
      };

      colour_t packed = 0;
      memcpy(&packed, &c, sizeof(c));

      // NOTE: No target
      if (grid.add_actor(packed, {x, y}, {0, 0}))
      {
        break;
      }
    }
  }

  bool paused = true;
  f32 timer   = 0;
  (void)timer;
  InitWindow(WIDTH, HEIGHT, "Random Paths");
  SetTargetFPS(60);
  while (!WindowShouldClose())
  {
    if (IsKeyPressed(KEY_SPACE))
    {
      paused = !paused;
    }
    if (IsKeyPressed(KEY_PERIOD))
    {
      paused = true;
      grid.update();
    }
    timer += 1 / GetFrameTime();
    if (timer > DELTA_TIME && !paused)
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
