#pragma once

#include "Constants.h"
#include "Util.h"
#include "TimeRegistration.h"


class DayReport
{
   private:
      WeekDays::WeekDaysEnum mWeekDay {WeekDays::Monday};
      QString mDayName;

   public:
      DayReport();
      DayReport(WeekDays::WeekDaysEnum weekDay);
      QList<TimeRegistration> mTimeRegistrationList;
      QString getDayName() const;

      quint64 getTotalWorkTime() const;
      QString getDayReportTotals(const QMap<int, QString> &projectIDMap) const;
      QString getDayReportPunchIns() const;
};

Q_DECLARE_METATYPE(DayReport)
