// Qt Includes
#include <QDateTime>
#include <QDebug>
#include <QJsonParseError>
#include <QJsonObject>
#include <QMap>

// Local Includes
#include "Util.h"
#include "ReportParser.h"

/**
 * Constructor
 *
 * @param parent parent object
 */
ReportParser::ReportParser(QObject *parent)
   : QObject(parent)
{

}

/**
 * Initializes this parser with the given JSON data
 *
 * @param byteArray byte array containing the JSON data
 *
 * @return true on success, false on failure
 */
bool ReportParser::initialize(const QByteArray &byteArray)
{
   QJsonParseError jsonError;

   mJsonDocument = QJsonDocument::fromJson(byteArray, &jsonError);
   if (jsonError.error != QJsonParseError::NoError)
   {
      mErrorText = QStringLiteral("ReportParser::initialize. %1").arg(jsonError.errorString());
      return false;
   }

   return true;
}

/**
 * @return return the last error text
 */
QString ReportParser::getErrorText() const
{
   return mErrorText;
}

bool ReportParser::isTimeRegInsideFromAndTo(const QDateTime &punchInUTC, const QDateTime &punchOutUTC, const QDateTime &fromDateTimeUTC, const QDateTime &toDateTimeUTC)
{
   if ((punchInUTC >= fromDateTimeUTC) && (punchInUTC <= toDateTimeUTC) &&
       (punchOutUTC >= fromDateTimeUTC) && (punchOutUTC <= toDateTimeUTC))
   {
      return  true;
   }
   else
   {
      return false;
   }
}


void ReportParser::createWeekReport(const QDateTime &fromDateTimeUTC, const QDateTime &toDateTimeUTC)
{
   //QMap<QString, DayReport> reportMap; // Key is Week day name

   QList<DayReport> weekReportList;
   weekReportList << DayReport(tr("Monday"));
   weekReportList << DayReport(tr("Tuesday"));
   weekReportList << DayReport(tr("Wednesday"));
   weekReportList << DayReport(tr("Thursday"));
   weekReportList << DayReport(tr("Friday"));
   weekReportList << DayReport(tr("Saturday"));
   weekReportList << DayReport(tr("Sunday"));

   QJsonObject rootObject = mJsonDocument.object();
   for (auto it = rootObject.begin(); it != rootObject.end(); it++)
   {
      QJsonObject timeRegObject = it.value().toObject();

      QDateTime punchInTimeUTC = QDateTime::fromString(timeRegObject.value("PunchInTime").toString(), Qt::ISODate);
      QDateTime punchOutTimeUTC = QDateTime::fromString(timeRegObject.value("PunchOutTime").toString(), Qt::ISODate);

      if (!isTimeRegInsideFromAndTo(punchInTimeUTC, punchOutTimeUTC, fromDateTimeUTC, toDateTimeUTC))
      {
         continue;
      }

      QString weekDayName = Util::getDayOfWeekAsText(punchInTimeUTC.date().dayOfWeek());

      DayReport &dayReport = weekReportList[punchInTimeUTC.date().dayOfWeek()-1];

      dayReport.mTimeRegistrationList << TimeRegistration(punchInTimeUTC, punchOutTimeUTC);

      //qDebug() << weekDayName;
   }

   qDebug() << weekReportList.size();
}
