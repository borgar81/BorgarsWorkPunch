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

QString WeekReport::getDayReport(int day, const QMap<int, QString> &projectIDMap) const
{
   return mDayReportList.at(day).getDayReport(projectIDMap);
}

QDateTime WeekReport::getFromDateTimeUTC() const
{
   return mFromDateTimeUTC;
}

QDateTime WeekReport::getToDateTimeUTC() const
{
   return mToDateTimeUTC;
}
