#pragma once

#include <QObject>
#include <QDateTime>
#include <QVariant>
#include <QTimer>
//#include "PunchData.h"

// Forward declarations
class Project;

/**
 * SQL Interface class
 *
 * @author Borgar Ovsthus
 *
 */
class SQLInterface : public QObject
{
   Q_OBJECT

   Q_PROPERTY(int activeProjectID READ getActiveProjectID WRITE setActiveProjectID NOTIFY activeProjectIDChanged);
   Q_PROPERTY(QVariantList projectList READ getProjectList NOTIFY projectListChanged);
   Q_PROPERTY(QTime totalWorkedTimeToday READ getTotalWorkedTimeToday  NOTIFY totalWorkedTimeTodayChanged);


   signals:
      void databaseDeleted();
      void projectDeleted(int projectID);
      void projectUpdated(int projectID);
      void activeProjectIDChanged();
      void projectListChanged();
      void totalWorkedTimeTodayChanged();

   private:
      QString mErrorText;

      int mActiveProjectID {-1};
      QVariantList mProjectList;

      QTime mTotalWorkedTimeToday;
      QDateTime mPunchInTimestamp;

      bool createProjectsTable();
      bool createProjectRegHistoryTable();
      bool createCurrentStateTable();
      bool createDbVersionTable();


      bool registerProjectWork(int projectID, const QDateTime &startTimeUTC, const QDateTime &endTimeUTC);
      bool readCurrentStateFromSQL(int &projectID, QDateTime &startTimeUTC);

      bool punchOut(const QDateTime &endTimeUTC);

   public:
      static QString PROJECTS_TABLE_NAME;
      static QString DEFAULT_PROJECT_TABLE_NAME;
      static QString PROJECT_REG_HISTORY_TABLE_NAME;
      static QString CURRENT_STATE_TABLE_NAME;
      static QString DATABASE_VERSION_TABLE_NAME;

      static const int DB_VERSION = 1;

      explicit SQLInterface(QObject *parent=nullptr);
      ~SQLInterface();


      bool initializeDatabase();

      bool openDatabase();
      void closeDatabase();

      Q_INVOKABLE bool addNewProject(const QString &name, const QString &network, int activity, int projectType, bool includeInHomePage);
      Q_INVOKABLE bool updateProject(int projectID, const QString &name, const QString &network, int activity, int projectType, bool includeInHomePage);
      Q_INVOKABLE bool deleteProject(int projectID);

      Q_INVOKABLE bool isPunchedIn() const;


      Q_INVOKABLE void fetchProjectList();
      Q_INVOKABLE void fetchCurrentState();

      Q_INVOKABLE bool punchIn(int projectID);
      Q_INVOKABLE bool punchOut();

      Q_INVOKABLE bool changeCurrentProject(int projectID);

      Q_INVOKABLE void deleteDatabase();

      Q_INVOKABLE void updateTotalTimeWorkedToday();

      QTime getPunchInTotal(const QDateTime &fromTimeUTC, const QDateTime &endTimeUTC) const;
      QTime getPunchInTotalForProject(int projectID, const QDateTime &fromTimeUTC, const QDateTime &endTimeUTC) const;

      //QVector<PunchData> getPunchInDataForDate(const QDate &date) const;

      // Get and Set methods
      QString getErrorText() const { return mErrorText; }

      int getActiveProjectID() const { return mActiveProjectID; }
      void setActiveProjectID(int activeProjectID);

      QVariantList getProjectList() const { return mProjectList; }
      void setProjectList(const QVariantList &projectList);


      QTime getTotalWorkedTimeToday() const { return mTotalWorkedTimeToday; }
      void setTotalWorkedTimeToday(const QTime &totalWorkedTimeToday);
};
