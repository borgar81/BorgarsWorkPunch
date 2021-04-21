// Qt Includes
#include <QUrl>
#include <QDesktopServices>

// Local Includes
#include "CppInterface.h"

CppInterface::CppInterface(QObject *parent)
   : QObject(parent)
{

}

bool CppInterface::sendEmailReport()
{
   bool ok = false;

   // TODO Verify Dates!

   //ReportGenerator reportGenerator(0);
   //reportGenerator.initialize(mSQLInterface);
   //QString reportStr = reportGenerator.generateReport(fromDate, toDate);

   //QString address = readSetting("emailAddress", QString()).toString();
   QString address = "borgar.ovsthus@technipfmc.com";
   QString header = "BorgarsWorkPunch report from bla bla to BLA BLA";
   QString text;
   text += "Copy This into SAP\n\n";
   text += "==================================\n";
   text += "reportStr";
   text += "\n";
   text += "==================================\n";

   //QString str = QString("mailto:%1&subject=%2&body=%3").arg(address).arg(header).arg(text);
   //QString str = QString("mailto:%1?Subject=%2?Body=%3").arg(address).arg(header).arg(text);

   //qDebug() << str;

   QString str;
   str += "mailto:?to=";
   str += address;
   str += "&subject=";
   str += header;
   str += "&body=";
   str += text;


   QUrl url = QUrl(str, QUrl::TolerantMode);

   //return QDesktopServices::openUrl(QUrl("mailto:?to=recipient@example.com&subject=The subject of an email&body=Here is some email body text", QUrl::TolerantMode));
   ok = QDesktopServices::openUrl(url);

   return ok;
}
