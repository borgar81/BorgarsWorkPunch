#pragma once

#include <QList>
#include <QDateTime>

#include "DayReport.h"

class WeekReport
{
   Q_GADGET

   //Q_PROPERTY(QList<DayReport> dayReportList READ getDayReportList)

   private:

      QList<DayReport> mDayReportList;

      QDateTime mFromDateTimeUTC;
      QDateTime mToDateTimeUTC;

   public:
      WeekReport();
      WeekReport(const QDateTime &fromDateTimeUTC, const QDateTime &toDateTimeUTC);

      DayReport getDayReport(int weekDay) const;
      DayReport &getDayReportRef(int weekDay);

      //QList<DayReport> &getDayReportList();
      QDateTime getFromDateTimeUTC() const;
      QDateTime getToDateTimeUTC() const;
      int getDayCount() const;

      quint64 getTotalWorkTime(int day) const;
      quint64 getWeekTotalHours() const;
      QString getDayReportTotals(int day, const QMap<int, QString> &projectIDMap) const;
      QString getDayReportPunchIns(int day) const;

      QString generateSapReportString(QMap<int, QVariantMap> projectMap) const;

      //--------------------------------------
      // Operator Overloading
      //--------------------------------------
      bool operator==(const WeekReport &other) const;
      bool operator!=(const WeekReport &other) const;
};

Q_DECLARE_METATYPE(WeekReport)
