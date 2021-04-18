// Qt Includes

// Library Includes
#include "BorgarsWorkPunchLib/src/FirebaseAuthHandler.h"
#include "BorgarsWorkPunchLib/src/FirebaseInterface.h"


// Local Includes
#include "TestApplication.h"

const QString API_KEY = "AIzaSyCaK51HBaXFPKlMOd3-SxgQVAeFt9ygFXE";

const QString GJOA = "-MY2Df5cS-P27saYOSDv";
const QString MORRAMOTE = "-MYE-FKiXYbGc1orFCbp";

TestApplication::TestApplication(QObject *parent)
   : QObject(parent)
{
   mAutHandler = new FirebaseAuthHandler(this);
   mAutHandler->setAPIKey(API_KEY);

   mFirebaseInterface = new FirebaseInterface(this);

   connect(mAutHandler, &FirebaseAuthHandler::userSignedIn,    this, &TestApplication::onUserLoggedIn);

   mAutHandler->signUserIn("Borgar.Ovsthus@technipfmc.com", "MyPassword");
}

void TestApplication::createTestData()
{
   // Monday
   mFirebaseInterface->registerProjectWork(MORRAMOTE, QDateTime(QDate(2021,  4, 12), QTime( 8,  30, 0), Qt::UTC),    QDateTime(QDate(2021,  4, 12), QTime( 9,  0, 0), Qt::UTC));
   mFirebaseInterface->registerProjectWork(GJOA,      QDateTime(QDate(2021,  4, 12), QTime( 9,  0, 0), Qt::UTC),     QDateTime(QDate(2021,  4, 12), QTime(16,  0, 0), Qt::UTC));

   // Tuesday
   mFirebaseInterface->registerProjectWork(GJOA,      QDateTime(QDate(2021,  4, 13), QTime( 8,  0, 0), Qt::UTC),     QDateTime(QDate(2021,  4, 13), QTime(16,  0, 0), Qt::UTC));

   // Wednesday
   mFirebaseInterface->registerProjectWork(MORRAMOTE, QDateTime(QDate(2021,  4, 14), QTime( 8,  30, 0), Qt::UTC),    QDateTime(QDate(2021,  4, 14), QTime( 9,  0, 0), Qt::UTC));
   mFirebaseInterface->registerProjectWork(GJOA,      QDateTime(QDate(2021,  4, 14), QTime( 9,  0, 0), Qt::UTC),     QDateTime(QDate(2021,  4, 14), QTime(16,  0, 0), Qt::UTC));

   // Thursday
   mFirebaseInterface->registerProjectWork(GJOA,      QDateTime(QDate(2021,  4, 15), QTime(12,  0, 0), Qt::UTC),     QDateTime(QDate(2021,  4, 15), QTime(16,  0, 0), Qt::UTC));

   // Friday
   mFirebaseInterface->registerProjectWork(GJOA,      QDateTime(QDate(2021,  4, 16), QTime(12,  0, 0), Qt::UTC),     QDateTime(QDate(2021,  4, 16), QTime(16,  0, 0), Qt::UTC));
}

void TestApplication::onUserLoggedIn(const QString &idToken, const QString &localID)
{
   mFirebaseInterface->setIDToken(idToken);
   mFirebaseInterface->setLocalID(localID);

   //createTestData();


   mFirebaseInterface->fetchReport();
}
