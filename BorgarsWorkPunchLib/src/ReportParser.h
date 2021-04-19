#pragma once

#include <QDateTime>
#include <QObject>
#include <QJsonDocument>

#include "WeekReport.h"


class ReportParser : public QObject
{
   Q_OBJECT

   signals:

   private:
      QJsonDocument mJsonDocument;

      QString mErrorText;

      bool isTimeRegInsideFromAndTo(const QDateTime &punchInUTC, const QDateTime &punchOutUTC, const QDateTime &fromDateTimeUTC, const QDateTime &toDateTimeUTC);

   public:
      explicit ReportParser(QObject *parent = nullptr);

      bool initialize(const QByteArray &byteArray);

      QString getErrorText() const;

      WeekReport createWeekReport(const QDateTime &fromDateTimeUTC, const QDateTime &toDateTimeUTC);


};

