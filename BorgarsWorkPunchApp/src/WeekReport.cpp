#include <QDebug>


// Local Includes
#include "Constants.h"
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

DayReport WeekReport::getDayReport(int weekDay) const
{
   return mDayReportList.at(weekDay);
}

DayReport &WeekReport::getDayReportRef(int weekDay)
{
   return mDayReportList[weekDay];
}

/*QList<DayReport> &WeekReport::getDayReportList()
{
   return mDayReportList;
}*/

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

/**
 * Generate a string that can be pasted directly into SAP based on this week report
 *
 * @param projectMap Map containing ProjectIDs and Projects. Key is ProjectID
 *
 * @return SAP report String
 */
QString WeekReport::generateSapReportString(QMap<int, QVariantMap> projectMap) const
{
   const int SAP_COLUMN_COUNT = 30;

   QString sapReportString;

   // row, column
   QList<QStringList> sapTable;

   //-------------------------------------------
   // Find unique projects for this week
   //-------------------------------------------
   QList<int> uniqueProjectIDs;
   for (const DayReport &dayReport : mDayReportList)
   {
      for (const TimeRegistration &timeRegData : dayReport.mTimeRegistrationList)
      {
         if (!uniqueProjectIDs.contains(timeRegData.getProjectID()))
         {
            uniqueProjectIDs << timeRegData.getProjectID();
         }
      }
   }

   // Find Max number of entries for one day
   int maxTimeRegistrationPerDay = 0;
   for (const DayReport &dayReport : mDayReportList)
   {
      maxTimeRegistrationPerDay = std::max(maxTimeRegistrationPerDay, dayReport.mTimeRegistrationList.size());
   }

   const int SAP_TABLE_ROW_COUNT = maxTimeRegistrationPerDay + uniqueProjectIDs.size();

   //-------------------------------------------
   // Initialize the table with tabs
   //-------------------------------------------
   for(int rowIndex=0; rowIndex<SAP_TABLE_ROW_COUNT; rowIndex++)
   {
      QStringList columnList;
      for(int colIndex=0; colIndex<SAP_COLUMN_COUNT; colIndex++)
      {
         columnList << "\t";
      }

      sapTable << columnList; // Append Column List
   }

   //-------------------------------------------
   // Insert Time-registration data
   //-------------------------------------------
   int rowIndex = 0;
   int columnIndex = SAPColumns::Monday_From;
   for (const DayReport &dayReport : mDayReportList)
   {
      // PunchIns/PunchOuts
      for (const TimeRegistration &timeRegData : dayReport.mTimeRegistrationList)
      {
         QVariantMap projectData = projectMap.value(timeRegData.getProjectID());


         sapTable[rowIndex][SAPColumns::AA_Type]   = (projectData["AA_Type"].toString() + "\t");
         sapTable[rowIndex][columnIndex]           = (timeRegData.getPunchInTimeUTC().toLocalTime().toString("hh:mm") + "\t");      // From
         sapTable[rowIndex][columnIndex+1]         = (timeRegData.getPunchOutTimeUTC().toLocalTime().toString("hh:mm") + "\t");     // To
         rowIndex++;
      }
      rowIndex = 0;
      columnIndex += 3;
   }


   //-------------------------------------------
   // Write Project Registrations
   //-------------------------------------------
   rowIndex = maxTimeRegistrationPerDay;
   for(int projectID : uniqueProjectIDs)
   {
      QVariantMap projectData = projectMap.value(projectID);
      ProjectTypes::ProjectTypesEnum projectType = static_cast<ProjectTypes::ProjectTypesEnum> (projectData["Type"].toInt());
      if (projectType == ProjectTypes::ProjectTypesEnum::Network)
      {
         sapTable[rowIndex][SAPColumns::Network] = (projectData["NetworkOrOrder"].toString() + "\t");
         sapTable[rowIndex][SAPColumns::Activity] = (projectData["Activity"].toString() + "\t");
      }
      else if (projectType == ProjectTypes::ProjectTypesEnum::Order)
      {
         sapTable[rowIndex][SAPColumns::Rec_Order] = (projectData["NetworkOrOrder"].toString() + "\t");
         int activity = projectData["Activity"].toInt();
         if (activity != -1)
         {
            sapTable[rowIndex][SAPColumns::Activity] = (QString::number(activity) + "\t");
         }
      }

      rowIndex++;
   }

   //-------------------------------------------
   // Write Project-registrations
   //-------------------------------------------
   rowIndex = maxTimeRegistrationPerDay;
   columnIndex = SAPColumns::Monday;
   for (const DayReport &dayReport : mDayReportList)
   {
      for(int projectID : uniqueProjectIDs)
      {
         quint64 workTimeForProjectInSeconds = dayReport.getTotalWorkTimeForProject(projectID);
         if (workTimeForProjectInSeconds > 0)
         {
            double workProjectHours = workTimeForProjectInSeconds / 3600.0;
            QString hourWorkedString = QString::number(workProjectHours, 'f', 2);
            hourWorkedString.replace('.', ',');    // TODO Should this be configurable?

            sapTable[rowIndex][columnIndex] = (hourWorkedString + "\t");
         }
         rowIndex++;
      }
      rowIndex = maxTimeRegistrationPerDay;
      columnIndex += 3;
   }

   //-------------------------------------------
   // Write the table to a String
   //-------------------------------------------
   for(int tmpRowIndex=0; tmpRowIndex<SAP_TABLE_ROW_COUNT; tmpRowIndex++)
   {
      for(int tmpColumnIndex=0; tmpColumnIndex<SAP_COLUMN_COUNT; tmpColumnIndex++)
      {
         sapReportString += sapTable[tmpRowIndex][tmpColumnIndex];
      }
      sapReportString += "\n";
   }

   return sapReportString;
}

/**
* Returns true if the WeekReport object is equal to the other WeekReport object
*
* @param other the other WeekReport object to compare with
*
* @return true if the two WeekReport objects are equal. Otherwise return false
*/
bool WeekReport::operator==(const WeekReport &other) const
{
   if (this == &other)
      return true;

   if (this->mFromDateTimeUTC != other.mFromDateTimeUTC)
      return false;

   if (this->mToDateTimeUTC != other.mToDateTimeUTC)
      return false;

   if (this->mDayReportList != other.mDayReportList)
      return false;

   return true;
}

/**
* Returns true if the WeekReport object is NOT equal to the other WeekReport object
*
* @param other the other WeekReport object to compare with
*
* @return true if the two WeekReport objects are NOT equal. Otherwise return false
*/
bool WeekReport::operator!=(const WeekReport &other)  const
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

QDateTime WeekReport::getFromDateTimeUTC() const
{
   return mFromDateTimeUTC;
}

QDateTime WeekReport::getToDateTimeUTC() const
{
   return mToDateTimeUTC;
}

int WeekReport::getDayCount() const
{
   return mDayReportList.size();
}
