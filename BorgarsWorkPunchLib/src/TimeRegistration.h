#pragma once

#include <QDateTime>


class TimeRegistration
{
   private:
      QString mProjectID;
      QDateTime mPunchInTimeUTC;
      QDateTime mPunchOutTimeUTC;

   public:
      TimeRegistration(const QString &projectID, const QDateTime &punchInUTC, const QDateTime &punchOutUTC);

      QDateTime getPunchInTimeUTC() const;
      QDateTime getPunchOutTimeUTC() const;
      QString getProjectID() const;
};
