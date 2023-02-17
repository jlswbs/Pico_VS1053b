/*
 * Klang – a node+text-based synthesizer library
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2022 Dennis P Paul.
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LUTSine_h
#define LUTSine_h

#include "KlangMath.hpp"

namespace klang {

    class LUTSine {
    public:
        static const uint16_t        WAVETABLE_LENGTH = 2048;
        static const SIGNAL_TYPE     WAVETABLE[WAVETABLE_LENGTH + 1];
        constexpr static const float ALPHA = (WAVETABLE_LENGTH / TWO_PI);

        static SIGNAL_TYPE sin_fast(const float pRad) {
            const uint16_t mIndex = ((uint32_t)(WAVETABLE_LENGTH * pRad / TWO_PI)) % WAVETABLE_LENGTH;
            return WAVETABLE[mIndex];
        }

        static SIGNAL_TYPE cos_fast(const float pRad) {
            return sin_fast(pRad + HALF_PI);
        }
    };
}  // namespace klang

#endif /* LUTSine_h */
