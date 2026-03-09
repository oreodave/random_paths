/* base.hpp: Basic definitions
 * Created: 2026-02-13
 * Author: Aryadev Chavali
 * License: See end of file
 * Commentary:
 */

#ifndef BASE_HPP
#define BASE_HPP

#include "./prick_aliases.h"
#include <optional>
#include <tuple>

template <typename T> using opt = std::optional<T>;
using std::pair;
using std::tuple;

constexpr u64 WIDTH      = 1024;
constexpr u64 HEIGHT     = 1024;
constexpr u64 GRID_SIZE  = 64;
constexpr u64 NUM_ACTORS = 64;
constexpr f32 DELTA_TIME = 1;
constexpr u64 DELTA_CULL = 5;

// should actors be allowed to go back on themselves?
#define ACTORS_BACKWARD_MOVEMENT 1
// should actors be able to replace other cells?
#define ACTORS_REPLACE_CELLS 0

#endif

/* Copyright (C) 2026 Aryadev Chavali

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the Unlicense for details.

 * You may distribute and modify this code under the terms of the Unlicense,
 * which you should have received a copy of along with this program.  If not,
 * please go to <https://unlicense.org/>.

 */
