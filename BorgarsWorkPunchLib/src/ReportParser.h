#pragma once

#include <QDateTime>
#include <QObject>
#include <QJsonDocument>

class TimeRegistration
{
   private:
      QDateTime mPunchInTimeUTC;
      QDateTime mPunchOutTimeUTC;


   public:
      TimeRegistration(const QDateTime &punchInUTC, const QDateTime &punchOutUTC)
      {
         mPunchInTimeUTC = punchInUTC;
         mPunchOutTimeUTC = punchOutUTC;
      }

};

class DayReport
{
   private:
      QString mDayName;

   public:
      DayReport(const QString &dayName)
      {
         mDayName = dayName;
      }
      QList<TimeRegistration> mTimeRegistrationList;

};

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

      void createWeekReport(const QDateTime &fromDateTimeUTC, const QDateTime &toDateTimeUTC);


};

