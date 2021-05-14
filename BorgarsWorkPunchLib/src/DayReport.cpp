// Qt Includes
#include <QDateTime>
#include <QSet>
#include <QDebug>

// Local Includes
#include "Formatter.h"
#include "DayReport.h"

QString DayReport::getDayName() const
{
   return mDayName;
}

WeekDays::WeekDaysEnum DayReport::getWeekDay() const
{
   return mWeekDay;
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
 * @return total worked time this day in seconds
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

/**
 * @return total worked time this day for the given project in seconds
 */
quint64 DayReport::getTotalWorkTimeForProject(int projectID) const
{
   quint64 seconds = 0;

   for (const TimeRegistration &timeReg : mTimeRegistrationList)
   {
      if (timeReg.getProjectID() == projectID)
      {
         seconds += timeReg.getPunchOutTimeUTC().toSecsSinceEpoch() - timeReg.getPunchInTimeUTC().toSecsSinceEpoch();
      }
   }

   return seconds;
}

/**
 * Creates a Day report string showing the the total for the day and the total for each project
 *
 * @param projectIDMap crossref map for ProjectID and project name
 *
 * @return day totals report as string
 */
QString DayReport::getDayReportTotals(const QMap<int, QString> &projectIDMap) const
{
   QString reportString;

   // Total Work
   double totalWorkHours = getTotalWorkTime() / 3600.0;
   reportString += QStringLiteral("   Total hours: %1\n").arg(QString::number(totalWorkHours, 'f', 2));

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
      double workProjectHours = it.value() / 3600.0;
      reportString += QStringLiteral("   %1: %2").arg(projectName, QString::number(workProjectHours, 'f', 2));
      if (it.key() != projectWorkMap.lastKey())
      {
         reportString += "\n";
      }
   }

   return reportString;
}

/**
 * Creates a Day report string showing all the PunchIn/PunchOuts for the day
 *
 * @param projectIDMap crossref map for ProjectID and project name
 *
 * @return day totals report as string
 */
QString DayReport::getDayReportPunchIns() const
{
   QString reportString;

   // Total Work
   double totalWorkHours = getTotalWorkTime() / 3600.0;
   reportString += QStringLiteral("   Total hours: %1\n").arg(QString::number(totalWorkHours, 'f', 2));

   // Projects
   for (const TimeRegistration &timeReg : mTimeRegistrationList)
   {
      reportString += QString("   %1: %2 - %3").arg(timeReg.getProjectName(), timeReg.getPunchInTimeUTC().toLocalTime().toString("hh:mm"), timeReg.getPunchOutTimeUTC().toLocalTime().toString("hh:mm"));

      if (&timeReg != &mTimeRegistrationList.last())
      {
         reportString += "\n";
      }
   }

   return reportString;
}

/**
* Returns true if the DayReport object is equal to the other DayReport object
*
* @param other the other DayReport object to compare with
*
* @return true if the two DayReport objects are equal. Otherwise return false
*/
bool DayReport::operator==(const DayReport &other) const
{
   if (this == &other)
      return true;

   if (this->mWeekDay != other.mWeekDay)
      return false;

   if (this->mDayName != other.mDayName)
      return false;

   if (this->mTimeRegistrationList != other.mTimeRegistrationList)
      return false;

   return true;
}

/**
* Returns true if the DayReport object is NOT equal to the other DayReport object
*
* @param other the other DayReport object to compare with
*
* @return true if the two DayReport objects are NOT equal. Otherwise return false
*/
bool DayReport::operator!=(const DayReport &other) const
{
   if(*this == other)
   {
      return false;
   }
   else
   {
      return true;
   }
}
