#pragma once

#include <QObject>
#include <QNetworkRequest>
#include <QDateTime>

#include "WeekReport.h"

#include "WeekReportModel.h"  // TODO Should this be forward declared?

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
   Q_PROPERTY(WeekReport currentWeekReport READ getCurrentWeekReport NOTIFY currentWeekReportChanged)
   Q_PROPERTY(WeekReportModel *currentWeekReportModel READ getCurrentWeekReportModel CONSTANT)

   signals:
      void activeProjectIDChanged();
      void projectListChanged();
      void currentWeekReportChanged();

      void reportFetched(const QByteArray &bytearray);

   private:
      QString mIDToken;
      QString mLocalID;

      QString mActiveProjectID;
      QVariantList mProjectList;
      QDateTime mPunchInTimestamp;

      WeekReport mCurrentWeekReport;
      WeekReportModel *mCurrentWeekReportModel;

      QNetworkAccessManager *mNetworkAccessManager;

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
      Q_INVOKABLE void fetchCurrentState();

      Q_INVOKABLE void fetchReport(int year, int weekNumber);

      QVariantList getProjectList() const;

      void setProjectList(const QVariantList &projectList);

      void registerProjectWork(const QString &projectID, const QDateTime &startTimeUTC, const QDateTime &endTimeUTC);      // TODO Make private! Only public for testing with TestApplication

      QString getActiveProject() const;
      void setActiveProject(const QString &activeProjectID);

      void setIDToken(const QString &iDToken);

      void setLocalID(const QString &localID);

      WeekReport getCurrentWeekReport() const;

      WeekReportModel *getCurrentWeekReportModel() const;

public Q_SLOTS:
      void onUserLoggedIn(const QString &idToken, const QString &localID);
      void onIDTokenChanged(const QString &idToken);
};

