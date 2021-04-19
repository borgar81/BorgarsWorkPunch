#pragma once

#include <QDateTime>


class TimeRegistration
{
   private:
      QDateTime mPunchInTimeUTC;
      QDateTime mPunchOutTimeUTC;


   public:
      TimeRegistration(const QDateTime &punchInUTC, const QDateTime &punchOutUTC);

};
