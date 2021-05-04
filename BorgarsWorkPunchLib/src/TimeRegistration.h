#pragma once

#include <QDateTime>


class TimeRegistration
{
   private:
      int mProjectID;
      QString mProjectName;
      QDateTime mPunchInTimeUTC;
      QDateTime mPunchOutTimeUTC;

   public:
      TimeRegistration(int projectID, const QString &projectName, const QDateTime &punchInUTC, const QDateTime &punchOutUTC);

      int getProjectID() const;
      QString getProjectName() const;
      QDateTime getPunchInTimeUTC() const;
      QDateTime getPunchOutTimeUTC() const;
};
