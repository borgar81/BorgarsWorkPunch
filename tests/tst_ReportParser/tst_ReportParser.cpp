#include <QtTest>
#include <QJsonObject>
#include <QJsonValue>
#include <QDateTime>

// add necessary includes here
#include "BorgarsWorkPunchLib/src/Constants.h"
#include "BorgarsWorkPunchLib/src/ReportParser.h"

class tst_ReportParser : public QObject
{
   Q_OBJECT

   private:
      ReportParser *mFixture;

      QJsonObject util_createTimeRegistrationObject(const QString &projectID, const QDateTime &punchInTimeUTC, const QDateTime &punchOutTimeUTC);

   private slots:

      void init();
      void cleanup();

      void initialize_OK();
      void initialize_ERROR();

};

/**
 * Init. Create fixture object
 */
void tst_ReportParser::init()
{
   mFixture = new ReportParser(this);
}

/**
 * Cleanup. Delete fixture object
 */
void tst_ReportParser::cleanup()
{
   delete mFixture;
   mFixture = nullptr;
}

QJsonObject tst_ReportParser::util_createTimeRegistrationObject(const QString &projectID, const QDateTime &punchInTimeUTC, const QDateTime &punchOutTimeUTC)
{
   QJsonObject obj;
   obj.insert("ProjectID", projectID);
   obj.insert("PunchInTime", punchInTimeUTC.toString(Qt::ISODate));
   obj.insert("PunchOutTime", punchOutTimeUTC.toString(Qt::ISODate));
   return obj;
}


/**
 * Initialize - OK
 */
void tst_ReportParser::initialize_OK()
{
   QByteArray ba;

   // Arrange
   QJsonObject jsonObject;
   jsonObject.insert("ID_1", util_createTimeRegistrationObject("PROJ_ID", QDateTime(QDate(2021,  4, 12), QTime( 8,  30, 0), Qt::UTC), QDateTime(QDate(2021,  4, 12), QTime( 9,  0, 0), Qt::UTC)));

   QJsonDocument jsonDocument(jsonObject);
   ba = jsonDocument.toJson();

   // Act
   bool result = mFixture->initialize(ba);

   // Assert
   QCOMPARE(result, true);
}

/**
 * Initialize - ERROR
 */
void tst_ReportParser::initialize_ERROR()
{
   // Arrange
   QByteArray ba;

   // Act
   bool result = mFixture->initialize(ba);

   // Assert
   QCOMPARE(result, false);
}


QTEST_APPLESS_MAIN(tst_ReportParser)

#include "tst_ReportParser.moc"
