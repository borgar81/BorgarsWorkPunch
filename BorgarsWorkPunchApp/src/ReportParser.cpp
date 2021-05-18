// Qt Includes
#include <QDateTime>
#include <QDebug>
#include <QJsonParseError>
#include <QJsonObject>
#include <QMap>

// Local Includes
#include "Util.h"
#include "WeekReport.h"
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


WeekReport ReportParser::createWeekReport(const QDateTime &fromDateTimeUTC, const QDateTime &toDateTimeUTC)
{
   WeekReport weekReport(fromDateTimeUTC, toDateTimeUTC);

   /*QJsonObject rootObject = mJsonDocument.object();
   for (auto it = rootObject.begin(); it != rootObject.end(); it++)
   {
      QJsonObject timeRegObject = it.value().toObject();

      QString projectID = timeRegObject.value("ProjectID").toString();
      QDateTime punchInTimeUTC = QDateTime::fromString(timeRegObject.value("PunchInTime").toString(), Qt::ISODate);
      QDateTime punchOutTimeUTC = QDateTime::fromString(timeRegObject.value("PunchOutTime").toString(), Qt::ISODate);

      if (!isTimeRegInsideFromAndTo(punchInTimeUTC, punchOutTimeUTC, fromDateTimeUTC, toDateTimeUTC))
      {
         continue;
      }

      //QString weekDayName = Util::getDayOfWeekAsText((WeekDays::WeekDaysEnum) punchInTimeUTC.date().dayOfWeek());

      DayReport &dayReport = weekReport.mDayReportList[punchInTimeUTC.date().dayOfWeek()-1];

      dayReport.mTimeRegistrationList << TimeRegistration(projectID, punchInTimeUTC, punchOutTimeUTC);
   }*/

   return weekReport;

   //qDebug() << weekReport.mDayReportList.size();
}
