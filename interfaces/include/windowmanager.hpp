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


class SimpleRect
{
public:
    SimpleRect();
    virtual ~SimpleRect();

	int x;
	int y;
	int width;
	int height;

    friend QDBusArgument &operator <<(QDBusArgument &argument, const SimpleRect &mSimpleRect);
    friend const QDBusArgument &operator >>(const QDBusArgument &argument, SimpleRect &mSimpleRect);
};


Q_DECLARE_METATYPE(SimplePoint)
Q_DECLARE_METATYPE(QList<SimplePoint>)

Q_DECLARE_METATYPE(SimpleRect)
Q_DECLARE_METATYPE(QList<SimpleRect>)


#endif // WINDOWMANAGER_H
