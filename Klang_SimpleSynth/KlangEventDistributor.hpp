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

#ifndef KlangEventDistributor_hpp
#define KlangEventDistributor_hpp

#include <stdint.h>

#include <vector>

#include "Klang.hpp"
#include "KlangEventListener.hpp"

namespace klang {
    class KlangEventDistributor {
    public:
        void add_event_listener(KlangEventListener* pEventListener) {
            mEventListeners.push_back(pEventListener);
        }

        void clear_listeners() {
            mEventListeners.clear();
        }

        void fire_event(KLANG_EVENT pEventType, std::vector<float> pData) {
            for (uint8_t i = 0; i < mEventListeners.size(); i++) {
                if (mEventListeners[i] != nullptr) {
                    mEventListeners[i]->event(pEventType, pData);
                }
            }
        }

    private:
        std::vector<KlangEventListener*> mEventListeners;
    };
}  // namespace klang

#endif /* KlangEventDistributor_hpp */
