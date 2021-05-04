// Qt Includes
#include <QDateTime>
#include <QSet>
#include <QDebug>

// Local Includes
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


/**
 * @return total worked hours this day in seconds
 */
quint64 DayReport::getTotalWorkTime() const
{
   quint64 seconds = 0;

   for (const TimeRegistration &timeReg : mTimeRegistrationList)
   {
      seconds += timeReg.getPunchOutTimeUTC().toSecsSinceEpoch() - timeReg.getPunchInTimeUTC().toSecsSinceEpoch();
   }

   return seconds;
}

QString DayReport::getDayReport(const QMap<int, QString> &projectIDMap) const
{
   QString reportString;

   // Total Work
   QTime workTotalTime(0, 0);
   workTotalTime = workTotalTime.addSecs(getTotalWorkTime());
   reportString += QStringLiteral("   Total hours: %1\n").arg(workTotalTime.toString("h:mm"));

   QMap<int, quint64> projectWorkMap;

   // Projects
   for (const TimeRegistration &timeReg : mTimeRegistrationList)
   {
      quint64 &projectWorkedSeconds = projectWorkMap[timeReg.getProjectID()];
      projectWorkedSeconds += timeReg.getPunchOutTimeUTC().toSecsSinceEpoch() - timeReg.getPunchInTimeUTC().toSecsSinceEpoch();
   }

   for (auto it = projectWorkMap.constBegin(); it != projectWorkMap.constEnd(); it++)
   {
      QString projectName = projectIDMap.value(it.key());
      QTime workProjectTime(0, 0);
      workProjectTime = workProjectTime.addSecs(it.value());
      reportString += QStringLiteral("   %1: %2").arg(projectName, workProjectTime.toString("h:mm"));
      if (it.key() != projectWorkMap.lastKey())
      {
         reportString += "\n";
      }
   }

   return reportString;
}
