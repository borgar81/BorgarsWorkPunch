#pragma once

#include <QObject>
#include <QNetworkRequest>

class QNetworkReply;
class QNetworkAccessManager;

/**
 * Firebase Interface
 *
 * @author Borgar Ovsthus
 */
class FirebaseInterface : public QObject
{
   Q_OBJECT

   Q_PROPERTY(QVariantList projectList READ getProjectList NOTIFY projectListChanged);

   signals:
      void projectListChanged();

   private:
      enum MessageTypes
      {
         FetchProjectList = 10,
         NewProject,
         UpdateProject
      };

      static const int MessageTypeAttribute = QNetworkRequest::User + 1;

      QString mIDToken;
      QString mLocalID;

      QVariantList mProjectList;

      QNetworkAccessManager *mNetworkAccessManager;

   private Q_SLOTS:
      void onReplyFinished(QNetworkReply *reply);


   public:
      explicit FirebaseInterface(QObject *parent = nullptr);


      Q_INVOKABLE void addNewProject(const QString &name, const QString &type, const QString &networkOrOrder, int activity);
      Q_INVOKABLE void updateProject(const QString &projectID, const QString &name, const QString &type, const QString &networkOrOrder, int activity);

      Q_INVOKABLE void fetchProjectList();

      QVariantList getProjectList() const;

      void setProjectList(const QVariantList &projectList);

public Q_SLOTS:
      void onUserLoggedIn(const QString &idToken, const QString &localID);
};

