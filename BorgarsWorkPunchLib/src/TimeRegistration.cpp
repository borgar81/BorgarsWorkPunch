// Qt Includes

// Local Includes
#include "TimeRegistration.h"

QString TimeRegistration::getProjectID() const
{
   return mProjectID;
}

TimeRegistration::TimeRegistration(const QString &projectID, const QDateTime &punchInUTC, const QDateTime &punchOutUTC)
{
   mProjectID = projectID;
   mPunchInTimeUTC = punchInUTC;
   mPunchOutTimeUTC = punchOutUTC;
}

QDateTime TimeRegistration::getPunchInTimeUTC() const
{
   return mPunchInTimeUTC;
}

QDateTime TimeRegistration::getPunchOutTimeUTC() const
{
   return mPunchOutTimeUTC;
}
