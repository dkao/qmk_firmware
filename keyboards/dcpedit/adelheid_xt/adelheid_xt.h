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

/* This a shortcut to help you visually see your layout.
 *
 * The first section contains all of the arguments representing the physical
 * layout of the board and position of the keys.
 *
 * The second converts the arguments into a two-dimensional array which
 * represents the switch matrix.
 */
#define LAYOUT( \
	k00, k06,      k10, k16, k20, k26, k30, k36, k40,      k46, k50, k56, k60, k66, k70,      k76,      k86, k90, k96, kA0, kA6, kB0, kB6, \
	k01, k07,      k11, k17, k21, k27, k31, k37, k41,      k47, k51, k57, k61, k67, k71, k77, k81,      k87, k91, k97, kA1, kA7, kB1, kB7, \
	k02, k08,      k12, k18, k22, k28, k32, k38,           k48, k52, k58, k62, k68, k72, k78, k82,      k88, k92, k98, kA2, kA8, kB2, kB8, \
	k03, k09,      k13, k19, k23, k29, k33, k39,           k49, k53, k59, k63, k69, k73,      k79,                     kA3, kA9, kB3, kB9, \
	k04, k0A,      k14, k1A, k24, k2A, k34, k3A,      k44, k4A, k54, k5A, k64,      k74,      k84,           k94,      kA4, kAA, kB4, kBA, \
	k05, k0B,      k15,           k1B, k2B, k3B,           k4B, k5B,                          k85,      k8B, k95, k9B, kA5, kAB, kB5, kBB  \
) \
{ \
	{ k00,   k10,   k20,   k30,   k40,   k50,   k60,   k70,   KC_NO, k90,   kA0,   kB0 }, \
	{ k01,   k11,   k21,   k31,   k41,   k51,   k61,   k71,   k81,   k91,   kA1,   kB1 }, \
	{ k02,   k12,   k22,   k32,   KC_NO, k52,   k62,   k72,   k82,   k92,   kA2,   kB2 }, \
	{ k03,   k13,   k23,   k33,   KC_NO, k53,   k63,   k73,   KC_NO, KC_NO, kA3,   kB3 }, \
	{ k04,   k14,   k24,   k34,   k44,   k54,   k64,   k74,   k84,   k94,   kA4,   kB4 }, \
	{ k05,   k15,   KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, k85,   k95,   kA5,   kB5 }, \
	{ k06,   k16,   k26,   k36,   k46,   k56,   k66,   k76,   k86,   k96,   kA6,   kB6 }, \
	{ k07,   k17,   k27,   k37,   k47,   k57,   k67,   k77,   k87,   k97,   kA7,   kB7 }, \
	{ k08,   k18,   k28,   k38,   k48,   k58,   k68,   k78,   k88,   k98,   kA8,   kB8 }, \
	{ k09,   k19,   k29,   k39,   k49,   k59,   k69,   k79,   KC_NO, KC_NO, kA9,   kB9 }, \
	{ k0A,   k1A,   k2A,   k3A,   k4A,   k5A,   KC_NO, KC_NO, KC_NO, KC_NO, kAA,   kBA }, \
	{ k0B,   k1B,   k2B,   k3B,   k4B,   k5B,   KC_NO, KC_NO, k8B,   k9B,   kAB,   kBB }  \
}
