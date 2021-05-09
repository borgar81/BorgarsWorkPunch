// Qt Includes
#include <QUrl>
#include <QDesktopServices>

// Local Includes
#include "WeekData.h"
#include "SQLInterface.h"
#include "email.h"
#include "CppInterface.h"

CppInterface::CppInterface(SQLInterface *sqlinterface, QObject *parent)
   : QObject(parent)
{
   mSQLInterface = sqlinterface;
}


bool CppInterface::sendEmailReport()
{
   /*
   bool ok = false;

   // TODO Verify Dates!

   //ReportGenerator reportGenerator(0);
   //reportGenerator.initialize(mSQLInterface);
   //QString reportStr = reportGenerator.generateReport(fromDate, toDate);

   //QString address = readSetting("emailAddress", QString()).toString();
   QString address = "borgar.ovsthus@technipfmc.com";
   QString header = "BorgarsWorkPunch report from bla bla to BLA BLA";
   QString text;

   text += "<html>";
   text += "<body>";
   text += "Copy This into SAP\n\n";
   text += "==================================\n";
   text += "'1	2	3	4	5	6	7				11	12		14	15		16	17		18	19		20	21		22	23		50	51'\n";
   text += "1%20%20%202\n";
   //text += "&quot;1  2  3\t4\t5\t6\t7				11	12		14	15		16	17		18	19		20	21		22	23		50	51&quot;\n";
   //text += "\"1&nbsp;&nbsp;&nbsp;2&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;3\t4\t5\t6\t7				11	12		14	15		16	17		18	19		20	21		22	23		50	51\"\n";

   text += "==================================\n";
   text += "</body>";
   text += "</html>";

   //QString str = QString("mailto:%1&subject=%2&body=%3").arg(address).arg(header).arg(text);
   //QString str = QString("mailto:%1?Subject=%2?Body=%3").arg(address).arg(header).arg(text);


   QString str;
   //str += "<";
   str += "mailto:?to=";
   str += address;
   str += "&subject=";
   str += header;
   str += "&body=";
   str += text;
   //str += ">";

   // TODO Write your own email client. have a look here:
            // https://github.com/xcoder123/SimpleSmtp_SSL_QT5
            // https://morf.lv/simple-tls-ssl-smtp-client-for-qt5
            // https://morf.lv/adding-file-attachments-to-smtp-client-for-qt5


   QUrl url = QUrl(str, QUrl::TolerantMode);

   //return QDesktopServices::openUrl(QUrl("<mailto:?to=recipient@example.com&subject=The subject of an email&body=Here is some           email body text>", QUrl::TolerantMode));
   ok = QDesktopServices::openUrl(url);


   return ok;*/

   Email email(nullptr, "");
   email.setSenderAddress("borgar.ovsthus@technipfmc.com");
   email.setReceiverAddress("borgar.ovsthus@technipfmc.com");
   email.setSubject("Week report");
   email.setMessageText("This is      a   test.\tTag");
   email.openInDefaultProgram();

   return true;
}

/**
 * @return Returns the week number of the todays date
 */
int CppInterface::getCurrentWeekNumber() const
{
   QDate date = QDate::currentDate();
   return date.weekNumber();
}

/**
 * @return the start datetime of the current week (in local time)
 */
QDateTime CppInterface::getStartOfCurrentWeekDate() const
{
   QDateTime startOfWeekDateTime(WeekData::getCurrentWeekStartDate(), QTime(0, 0, 0));
   return startOfWeekDateTime;
}

/**
 * @return the end datetime of the current week (in local time)
 */
QDateTime CppInterface::getEndOfCurrentWeekDate() const
{
   QDateTime endOfWeekDateTime(WeekData::getCurrentWeekEndDate(), QTime(23, 59, 59));
   return endOfWeekDateTime;
}

/**
 * SLOT. Called when the application state changes. Opens/Closes SQL connection
 *
 * @param state the new application state
 */
void CppInterface::onApplicationStateChanged(Qt::ApplicationState state)
{
   if (state == Qt::ApplicationSuspended)
   {
      mSQLInterface->closeDatabase();
   }
   else if (state == Qt::ApplicationActive)
   {
      mSQLInterface->openDatabase();
      mSQLInterface->updateTotalTimeWorkedToday();
   }
}
