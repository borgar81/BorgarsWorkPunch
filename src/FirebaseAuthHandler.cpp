// Qt Includes
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>

// Local Includes
#include "Constants.h"
#include "FirebaseAuthHandler.h"

/**
 * Constructor
 *
 * @param parent parent object
 */
FirebaseAuthHandler::FirebaseAuthHandler(QObject *parent)
   :QObject(parent)
{
   mNetworkAccessManager = new QNetworkAccessManager(this);
   connect(mNetworkAccessManager, &QNetworkAccessManager::finished, this, &FirebaseAuthHandler::onReplyFinished);
   connect(&mRefreshTokenTimer, &QTimer::timeout, this, &FirebaseAuthHandler::onRefreshTokenTimeout);
}

/**
 * Destructor
 */
FirebaseAuthHandler::~FirebaseAuthHandler()
{
}

/**
 * Signs the given user up
 *
 * @param emailAddress the email address
 * @param password the password
 */
void FirebaseAuthHandler::signUserUp(const QString &emailAddress, const QString &password)
{
   QString signUpEndpoint = "https://identitytoolkit.googleapis.com/v1/accounts:signUp?key=" + mAPIKey;

   QVariantMap variantPayload;
   variantPayload["email"] = emailAddress;
   variantPayload["password"] = password;
   variantPayload["returnSecureToken"] = true;

   QJsonDocument jsonPayload = QJsonDocument::fromVariant( variantPayload );
   performPOST( signUpEndpoint, jsonPayload, MessageTypes::SignUserUp );
}

/**
 * Signs the given user in
 *
 * @param emailAddress the email address
 * @param password the password
 */
void FirebaseAuthHandler::signUserIn(const QString &emailAddress, const QString &password)
{
   QString signInEndpoint = "https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=" + mAPIKey;

   QVariantMap variantPayload;
   variantPayload["email"] = emailAddress;
   variantPayload["password"] = password;
   variantPayload["returnSecureToken"] = true;

   QJsonDocument jsonPayload = QJsonDocument::fromVariant( variantPayload );
   performPOST( signInEndpoint, jsonPayload, MessageTypes::SignUserIn);
}

void FirebaseAuthHandler::onRefreshTokenTimeout()
{
   QString endPoint = "https://securetoken.googleapis.com/v1/token?key=" + mAPIKey;

   QVariantMap variantPayload;
   variantPayload["grant_type"] = "refresh_token";
   variantPayload["refresh_token"] = mRefreshToken;

   //QByteArray ba = "grant_type=refresh_token&refresh_token=";
   //ba += mRefreshToken.toUtf8();

   QJsonDocument jsonPayload = QJsonDocument::fromVariant( variantPayload );
   performPOST( endPoint, jsonPayload, MessageTypes::RefreshToken);
   /*QNetworkRequest request( (QUrl( url )) );
   request.setAttribute((QNetworkRequest::Attribute )MessageTypes::MessageTypeAttribute, MessageTypes::RefreshToken);
   request.setHeader( QNetworkRequest::ContentTypeHeader, QString("application/json"));
   mNetworkAccessManager->post( request, jsonPayload.toJson());
   //qDebug() << "PAYLOAD: " << ba;
   */

}


void FirebaseAuthHandler::performPOST(const QString &url, const QJsonDocument &payload, int messageType)
{
   QNetworkRequest request( (QUrl( url )) );
   request.setAttribute((QNetworkRequest::Attribute )MessageTypes::MessageTypeAttribute, messageType);
   request.setHeader( QNetworkRequest::ContentTypeHeader, QString("application/json"));
   mNetworkAccessManager->post( request, payload.toJson());
   //connect( m_networkReply, &QNetworkReply::readyRead, this, &AuthHandler::networkReplyReadyRead );
}

/**
 * SLOT. Called when a network reply has finished
 *
 * @param reply the network reply
 */
void FirebaseAuthHandler::onReplyFinished(QNetworkReply *reply)
{
   int messageType = reply->request().attribute((QNetworkRequest::Attribute )MessageTypes::MessageTypeAttribute).toInt();

   if (reply->error() != QNetworkReply::NoError)
   {
      QString errorText = reply->errorString();
      qDebug() << QString("FirebaseAuthHandler. Reply error for message type %1: %2").arg(messageType).arg(errorText);

     reply->deleteLater();

     // TODO Report error properly
     return;
   }

   QByteArray byteArray = reply->readAll();

   if (messageType == MessageTypes::SignUserUp)
   {
      qDebug() << "Sign User UP reply: " << byteArray;
   }
   else if (messageType == MessageTypes::SignUserIn)
   {
      QJsonDocument jsonDocument = QJsonDocument::fromJson( byteArray );
      mIDToken = jsonDocument.object().value("idToken").toString();
      mRefreshToken = jsonDocument.object().value("refreshToken").toString();
      QString localID = jsonDocument.object().value("localId").toString();
      setLocalID(localID);

      qDebug() << "User signed in successfully!";
      //qDebug() << "JSON Document: " << jsonDocument.object();
      //m_idToken = idToken;
      emit userSignedIn(mIDToken, mLocalID);

      mRefreshTokenTimer.start(1800 * 1000);    // 30 minutes
   }
   else if (messageType == MessageTypes::RefreshToken)
   {
      //qDebug() << "Refresh Token reply: " << byteArray;
      QJsonDocument jsonDocument = QJsonDocument::fromJson( byteArray );
      //mRefreshToken = jsonDocument.object().value("refreshToken").toString();

      mIDToken = jsonDocument.object().value("id_token").toString();
      emit idTokenChanged(mIDToken);

   }

   reply->deleteLater();
}

QString FirebaseAuthHandler::getLocalID() const
{
   return mLocalID;
}

void FirebaseAuthHandler::setLocalID(const QString &localID)
{
   if (localID != mLocalID)
   {
      mLocalID = localID;
      emit localIDChanged();
   }
}
