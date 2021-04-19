#include <QObject>

#include "Util.h"

/**
 * Returns the name of the day for the given week day number
 *
 * @param weekDayNumber the week day number
 *
 * @return week day name
 */
QString Util::getDayOfWeekAsText(WeekDays::WeekDaysEnum weekDay)
{
   QString weekDayName;

   switch(weekDay)
   {
      case WeekDays::Monday:
         weekDayName = QObject::tr("Monday");
         break;

      case WeekDays::Tuesday:
         weekDayName = QObject::tr("Tuesday");
         break;

      case WeekDays::Wednesday:
         weekDayName = QObject::tr("Wednesday");
         break;

      case WeekDays::Thursday:
         weekDayName = QObject::tr("Thursday");
         break;

      case WeekDays::Friday:
         weekDayName = QObject::tr("Friday");
         break;

      case WeekDays::Saturday:
         weekDayName = QObject::tr("Saturday");
         break;

      case WeekDays::Sunday:
         weekDayName = QObject::tr("Sunday");
         break;

      default:
         weekDayName = QObject::tr("Unknown day");
         break;

   }

   return weekDayName;
}
