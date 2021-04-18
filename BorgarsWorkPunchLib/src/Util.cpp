#include <QObject>

#include "Util.h"


/**
 * Returns the name of the day for the given week day number
 *
 * @param weekDayNumber the week day number
 *
 * @return week day name
 */
QString Util::getDayOfWeekAsText(int weekDayNumber)
{
   QString weekDayName;

   switch(weekDayNumber)
   {
      case 1:
         weekDayName = QObject::tr("Monday");
         break;

      case 2:
         weekDayName = QObject::tr("Tuesday");
         break;

      case 3:
         weekDayName = QObject::tr("Wednesday");
         break;

      case 4:
         weekDayName = QObject::tr("Thursday");
         break;

      case 5:
         weekDayName = QObject::tr("Friday");
         break;

      case 6:
         weekDayName = QObject::tr("Saturday");
         break;

      case 7:
         weekDayName = QObject::tr("Sunday");
         break;

      default:
         weekDayName = QObject::tr("Unknown day");
         break;

   }

   return weekDayName;
}
