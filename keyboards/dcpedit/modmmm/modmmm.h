/* Copyright 2021 dcpedit
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include "quantum.h"

#define ___ KC_NO

/* This a shortcut to help you visually see your layout.
 *
 * The first section contains all of the arguments representing the physical
 * layout of the board and position of the keys.
 *
 * The second converts the arguments into a two-dimensional array which
 * represents the switch matrix.
 */

#define LAYOUT(                                                                                                         \
    k00,     k60, k01, k61, k02,    k62, k03, k63, k04,    k64, k05, k65, k06,    k66, k07, k67,                        \
		                                                                                                                    \
    k10, k70, k11, k71, k12, k72, k13, k73, k14, k74, k15, k75, k16, k76, k17,    k27, k77, k87,    k08, k68, k09, k69, \
      k20, k80, k21, k81, k22, k82, k23, k83, k24, k84, k25, k85, k26, k86,       k37, k47, k97,    k18, k78, k19, k79, \
       k30, k90, k31, k91, k32, k92, k33, k93, k34, k94, k35, k95, k36,                             k28, k88, k29, k89, \
    k40, kA0, k41, kA1, k42, kA2, k43, kA3, k44, kA4, k45, kA5, k46, k96,              k48,         k38, k98, k39, k99, \
    k50,            k51,     kB2,     kB3,     kB4,     kB5,              kA6,    k58, kB8, kA8,    k59, k49, kB9, kA9  \
) {                                                             \
    { k00,  k01,  k02,  k03,  k04,  k05,  k06, k07, k08, k09 }, \
    { k10,  k11,  k12,  k13,  k14,  k15,  k16, k17, k18, k19 }, \
    { k20,  k21,  k22,  k23,  k24,  k25,  k26, k27, k28, k29 }, \
    { k30,  k31,  k32,  k33,  k34,  k35,  k36, k37, k38, k39 }, \
    { k40,  k41,  k42,  k43,  k44,  k45,  k46, k47, k48, k49 }, \
    { k50,  k51,  ___,  ___,  ___,  ___,  ___, ___, k58, k59 }, \
    { k60,  k61,  k62,  k63,  k64,  k65,  k66, k67, k68, k69 }, \
    { k70,  k71,  k72,  k73,  k74,  k75,  k76, k77, k78, k79 }, \
    { k80,  k81,  k82,  k83,  k84,  k85,  k86, k87, k88, k89 }, \
    { k90,  k91,  k92,  k93,  k94,  k95,  k96, k97, k98, k99 }, \
    { kA0,  kA1,  kA2,  kA3,  kA4,  kA5,  kA6, ___, kA8, kA9 }, \
    { ___,  ___,  kB2,  kB3,  kB4,  kB5,  ___, ___, kB8, kB9 }  \
}

