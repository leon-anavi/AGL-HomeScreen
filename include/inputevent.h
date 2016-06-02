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

#ifndef INPUTEVENT_H
#define INPUTEVENT_H

namespace SystemInputEvent
{
    typedef enum InputEventType
    {
        INPUTEVENTTYPE_UNDEFINED,
        INPUTEVENTTYPE_KEY_PRESSED
    } eInputEventType;

    typedef enum InputEventZone
    {
        INPUTEVENTZONE_UNDEFINED,
        INPUTEVENTZONE_1,
        INPUTEVENTZONE_2
    } eInputEventZone;
}

#endif // INPUTEVENT_H