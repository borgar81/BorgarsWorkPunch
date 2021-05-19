#pragma once

#include <QObject>
#include <QDateTime>
#include <QVariant>
#include <QTimer>
//#include "PunchData.h"

#include "WeekReport.h"
#include "WeekReportModel.h"  // TODO Should this be forward declared?


// Forward declarations

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
   Q_PROPERTY(QString totalWorkedHoursToday READ getTotalWorkedHoursToday  NOTIFY totalWorkedHoursTodayChanged);
   Q_PROPERTY(QString totalWorkedHoursWeek READ getTotalWorkedHoursWeek  NOTIFY totalWorkedHoursWeekChanged);
   Q_PROPERTY(WeekReport currentWeekReport READ getCurrentWeekReport WRITE setCurrentWeekReport NOTIFY currentWeekReportChanged)
   Q_PROPERTY(WeekReportModel *currentWeekReportModel READ getCurrentWeekReportModel CONSTANT)

   signals:
      void databaseDeleted();
      void projectDeleted(int projectID);
      void projectUpdated(int projectID);
      void activeProjectIDChanged();
      void projectListChanged();
      void totalWorkedHoursTodayChanged();
      void totalWorkedHoursWeekChanged();
      void currentWeekReportChanged();

   private:
      QString mErrorText;

      int mActiveProjectID {-1};
      QVariantList mProjectList;

      QString mTotalWorkedHoursToday;
      QString mTotalWorkedHoursWeek;
      QDateTime mPunchInTimestamp;

      WeekReport mCurrentWeekReport;
      WeekReportModel *mCurrentWeekReportModel;

      /** Map containing ProjectIDs and Project Names. Key is ProjectID */
      QMap<int, QString> mProjectIDCrossRefMap;

      /** Map containing ProjectIDs and Project Data. Key is ProjectID */
      QMap<int, QVariantMap> mProjectMap;

      bool createProjectsTable(int currentVersion);
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

      static const int DB_VERSION = 2;

      explicit SQLInterface(QObject *parent=nullptr);
      ~SQLInterface();


      bool initializeDatabase();

      bool openDatabase();
      void closeDatabase();

      Q_INVOKABLE bool addNewProject(const QString &name, const QString &network, int activity, int projectType, bool includeInHomePage, const QString &aaType);
      Q_INVOKABLE bool updateProject(int projectID, const QString &name, const QString &network, int activity, int projectType, bool includeInHomePage, const QString &aaType);
      Q_INVOKABLE bool deleteProject(int projectID);

      Q_INVOKABLE bool isPunchedIn() const;


      Q_INVOKABLE void fetchProjectList();
      Q_INVOKABLE void fetchCurrentState();
      Q_INVOKABLE void fetchReport(const QDateTime &fromTimeLocalTime, const QDateTime &toTimeLocalTime);

      Q_INVOKABLE bool punchIn(int projectID);
      Q_INVOKABLE bool punchOut();

      Q_INVOKABLE bool changeCurrentProject(int projectID);

      Q_INVOKABLE void deleteDatabase();

      Q_INVOKABLE void updateTotalTimeWorkedToday();

      Q_INVOKABLE void insertTestData();

      quint32 getPunchInTotal(const QDateTime &fromTimeUTC, const QDateTime &endTimeUTC) const;
      QTime getPunchInTotalForProject(int projectID, const QDateTime &fromTimeUTC, const QDateTime &endTimeUTC) const;

      //QVector<PunchData> getPunchInDataForDate(const QDate &date) const;

      // Get and Set methods
      QString getErrorText() const { return mErrorText; }

      int getActiveProjectID() const { return mActiveProjectID; }
      void setActiveProjectID(int activeProjectID);

      QVariantList getProjectList() const { return mProjectList; }
      void setProjectList(const QVariantList &projectList);

      QString getTotalWorkedHoursToday() const { return mTotalWorkedHoursToday; }
      void setTotalWorkedHoursToday(const QString &totalWorkedHoursToday);

      QString getTotalWorkedHoursWeek() const { return mTotalWorkedHoursWeek; }
      void setTotalWorkedHoursWeek(const QString &totalWorkedHoursWeek);

      WeekReport getCurrentWeekReport() const { return mCurrentWeekReport; }
      void setCurrentWeekReport(const WeekReport &weekReport);

      WeekReportModel *getCurrentWeekReportModel() const { return mCurrentWeekReportModel; }

      QMap<int, QString> getProjectIDCrossRefMap() const;
      QMap<int, QVariantMap> getProjectMap() const;
};
