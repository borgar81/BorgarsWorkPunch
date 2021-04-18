#pragma once

#include <QString>

class Util
{
   private:
      Util() {};     // private constructor

   public:

      static QString getDayOfWeekAsText(int weekDayNumber);
};

