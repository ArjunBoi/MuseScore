/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 *
 * MuseScore
 * Music Composition & Notation
 *
 * Copyright (C) 2021 MuseScore BVBA and others
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "zerberus.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "channel.h"
#include "voice.h"
#include "instrument.h"
#include "zone.h"
#include "controllers.h"

using namespace mu::zerberus;

// static const float PI_2 =  1.57079632679489661923;    /* pi/2 */

//---------------------------------------------------------
//   Channel
//---------------------------------------------------------

Channel::Channel(Zerberus* ms, int i)
{
    _msynth     = ms;
    _idx        = i;
    _instrument = 0;
    _gain       = 1.0;
    _midiVolume = 1.0;
    _panLeftGain  = cosf(static_cast<float>(M_PI_2 * 64.0 / 126.0));
    _panRightGain = sinf(static_cast<float>(M_PI_2 * 64.0 / 126.0));
    memset(ctrl, 0, 128 * sizeof(char));
    ctrl[CTRL_EXPRESSION] = 127;
}

//---------------------------------------------------------
//   pitchBend
//---------------------------------------------------------

void Channel::pitchBend(int)
{
}

//---------------------------------------------------------
//   controller
//    realtime
//---------------------------------------------------------

void Channel::controller(int c, int val)
{
    ctrl[c] = val;
    if (c == CTRL_SUSTAIN) {
        if (val < 0x40) {
            for (Voice* v = _msynth->getActiveVoices(); v; v = v->next()) {
                if (v->isSustained()) {
//printf("sustain off %p\n", v);
                    v->stop();
                }
            }
        }
    } else if (c == CTRL_PANPOT) {
        val -= 1;
        if (val < 0) {
            val = 0;
        }
        _panLeftGain  = cosf(M_PI_2 * float(val) / 126.0);
        _panRightGain = sinf(M_PI_2 * float(val) / 126.0);
    } else if (c == CTRL_VOLUME) {
        // This was previously:
        // (float(val) * float(ctrl[Ms::CTRL_EXPRESSION])) / (127.0 * 127.0);
        // which was a hack to include CC11 support for SFZ. If necessary, revert to this.
        _midiVolume = float(val) / 127.0;
    } else if (c == CTRL_ALL_NOTES_OFF) {
        for (Voice* v = _msynth->getActiveVoices(); v; v = v->next()) {
            if (!v->isOff()) {
                v->stop();
            }
        }
    } else if (c == CTRL_PROGRAM) {
        printf("Zerberus: program %d\n", val);
        ZInstrument* zi = _msynth->instrument(val);
        if (zi == 0) {
            printf("   not found\n");
        }
        if (zi && zi != _instrument) {
            for (Voice* v = _msynth->getActiveVoices(); v; v = v->next()) {
                v->off();
            }
            _instrument = zi;
            resetCC();
        }
    }

    for (Zone* z : instrument()->zones()) {
        z->updateCCGain(this);
    }
//      else
//            qDebug("Zerberus: ctrl 0x%02x 0x%02x", ctrl, val);
}

//---------------------------------------------------------
//   sustain
//---------------------------------------------------------

int Channel::sustain() const
{
    return ctrl[CTRL_SUSTAIN];
}

int Channel::getCtrl(int CTRL) const
{
    return ctrl[CTRL];
}

void Channel::resetCC()
{
    if (!_instrument) {
        return;
    }
    for (int i = 0; i < 128; i++) {
        if (_instrument->getSetCC(i) != -1) {
            ctrl[i] = _instrument->getSetCC(i);
        }
    }
}
