#pragma once

#include <QList>
#include <QDateTime>

#include "DayReport.h"

class WeekReport
{
   Q_GADGET

   Q_PROPERTY(QList<DayReport> dayReportList READ getDayReportList)

   private:
      QDateTime mFromDateTimeUTC;
      QDateTime mToDateTimeUTC;

   public:
      WeekReport();
      WeekReport(const QDateTime &fromDateTimeUTC, const QDateTime &toDateTimeUTC);

      QList<DayReport> mDayReportList;

      QList<DayReport> getDayReportList() const;
      QDateTime getFromDateTimeUTC() const;
      QDateTime getToDateTimeUTC() const;

      quint64 getTotalWorkTime(int day) const;
      QString getDayReportTotals(int day, const QMap<int, QString> &projectIDMap) const;
      QString getDayReportPunchIns(int day) const;

};

Q_DECLARE_METATYPE(WeekReport)
