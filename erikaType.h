#ifndef WATCHY_7_SEG_H
#define WATCHY_7_SEG_H

#include <Watchy.h>
#include "Erika_Type_B10pt7b.h"
#include "Erika_Type_B12pt7b.h"
#include "Erika_Type_B20pt7b.h"
#include "Erika_Type_B30pt7b.h"


#include "icons.h"

class erikaType : public Watchy{
    using Watchy::Watchy;
    public:
        void drawWatchFace();
        void drawTime();
        void drawDate();
        void drawSteps();
        void drawWeather();
        void drawBattery();
};

#endif
