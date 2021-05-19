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
WeekData::WeekData(int weekNumber, const QDateTime &startDate, const QDateTime &endDate)
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
QDateTime WeekData::getCurrentWeekStartDate()
{
   QDateTime dateTime = QDateTime::currentDateTime();
   dateTime.setTime(QTime(0, 0, 0));

   while(dateTime.date().dayOfWeek() != Qt::Monday)
   {
      dateTime = dateTime.addDays(-1);
   }

   return dateTime;
}

/**
 * @return the end date of the current week (Sunday)
 */
QDateTime WeekData::getCurrentWeekEndDate()
{
   QDateTime dateTime = QDateTime::currentDateTime();
   dateTime.setTime(QTime(23, 59, 59));

   while(dateTime.date().dayOfWeek() != Qt::Sunday)
   {
      dateTime = dateTime.addDays(1);
   }

   return dateTime;
}
