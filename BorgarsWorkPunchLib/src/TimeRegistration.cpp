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

