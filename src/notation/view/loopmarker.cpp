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

#include "loopmarker.h"

using namespace mu::notation;

LoopMarker::LoopMarker(LoopBoundaryType type)
    : m_type(type)
{
}

void LoopMarker::setRect(const QRect& rect)
{
    m_rect = rect;
}

void LoopMarker::setVisible(bool visible)
{
    m_visible = visible;
}

void LoopMarker::setStyle(INotationStylePtr style)
{
    m_style = style;
}

void LoopMarker::paint(mu::draw::Painter* painter)
{
    if (!m_visible || !m_style) {
        return;
    }

    QPolygonF triangle(3);

    qreal x = m_rect.left();
    qreal y = m_rect.top();
    qreal h = m_style->styleValue(StyleId::spatium).toDouble() * 2;

    QColor color = configuration()->loopMarkerColor();

    switch (m_type) {
    case LoopBoundaryType::LoopIn: { // draw a right-pointing triangle
        qreal tx = x - 1.0;
        triangle[0] = QPointF(tx, y);
        triangle[1] = QPointF(tx, y + h);
        triangle[2] = QPointF(tx + h, y + h / 2);
    }
    break;
    case LoopBoundaryType::LoopOut: { // draw a left-pointing triangle
        triangle[0] = QPointF(x, y);
        triangle[1] = QPointF(x, y + h);
        triangle[2] = QPointF(x - h, y + h / 2);
    }
    break;
    case LoopBoundaryType::Unknown: return;
    }

    painter->setPen(QPen(color, 2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    painter->drawLine(x, y, x, m_rect.bottom());
    painter->setBrush(color);
    painter->drawConvexPolygon(triangle);
}
