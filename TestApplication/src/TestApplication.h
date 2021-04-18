#pragma once

#include <QObject>

class FirebaseAuthHandler;
class FirebaseInterface;


class TestApplication : public QObject
{
   Q_OBJECT

   signals:

   private:
      FirebaseAuthHandler *mAutHandler;
      FirebaseInterface *mFirebaseInterface;

      void createTestData();

   private Q_SLOTS:
      void onUserLoggedIn(const QString &idToken, const QString &localID);

   public:
      explicit TestApplication(QObject *parent = nullptr);


   void start();
};

