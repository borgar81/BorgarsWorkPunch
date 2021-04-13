#pragma once

#include <QObject>
#include <QNetworkRequest>
#include <QDateTime>

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

   Q_PROPERTY(QString activeProjectID READ getActiveProject WRITE setActiveProject NOTIFY activeProjectIDChanged);
   Q_PROPERTY(QVariantList projectList READ getProjectList NOTIFY projectListChanged);

   signals:
      void activeProjectIDChanged();
      void projectListChanged();

   private:
      enum MessageTypes
      {
         FetchProjectList = 10,
         NewProject,
         UpdateProject,
         PunchIn,
         PunchOut,
         RegisterProjectWork

      };

      static const int MessageTypeAttribute = QNetworkRequest::User + 1;

      QString mIDToken;
      QString mLocalID;

      QString mActiveProjectID;
      QVariantList mProjectList;
      QDateTime mPunchInTimestamp;

      QNetworkAccessManager *mNetworkAccessManager;

      void registerProjectWork(const QString &projectID, const QDateTime &startTimeUTC, const QDateTime &endTimeUTC);

   private Q_SLOTS:
      void onReplyFinished(QNetworkReply *reply);

   public:
      explicit FirebaseInterface(QObject *parent = nullptr);


      Q_INVOKABLE void addNewProject(const QString &name, const QString &type, const QString &networkOrOrder, int activity);
      Q_INVOKABLE void updateProject(const QString &projectID, const QString &name, const QString &type, const QString &networkOrOrder, int activity);
      Q_INVOKABLE void punchIn(const QString &projectID);
      Q_INVOKABLE void punchOut();
      Q_INVOKABLE void changeCurrentProject(const QString &projectID);
      Q_INVOKABLE bool isPunchedIn() const;

      Q_INVOKABLE void fetchProjectList();

      QVariantList getProjectList() const;

      void setProjectList(const QVariantList &projectList);

      QString getActiveProject() const;
      void setActiveProject(const QString &activeProjectID);

public Q_SLOTS:
      void onUserLoggedIn(const QString &idToken, const QString &localID);
};
