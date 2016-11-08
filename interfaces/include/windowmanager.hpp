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

#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QtDBus>

#define WINDOWMANAGER_NO_ERROR 0
#define WINDOWMANAGER_ERROR_ID_ALREADY_DEFINED 1
#define WINDOWMANAGER_ERROR_NAME_ALREADY_DEFINED 2
#define WINDOWMANAGER_ERROR_ID_NOT_FOUND 3
#define WINDOWMANAGER_ERROR_NAME_NOT_FOUND 4
#define WINDOWMANAGER_ERROR_ID_IN_USE 5

class SimplePoint
{
public:
    SimplePoint();
    virtual ~SimplePoint();

	int x;
	int y;

    friend QDBusArgument &operator <<(QDBusArgument &argument, const SimplePoint &mSimplePoint);
    friend const QDBusArgument &operator >>(const QDBusArgument &argument, SimplePoint &mSimplePoint);
};


class LayoutArea
{
public:
    LayoutArea();
    virtual ~LayoutArea();

	int x;
	int y;
	int width;
	int height;

    friend QDBusArgument &operator <<(QDBusArgument &argument, const LayoutArea &mLayoutArea);
    friend const QDBusArgument &operator >>(const QDBusArgument &argument, LayoutArea &mLayoutArea);
};

class Layout
{
public:
    Layout();
    Layout(int layoutId, const QString &layoutName, const QList<LayoutArea> &surfaceAreas);
    virtual ~Layout();

    int id;
    QString name;
    QList<LayoutArea> layoutAreas;

    friend QDBusArgument &operator <<(QDBusArgument &argument, const Layout &mLayout);
    friend const QDBusArgument &operator >>(const QDBusArgument &argument, Layout &mLayout);
};

Q_DECLARE_METATYPE(SimplePoint)
Q_DECLARE_METATYPE(QList<SimplePoint>)

Q_DECLARE_METATYPE(LayoutArea)
Q_DECLARE_METATYPE(QList<LayoutArea>)

Q_DECLARE_METATYPE(Layout)
Q_DECLARE_METATYPE(QList<Layout>)

#endif // WINDOWMANAGER_H
