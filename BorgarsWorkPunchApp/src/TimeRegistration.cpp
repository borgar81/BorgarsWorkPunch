// Qt Includes

// Local Includes
#include "TimeRegistration.h"

TimeRegistration::TimeRegistration(int projectID, const QString &projectName, const QDateTime &punchInUTC, const QDateTime &punchOutUTC)
{
   mProjectID = projectID;
   mProjectName = projectName;
   mPunchInTimeUTC = punchInUTC;
   mPunchOutTimeUTC = punchOutUTC;
}

int TimeRegistration::getProjectID() const
{
   return mProjectID;
}

QString TimeRegistration::getProjectName() const
{
   return mProjectName;
}

QDateTime TimeRegistration::getPunchInTimeUTC() const
{
   return mPunchInTimeUTC;
}

QDateTime TimeRegistration::getPunchOutTimeUTC() const
{
   return mPunchOutTimeUTC;
}

/**
* Returns true if the TimeRegistration object is equal to the other TimeRegistration object
*
* @param other the other TimeRegistration object to compare with
*
* @return true if the two TimeRegistration objects are equal. Otherwise return false
*/
bool TimeRegistration::operator==(const TimeRegistration &other) const
{
   if (this == &other)
      return true;

   if (this->mProjectID != other.mProjectID)
      return false;

   if (this->mProjectName != other.mProjectName)
      return false;

   if (this->mPunchInTimeUTC != other.mPunchInTimeUTC)
      return false;

   if (this->mPunchOutTimeUTC != other.mPunchOutTimeUTC)
      return false;

   return true;
}

/**
* Returns true if the TimeRegistration object is NOT equal to the other TimeRegistration object
*
* @param other the other TimeRegistration object to compare with
*
* @return true if the two TimeRegistration objects are NOT equal. Otherwise return false
*/
bool TimeRegistration::operator!=(const TimeRegistration &other) const
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

