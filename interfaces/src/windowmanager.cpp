/*
 * Copyright (C) 2016 Mentor Graphics Development (Deutschland) GmbH
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "include/windowmanager.hpp"

SimplePoint::SimplePoint()
{
}

SimplePoint::~SimplePoint()
{
}


LayoutArea::LayoutArea()
{
}

LayoutArea::~LayoutArea()
{
}


Layout::Layout():
    id(-1),
    name("N/A"),
    layoutAreas()
{
}

Layout::Layout(int layoutId, const QString &layoutName, const QList<LayoutArea> &surfaceAreas):
    id(layoutId),
    name(layoutName),
    layoutAreas(surfaceAreas)
{
}

Layout::~Layout()
{
}

QDBusArgument &operator <<(QDBusArgument &argument, const SimplePoint &mSimplePoint)
{
	argument.beginStructure();
	argument << mSimplePoint.x;
	argument << mSimplePoint.y;
	argument.endStructure();

	return argument;
}

const QDBusArgument &operator >>(const QDBusArgument &argument, SimplePoint &mSimplePoint)
{
	argument.beginStructure();
    argument >> mSimplePoint.x;
    argument >> mSimplePoint.y;
	argument.endStructure();
	return argument;
}

QDBusArgument &operator <<(QDBusArgument &argument, const LayoutArea &mLayoutArea)
{
	argument.beginStructure();
    argument << mLayoutArea.x;
    argument << mLayoutArea.y;
    argument << mLayoutArea.width;
    argument << mLayoutArea.height;
	argument.endStructure();

	return argument;
}

const QDBusArgument &operator >>(const QDBusArgument &argument, LayoutArea &mLayoutArea)
{
	argument.beginStructure();
    argument >> mLayoutArea.x;
    argument >> mLayoutArea.y;
    argument >> mLayoutArea.width;
    argument >> mLayoutArea.height;
	argument.endStructure();
	return argument;
}

QDBusArgument &operator <<(QDBusArgument &argument, const Layout &mLayout)
{
    argument.beginStructure();
    argument << mLayout.id;
    argument << mLayout.name;
    argument << mLayout.layoutAreas;
    argument.endStructure();

    return argument;
}

const QDBusArgument &operator >>(const QDBusArgument &argument, Layout &mLayout)
{
    argument.beginStructure();
    argument >> mLayout.id;
    argument >> mLayout.name;
    argument >> mLayout.layoutAreas;
    argument.endStructure();
    return argument;
}
