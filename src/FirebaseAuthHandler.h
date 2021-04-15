#pragma once

#include <QObject>
#include <QTimer>

// Forward declarations
class QNetworkAccessManager;
class QNetworkReply;

/**
 * Auth Handler for Firebase
 *
 * @author Borgar Ovsthus
 */
class FirebaseAuthHandler : public QObject
{
   Q_OBJECT

   Q_PROPERTY(QString localID READ getLocalID WRITE setLocalID NOTIFY localIDChanged);

   Q_SIGNALS:
      void userSignedIn(const QString &idToken, const QString &localID);
      void idTokenChanged(const QString &idToken);
      void localIDChanged();


   private:
      QString mAPIKey;
      QNetworkAccessManager *mNetworkAccessManager;

      QString mIDToken;
      QString mRefreshToken;
      QString mLocalID;

      QTimer mRefreshTokenTimer;

      void performPOST(const QString &url, const QJsonDocument &payload, int messageType);

   private Q_SLOTS:
      void onReplyFinished(QNetworkReply *reply);
      void onRefreshTokenTimeout();


   public:
      explicit FirebaseAuthHandler(QObject *parent=nullptr);
      ~FirebaseAuthHandler();

      void setAPIKey(const QString &aPIKey) { mAPIKey = aPIKey; }

      Q_INVOKABLE void signUserUp(const QString &emailAddress, const QString &password);
      Q_INVOKABLE void signUserIn(const QString &emailAddress, const QString &password);
      QString getLocalID() const;
      void setLocalID(const QString &localID);
};

