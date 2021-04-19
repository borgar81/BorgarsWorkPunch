#include "DayReport.h"

QString DayReport::getDayName() const
{
   return mDayName;
}

DayReport::DayReport()
{
   mDayName = Util::getDayOfWeekAsText(mWeekDay);
}

DayReport::DayReport(WeekDays::WeekDaysEnum weekDay)
{
   mWeekDay = weekDay;
   mDayName = Util::getDayOfWeekAsText(weekDay);
}
