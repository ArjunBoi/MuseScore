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

#ifndef MU_UI_MACOSPLATFORMTHEME_H
#define MU_UI_MACOSPLATFORMTHEME_H

#include "internal/iplatformtheme.h"

namespace mu::ui {
class MacOSPlatformTheme : public IPlatformTheme
{
public:
    void startListening() override;
    void stopListening() override;

    bool isFollowSystemThemeAvailable() const override;

    ThemeCode themeCode() const override;
    async::Channel<ThemeCode> themeCodeChanged() const override;

    void applyPlatformStyleOnAppForTheme(ThemeCode themeCode) override;
    void applyPlatformStyleOnWindowForTheme(QWidget* window, ThemeCode themeCode) override;

private:
    bool isSystemDarkMode() const;
    bool isSystemHighContrast() const;

    async::Channel<ThemeCode> m_channel;
};
}

#endif // MU_UI_MACOSPLATFORMTHEME_H
