// Local Includes
#include "Formatter.h"
#include "WeekData.h"


/**
 * Constructor
 *
 * @param weekNumber the week number
 * @param startDate the start date of the week
 * @param endDate the end date of the week
 */
WeekData::WeekData(int weekNumber, const QDate &startDate, const QDate &endDate)
{
   mWeekNumber = weekNumber;
   mStartDate = startDate;
   mEndDate = endDate;
}

/**
 * @return formatted week string
 */
QString WeekData::toString() const
{
   QString weekNumberStr = Formatter::intToString(mWeekNumber, 2, '0');

   QString str = QStringLiteral("%1: %2 - %3")
            .arg(weekNumberStr)
            .arg(mStartDate.toString(QStringLiteral("dd.MM.yy")))
            .arg(mEndDate.toString(QStringLiteral("dd.MM.yy")));

         return str;
}

/**
 * @return the start date of the current week (Monday)
 */
QDate WeekData::getCurrentWeekStartDate()
{
   QDate date = QDate::currentDate();

   while(date.dayOfWeek() != Qt::Monday)
   {
      date = date.addDays(-1);
   }

   return date;
}

/**
 * @return the end date of the current week (Sunday)
 */
QDate WeekData::getCurrentWeekEndDate()
{
   QDate date = QDate::currentDate();

   while(date.dayOfWeek() != Qt::Sunday)
   {
      date = date.addDays(1);
   }

   return date;
}
