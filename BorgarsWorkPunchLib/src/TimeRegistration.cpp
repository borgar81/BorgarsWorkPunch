// Qt Includes

// Local Includes
#include "TimeRegistration.h"

TimeRegistration::TimeRegistration(const QDateTime &punchInUTC, const QDateTime &punchOutUTC)
{
   mPunchInTimeUTC = punchInUTC;
   mPunchOutTimeUTC = punchOutUTC;
}
