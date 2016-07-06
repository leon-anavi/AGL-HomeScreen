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

#ifndef DAYNIGHTMODE_H
#define DAYNIGHTMODE_H

namespace SystemDayNight {
    typedef enum DayNightMode
    {
        DAYNIGHTMODE_UNDEFINED = -1,
        DAYNIGHTMODE_DAY = 0,
        DAYNIGHTMODE_NIGHT = 1
    } eDayNightMode;
}

#endif // DAYNIGHTMODE_H
