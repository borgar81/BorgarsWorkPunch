// Qt Includes
#include <QUrl>
#include <QDesktopServices>
#include <QDebug>

// C++ Includes
#include <iostream>

// Local Includes
#include "WeekData.h"
#include "SQLInterface.h"
#include "CppInterface.h"
#ifdef Q_OS_IOS
   #include "IOSInterface.h"
#endif


void CppInterface::onMailCancelled()
{
   emit debugMessage("Mail Cancelled");
}

void CppInterface::onMailSaved()
{
   emit debugMessage("Mail Saved");
}

void CppInterface::onMailSent()
{
   emit debugMessage("Mail Sent");
}

void CppInterface::onMailFailed()
{
   emit debugMessage("Mail Failed");
}

CppInterface::CppInterface(SQLInterface *sqlinterface, QObject *parent)
   : QObject(parent)
{
   mSQLInterface = sqlinterface;


   // TODO Make general interface for both IOS and Android
#ifdef Q_OS_IOS
   mIOSInterface = new IOSInterface(this);
   connect(mIOSInterface, &IOSInterface::mailCancelled, this, &CppInterface::onMailCancelled);
   connect(mIOSInterface, &IOSInterface::mailSaved, this, &CppInterface::onMailSaved);
   connect(mIOSInterface, &IOSInterface::mailSent, this, &CppInterface::onMailSent);
   connect(mIOSInterface, &IOSInterface::mailFailed, this, &CppInterface::onMailFailed);
#endif
}


bool CppInterface::sendEmailReport()
{
   WeekReport weekReport = mSQLInterface->getCurrentWeekReport();

   QList<QString> toList;
   toList << "borgar.ovsthus@technipfmc.com";         // TODO Read from Settings

   QString body = "This email has been sent from BorarsWorkPunch.\n\nOpen the attatchment with the desired format, copy the whole content, and paste it into SAP or ESS";

   QString subject = QStringLiteral("Work report (%1 -> %2)")
         .arg(weekReport.getFromDateTimeUTC().toLocalTime().toString("dd.MM.yyyy"))
         .arg(weekReport.getToDateTimeUTC().toLocalTime().toString("dd.MM.yyyy"));

   QString sapString = weekReport.generateSapReportString(mSQLInterface->getProjectMap());
   //std::cout << "===================================" << std::endl;
   //std::cout << sapString.toStdString();
   //std::cout << "===================================" << std::endl;

#ifdef Q_OS_IOS
      mIOSInterface->openNewMailMessage(subject, toList, body, sapString.toUtf8(), "SAP_Format.txt");
#else
   emit debugMessage("Send-email is not supported on this OS");
#endif
   //emit debugMessage(macAddress);

   /*
   Links:
      https://github.com/ndesai/qt-mobile-modules
      https://forum.qt.io/topic/88297/native-objective-c-calls-from-cpp-qt-ios-email-call
   */

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

   // https://docs.fileformat.com/email/emlx/
   // https://docs.fileformat.com/email/msg/
   // https://docs.fileformat.com/email/eml/
   // https://blog.aspose.com/2020/08/07/create-outlook-email-msg-eml-emlx-using-cpp/

   /*Email email(nullptr, "");

   connect(&email, &Email::debugMessage, this, &CppInterface::debugMessage);

   email.setSenderAddress("borgar.ovsthus@technipfmc.com");
   email.setReceiverAddress("borgar.ovsthus@technipfmc.com");
   email.setSubject("Week report");
   email.setMessageText("This is      a   test.Tag");
   email.openInDefaultProgram();
   */


   //QDesktopServices::openUrl(QUrl("ms-outlook://compose?to=borgar.ovsthus@technipfmc.com&subject=Weekreport&body=Message", QUrl::TolerantMode));
   //qDebug() << result;
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
   return WeekData::getCurrentWeekStartDate();
   //QDateTime startOfWeekDateTime(WeekData::getCurrentWeekStartDate(), QTime(0, 0, 0));
   //return startOfWeekDateTime;
}

/**
 * @return the end datetime of the current week (in local time)
 */
QDateTime CppInterface::getEndOfCurrentWeekDate() const
{
   return WeekData::getCurrentWeekEndDate();
   //QDateTime endOfWeekDateTime(WeekData::getCurrentWeekEndDate(), QTime(23, 59, 59));
   //return endOfWeekDateTime;
}

/**
 * Adds the number of days to the given datetime and returns the results
 *
 * @param dateTime the original datetime
 * @param days the number of days to add (can be negative)
 *
 * @return the new date time
 */
QDateTime CppInterface::addDaysToDateTime(const QDateTime &dateTime, int days) const
{
   return dateTime.addDays(days);
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
