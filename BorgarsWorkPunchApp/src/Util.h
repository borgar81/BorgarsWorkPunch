#pragma once

#include <QString>

#include "Constants.h"


class Util
{
   private:
      Util() {};     // private constructor

   public:

      static QString getDayOfWeekAsText(WeekDays::WeekDaysEnum weekDay);
};

