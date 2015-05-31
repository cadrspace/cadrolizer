/* common.hpp -- Common procedures and macros.
 *
 * Copyright (C) 2015 Artyom V. Poptsov <poptsov.artyom@gmail.com>
 *
 * This file is part of Cadrolizer.
 *
 * Cadrolizer is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Cadrolizer is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cadrolizer.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __COMMON_HPP__
#define __COMMON_HPP__

bool debug = false;

#define DEBUG(...)                                              \
        do {                                                    \
                if (debug)                                      \
                        printf("[DEBUG] " __VA_ARGS__);         \
        } while (0)

#define ERROR(...)                                      \
        do {                                            \
                printf("[ERROR] " __VA_ARGS__);         \
        } while (0)

#endif  /* ifndef __COMMON_HPP__ */

/* common.hpp ends here. */
