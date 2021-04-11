#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QVariantMap>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "FirebaseInterface.h"

/**
 * Constructor
 *
 * @param parent parent object
 */
FirebaseInterface::FirebaseInterface(QObject *parent)
   : QObject(parent)
{
   mNetworkAccessManager = new QNetworkAccessManager(this);
   connect(mNetworkAccessManager, &QNetworkAccessManager::finished, this, &FirebaseInterface::onReplyFinished);
}

/**
 * SLOT. Called when a network reply has finished
 */
QVariantList FirebaseInterface::getProjectList() const
{
   return mProjectList;
}

void FirebaseInterface::setProjectList(const QVariantList &projectList)
{
   if (projectList != mProjectList)
   {
      mProjectList = projectList;
      emit projectListChanged();
   }
}

void FirebaseInterface::onReplyFinished(QNetworkReply *reply)
{
   if (reply->error() != QNetworkReply::NoError)
   {
      reply->deleteLater();

      // TODO Report error
      return;
   }

   int messageType = reply->request().attribute((QNetworkRequest::Attribute )MessageTypeAttribute).toInt();

   QByteArray byteArray = reply->readAll();

   if (messageType == FetchProjectList)
   {
      QJsonDocument jsonDocument = QJsonDocument::fromJson( byteArray );

      QVariantList tmpList;

      QJsonObject jsonObject = jsonDocument.object();
      for(auto it = jsonObject.begin(), end = jsonObject.end(); it != end; ++it)
      {
         QVariantMap map = it.value().toObject().toVariantMap();
         map["Id"] = it.key();
         tmpList << map;
      }

      setProjectList(tmpList);
   }
   else if (messageType == NewProject)
   {
      fetchProjectList();  // Re-fetch the project list

      qDebug() << "NEW PROJECT RESPONSE: " << byteArray;
   }
   else if (messageType == UpdateProject)
   {
      fetchProjectList();  // Re-fetch the project list

      qDebug() << "UPDATE PROJECT RESPONSE: " << byteArray;
   }
   else
   {
   //parseSignUpResponse(response);

      qDebug() << "UNKNOWN RESPONSE: " << byteArray;
   }

   reply->deleteLater();
   //qDebug() << response;
}

void FirebaseInterface::addNewProject(const QString &name, const QString &type, const QString &networkOrOrder, int activity)
{
   QVariantMap variantMap;
   variantMap["Name"] = name;
   variantMap["Type"] = type;
   if (type == "Network")
   {
      variantMap["NetworkOrOrder"] = networkOrOrder;
   }
   else
   {
      variantMap["NetworkOrOrder"] = networkOrOrder.toInt();
   }

   variantMap["Activity"] = activity;

   QJsonDocument jsonDoc = QJsonDocument::fromVariant(variantMap);

   QString endPoint = QString("https://borgarsworkpunch-default-rtdb.europe-west1.firebasedatabase.app/Users/%1/Projects.json?auth=%2")
                  .arg(mLocalID)
                  .arg(mIDToken);

   QNetworkRequest request((QUrl(endPoint)));
   request.setAttribute((QNetworkRequest::Attribute )MessageTypeAttribute, NewProject);
   request.setHeader( QNetworkRequest::ContentTypeHeader, QString( "application/json"));
   mNetworkAccessManager->post(request, jsonDoc.toJson());
}

void FirebaseInterface::updateProject(const QString &projectID, const QString &name, const QString &type, const QString &networkOrOrder, int activity)
{
   QVariantMap variantMap;
   variantMap["Name"] = name;
   variantMap["Type"] = type;
   if (type == "Network")
   {
      variantMap["NetworkOrOrder"] = networkOrOrder;
   }
   else
   {
      variantMap["NetworkOrOrder"] = networkOrOrder.toInt();
   }

   variantMap["Activity"] = activity;

   QJsonDocument jsonDoc = QJsonDocument::fromVariant(variantMap);

   QString endPoint = QString("https://borgarsworkpunch-default-rtdb.europe-west1.firebasedatabase.app/Users/%1/Projects/%2.json?auth=%3")
                  .arg(mLocalID)
                  .arg(projectID)
                  .arg(mIDToken);

   QNetworkRequest request((QUrl(endPoint)));
   request.setAttribute((QNetworkRequest::Attribute )MessageTypeAttribute, UpdateProject);
   request.setHeader( QNetworkRequest::ContentTypeHeader, QString( "application/json"));
   mNetworkAccessManager->put(request, jsonDoc.toJson());

}

void FirebaseInterface::fetchProjectList()
{
   qDebug() << "FETCH NEW PROJECT LIST";

   QString endPoint = QString("https://borgarsworkpunch-default-rtdb.europe-west1.firebasedatabase.app/Users/%1/Projects.json?auth=%2")
                  .arg(mLocalID)
                  .arg(mIDToken);

   QNetworkRequest request((QUrl(endPoint)));
   request.setAttribute((QNetworkRequest::Attribute )MessageTypeAttribute, FetchProjectList);
   request.setHeader( QNetworkRequest::ContentTypeHeader, QString( "application/json"));
   mNetworkAccessManager->get(request);

}


/**
 * SLOT. Called when the user logs-in
 *
 * @param idToken the ID token
 * @param localID the Local ID
 */
void FirebaseInterface::onUserLoggedIn(const QString &idToken, const QString &localID)
{
   mIDToken = idToken;
   mLocalID = localID;

   fetchProjectList();
}

