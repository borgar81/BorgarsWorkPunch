#include "WeekReport.h"

WeekReport::WeekReport()
{

}

WeekReport::WeekReport(const QDateTime &fromDateTimeUTC, const QDateTime &toDateTimeUTC)
{
   mFromDateTimeUTC = fromDateTimeUTC;
   mToDateTimeUTC = toDateTimeUTC;

   mDayReportList << DayReport(WeekDays::Monday);
   mDayReportList << DayReport(WeekDays::Tuesday);
   mDayReportList << DayReport(WeekDays::Wednesday);
   mDayReportList << DayReport(WeekDays::Thursday);
   mDayReportList << DayReport(WeekDays::Friday);
   mDayReportList << DayReport(WeekDays::Saturday);
   mDayReportList << DayReport(WeekDays::Sunday);
}

QList<DayReport> WeekReport::getDayReportList() const
{
   return mDayReportList;
}

quint64 WeekReport::getTotalWorkTime(int day) const
{
   return mDayReportList.at(day).getTotalWorkTime();
}

quint64 WeekReport::getWeekTotalHours() const
{
   quint64 weekTotalTimeInSeconds = 0;

   for (const DayReport &dayReport : mDayReportList)
   {
      weekTotalTimeInSeconds += getTotalWorkTime(dayReport.getWeekDay()-1);
   }

   return weekTotalTimeInSeconds;
}

QString WeekReport::getDayReportTotals(int day, const QMap<int, QString> &projectIDMap) const
{
   return mDayReportList.at(day).getDayReportTotals(projectIDMap);
}

QString WeekReport::getDayReportPunchIns(int day) const
{
   return mDayReportList.at(day).getDayReportPunchIns();
}

QDateTime WeekReport::getFromDateTimeUTC() const
{
   return mFromDateTimeUTC;
}

QDateTime WeekReport::getToDateTimeUTC() const
{
   return mToDateTimeUTC;
}
