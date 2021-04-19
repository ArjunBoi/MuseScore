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
#ifndef MU_UI_KEYNAVIGATIONPOPUPPANEL_H
#define MU_UI_KEYNAVIGATIONPOPUPPANEL_H

#include <QObject>
#include "keynavigationsubsection.h"
#include "keynavigationcontrol.h"

namespace mu::ui {
class KeyNavigationPopupPanel : public KeyNavigationSubSection
{
    Q_OBJECT

    Q_PROPERTY(KeyNavigationControl * parentControl READ parentControl WRITE setParentControl NOTIFY parentControlChanged)

public:
    explicit KeyNavigationPopupPanel(QObject* parent = nullptr);

    KeyNavigationControl* parentControl() const;

public slots:
    void setParentControl(KeyNavigationControl* parentControl);

signals:
    void parentControlChanged(KeyNavigationControl* parentControl);

private:
    KeyNavigationControl* m_parentControl = nullptr;
};
}
#endif // MU_UI_KEYNAVIGATIONPOPUPPANEL_H
