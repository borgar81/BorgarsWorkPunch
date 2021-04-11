// Qt Includes
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>

// Local Include3s
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
   performPOST( signUpEndpoint, jsonPayload );
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
   performPOST( signInEndpoint, jsonPayload );
}

void FirebaseAuthHandler::performPOST(const QString &url, const QJsonDocument &payload)
{
   QNetworkRequest newRequest( (QUrl( url )) );
   newRequest.setHeader( QNetworkRequest::ContentTypeHeader, QString( "application/json"));
   mNetworkAccessManager->post( newRequest, payload.toJson());
   //connect( m_networkReply, &QNetworkReply::readyRead, this, &AuthHandler::networkReplyReadyRead );
}

void FirebaseAuthHandler::parseSignUpResponse(const QByteArray &byteArray)
{
   QJsonDocument jsonDocument = QJsonDocument::fromJson( byteArray );
    if ( jsonDocument.object().contains("error") )
    {
        qDebug() << "Error occured!" << byteArray;
    }
    else if ( jsonDocument.object().contains("kind"))
    {
        mIDToken = jsonDocument.object().value("idToken").toString();
        QString localID = jsonDocument.object().value("localId").toString();
        setLocalID(localID);

        qDebug() << "User signed in successfully!";
        qDebug() << "JSON Document: " << jsonDocument.object();
        //m_idToken = idToken;
        emit userSignedIn(mIDToken, mLocalID);
    }
    else
        qDebug() << "The response was: " << byteArray;
}


/**
 * SLOT. Called when a network reply has finished
 */
void FirebaseAuthHandler::onReplyFinished(QNetworkReply *reply)
{
   QByteArray response = reply->readAll();
   parseSignUpResponse(response);

   reply->deleteLater();
   //qDebug() << response;
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
