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

QString FirebaseInterface::getActiveProject() const
{
   return mActiveProjectID;
}

void FirebaseInterface::setActiveProject(const QString &activeProjectID)
{
   if (activeProjectID != mActiveProjectID)
   {
      mActiveProjectID = activeProjectID;
      emit activeProjectIDChanged();
   }
}

/**
 * We start working
 *
 * @param projectID the ID of the project we start working on
 */
void FirebaseInterface::punchIn(const QString &projectID)
{
   QVariantMap variantMap;
   variantMap["PunchInTime"] = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
   variantMap["ProjectID"] = projectID;

   QJsonDocument jsonDoc = QJsonDocument::fromVariant(variantMap);
   QString endPoint = QString("https://borgarsworkpunch-default-rtdb.europe-west1.firebasedatabase.app/Users/%1/CurrentState.json?auth=%2")
                  .arg(mLocalID)
                  .arg(mIDToken);

   QNetworkRequest request((QUrl(endPoint)));
   request.setAttribute((QNetworkRequest::Attribute )MessageTypeAttribute, PunchIn);
   request.setHeader( QNetworkRequest::ContentTypeHeader, QString( "application/json"));
   mNetworkAccessManager->put(request, jsonDoc.toJson());
}

/**
 * We stop working
 */
void FirebaseInterface::punchOut()
{
   //-------------------------------
   // Register Work Time
   //-------------------------------
   registerProjectWork(mActiveProjectID, mPunchInTimestamp, QDateTime::currentDateTimeUtc());

   //-------------------------------
   // Clear current state
   //-------------------------------
   QVariantMap variantMap;
   variantMap["PunchInTime"] = QString();
   variantMap["ProjectID"] = QString();

   QJsonDocument jsonDoc = QJsonDocument::fromVariant(variantMap);
   QString endPoint = QString("https://borgarsworkpunch-default-rtdb.europe-west1.firebasedatabase.app/Users/%1/CurrentState.json?auth=%2")
                  .arg(mLocalID)
                  .arg(mIDToken);

   QNetworkRequest request((QUrl(endPoint)));
   request.setAttribute((QNetworkRequest::Attribute )MessageTypeAttribute, PunchOut);
   request.setHeader( QNetworkRequest::ContentTypeHeader, QString( "application/json"));
   mNetworkAccessManager->put(request, jsonDoc.toJson());
}

/**
 * Changes the current project. If we are punched in, we register the current work time and start a new project
 *
 * @param projectID the project ID to change to
 */
void FirebaseInterface::changeCurrentProject(const QString &projectID)
{
   // Are we punched in?
   if (mPunchInTimestamp.isValid())
   {
      QDateTime timeNowUTC = QDateTime::currentDateTimeUtc();
      registerProjectWork(mActiveProjectID, mPunchInTimestamp, timeNowUTC);
   }

   punchIn(projectID);
}

bool FirebaseInterface::isPunchedIn() const
{
   return mPunchInTimestamp.isValid();
}

/**
 * Store project work time in database
 *
 * @param projectID the projectID
 * @param startTimeUTC the start time (UTC)
 * @param endTimeUTC  the end time (UTC)
 */
void FirebaseInterface::registerProjectWork(const QString &projectID, const QDateTime &startTimeUTC, const QDateTime &endTimeUTC)
{
   QVariantMap variantMap;
   variantMap["ProjectID"] = projectID;
   variantMap["PunchInTime"] = startTimeUTC.toString(Qt::ISODate);
   variantMap["PunchOutTime"] = endTimeUTC.toString(Qt::ISODate);

   QJsonDocument jsonDoc = QJsonDocument::fromVariant(variantMap);
   QString endPoint = QString("https://borgarsworkpunch-default-rtdb.europe-west1.firebasedatabase.app/Users/%1/TimeRegistration/%2/%3.json?auth=%4")
                  .arg(mLocalID)
                  .arg(endTimeUTC.date().year())
                  .arg(endTimeUTC.date().weekNumber())
                  .arg(mIDToken);

   QNetworkRequest request((QUrl(endPoint)));
   request.setAttribute((QNetworkRequest::Attribute )MessageTypeAttribute, RegisterProjectWork);
   request.setHeader( QNetworkRequest::ContentTypeHeader, QString( "application/json"));
   mNetworkAccessManager->post(request, jsonDoc.toJson());
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
   else if (messageType == PunchIn)
   {
      QJsonDocument jsonDocument = QJsonDocument::fromJson( byteArray );
      QJsonObject jsonObject = jsonDocument.object();
      mPunchInTimestamp = QDateTime::fromString(jsonObject.value("PunchInTime").toString(), Qt::ISODate);
      QString projectID = jsonObject.value("ProjectID").toString();
      setActiveProject(projectID);
   }
   else if (messageType == PunchOut)
   {
      qDebug() << "PUNCH-OUT RESPONSE: " << byteArray;
      mPunchInTimestamp = QDateTime();
   }
   else if (messageType == RegisterProjectWork)
   {
      qDebug() << "REGISTER PROJECT WORK RESPONSE: " << byteArray;
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
