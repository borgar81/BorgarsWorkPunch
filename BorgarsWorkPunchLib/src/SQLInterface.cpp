// Qt Includes
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QDir>
#include <QSqlError>
#include <QSqlRecord>
#include <QStandardPaths>
#include <QDateTime>

// Local Includes
//#include "Project.h"
#include "TimeRegistration.h"
#include "WeekData.h"
#include "SQLInterface.h"

QString SQLInterface::PROJECTS_TABLE_NAME             = "projects";
QString SQLInterface::PROJECT_REG_HISTORY_TABLE_NAME  = "project_reg_history";
QString SQLInterface::CURRENT_STATE_TABLE_NAME        = "current_state";
QString SQLInterface::DATABASE_VERSION_TABLE_NAME     = "db_version";

const QString DATABASE_FILENAME = "BorgarsWorkPunch.sqlite3";

/**
 * Constructor
 *
 * @param parent parent object
 */
SQLInterface::SQLInterface(QObject *parent)
   : QObject(parent)
{
   mTotalWorkedHoursToday = QStringLiteral("0.00");

   mCurrentWeekReportModel = new WeekReportModel(this);
}

/**
 * Destructor
 */
SQLInterface::~SQLInterface()
{
}

/**
 * Initializes the database
 *
 * @return true on success. False on failure
 */
bool SQLInterface::initializeDatabase()
{
   if(!openDatabase())
   {
      return false;
   }

   if(!createProjectsTable())
   {
      return false;
   }

   if(!createProjectRegHistoryTable())
   {
      return false;
   }

   if(!createCurrentStateTable())
   {
      return false;
   }

   /*if(!createCurrentProjectRegTable())
   {
      return false;
   }

   if(!createCurrentPunchRegTable())
   {
      return false;
   }*/

   if(!createDbVersionTable())
   {
      return false;
   }

   // Read Current state
   int projectID;
   QDateTime punchInTime;
   if (readCurrentStateFromSQL(projectID, punchInTime))
   {
      if (projectID != -1)
      {
         mPunchInTimestamp = punchInTime;
         setActiveProjectID(projectID);
      }
      qDebug() << "Current project: " << projectID;
      qDebug() << "punchinTime: " << punchInTime;
   }

   // Validate current PunchIn
   if(isPunchedIn())
   {
      // TODO  Should we re-enable this test?

      /*QDateTime currentTimeUTC = QDateTime::currentDateTimeUtc();
      QDateTime punchInTimeUTC;
      if(readCurrentPunchIn(punchInTimeUTC))
      {
         int daysBetween = punchInTimeUTC.toLocalTime().daysTo(currentTimeUTC.toLocalTime());
         if(daysBetween > 0)
         {
            QDateTime autoPunchOutTimestamp = punchInTimeUTC.toLocalTime();
            autoPunchOutTimestamp.setTime(QTime(23, 59, 59));
            punchOut(autoPunchOutTimestamp.toUTC());
         }
         qDebug() << "Days: " << daysBetween;
      }*/
   }

   //=========================================
   // DummyData Start
   //=========================================
   //Punch History
   /*insertDummyPunchInData(QDateTime(QDate(2016, 5, 30), QTime(8, 30)), QDateTime(QDate(2016, 5, 30), QTime(16, 30)));
   insertDummyPunchInData(QDateTime(QDate(2016, 5, 31), QTime(8, 45)), QDateTime(QDate(2016, 5, 31), QTime(17, 33)));
   insertDummyPunchInData(QDateTime(QDate(2016, 6, 1),  QTime(9, 45)), QDateTime(QDate(2016, 6, 1), QTime(19, 00)));
   insertDummyPunchInData(QDateTime(QDate(2016, 6, 2),  QTime(8, 0)),  QDateTime(QDate(2016, 6, 2), QTime(14, 12)));
   insertDummyPunchInData(QDateTime(QDate(2016, 6, 3),  QTime(10, 0)), QDateTime(QDate(2016, 6, 3), QTime(20, 48)));

   insertDummyPunchInData(QDateTime(QDate(2016, 5, 30),  QTime(20, 0)), QDateTime(QDate(2016, 5, 30), QTime(20, 48)));
   insertDummyPunchInData(QDateTime(QDate(2016, 6, 2),  QTime(16, 0)),  QDateTime(QDate(2016, 6, 2), QTime(18, 12)));
   insertDummyPunchInData(QDateTime(QDate(2016, 6, 2),  QTime(20, 0)),  QDateTime(QDate(2016, 6, 2), QTime(23, 22)));
   */


   //=========================================
   // DummyData End
   //=========================================

   return true;
}

/**
 * Creates the database file
 *
 * @return true on success. False on failure
 */
bool SQLInterface::openDatabase()
{
   QSqlDatabase database = QSqlDatabase::database();
   if (!database.isValid())
   {
      database = QSqlDatabase::addDatabase("QSQLITE");
      if (!database.isValid())
      {
         mErrorText = QStringLiteral("Cannot add database: %1").arg(database.lastError().text());
         return false;
      }
   }

   if (database.isOpen())
   {
      return true;
   }

   const QDir writeDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
   if (!writeDir.mkpath("."))
   {
      mErrorText = QStringLiteral("Failed to create writable directory at %1").arg(writeDir.absolutePath());
      return false;
   }

   // Ensure that we have a writable location on all devices.
   const QString fileName = writeDir.absolutePath() + "/" + DATABASE_FILENAME;

   qDebug() << "Database file: " << fileName;

   // When using the SQLite driver, open() will create the SQLite database if it doesn't exist.
   database.setDatabaseName(fileName);

   if (!database.open())
   {
     mErrorText = QStringLiteral("Cannot open database: %1").arg(database.lastError().text());
     QFile::remove(fileName);
     return false;
   }

   qDebug() << "Database opened!";

   return true;
}

void SQLInterface::closeDatabase()
{
   QSqlDatabase database = QSqlDatabase::database();
   if (database.isValid() && database.isOpen())
   {
      database.close();
      qDebug() << "Database closed!";
   }
}

/**
 * Creates the projects table
 *
 * @return true on success, false on failure
 */
bool SQLInterface::createProjectsTable()
{
   if (QSqlDatabase::database().tables().contains(PROJECTS_TABLE_NAME))
   {
      // The table already exists; we don't need to do anything.
     return true;
   }

   QSqlQuery query;
   if (!query.exec(QStringLiteral("CREATE TABLE %1(projectID INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL UNIQUE, "
                                  "network TEXT NOT NULL, activity INTEGER NOT NULL, projectType INTEGER NOT NULL, inclHomePage INTEGER NOT NULL)")
                                   .arg(PROJECTS_TABLE_NAME)))
   {
      mErrorText = QStringLiteral("Failed to query database: %1").arg(query.lastError().text());
      return false;
   }

   return true;
}

/**
 * Creates the TimeRegistration table
 *
 * @return true on success, false on failure
 */
bool SQLInterface::createProjectRegHistoryTable()
{
   if (QSqlDatabase::database().tables().contains(PROJECT_REG_HISTORY_TABLE_NAME))
   {
     // The table already exists; we don't need to do anything.
     return true;
   }

   QSqlQuery query;
   if (!query.exec(QStringLiteral("CREATE TABLE %1(id INTEGER PRIMARY KEY AUTOINCREMENT, projectID INTEGER, punchIn TEXT, punchOut TEXT)").arg(PROJECT_REG_HISTORY_TABLE_NAME)))
   {
      mErrorText = QStringLiteral("Failed to query database: %1").arg(query.lastError().text());
      return false;
   }

   return true;
}

/**
 * Creates the CurrentState table
 *
 * @return true on success, false on failure
 */
bool SQLInterface::createCurrentStateTable()
{
   if (QSqlDatabase::database().tables().contains(CURRENT_STATE_TABLE_NAME))
   {
     // The table already exists; we don't need to do anything.
     return true;
   }

   QSqlQuery query;
   if (!query.exec(QStringLiteral("CREATE TABLE %1(projectID INTEGER, punchInTime TEXT)").arg(CURRENT_STATE_TABLE_NAME)))
   {
      mErrorText = QStringLiteral("Failed to query database: %1").arg(query.lastError().text());
      return false;
   }

   // Insert default row
   if (!query.exec(QStringLiteral("INSERT INTO %1(projectID, punchInTime) VALUES(-1, '')").arg(CURRENT_STATE_TABLE_NAME)))
   {
      mErrorText = QStringLiteral("Failed to query database: %1").arg(query.lastError().text());
      return false;
   }

   return true;
}

/**
 * Creates the DbVersion table
 *
 * @return true on success, false on failure
 */
bool SQLInterface::createDbVersionTable()
{
   QSqlQuery query;

   // Create Table if it does not exist
   if (!QSqlDatabase::database().tables().contains(DATABASE_VERSION_TABLE_NAME))
   {
      if (!query.exec(QStringLiteral("CREATE TABLE %1(version INTEGER)").arg(DATABASE_VERSION_TABLE_NAME)))
      {
         mErrorText = QStringLiteral("Failed to query database: %1").arg(query.lastError().text());
         return false;
      }
   }

   // Delete Row
   query.exec(QStringLiteral("DELETE * FROM %1").arg(DATABASE_VERSION_TABLE_NAME));

   // Insert new Version
   if(!query.exec(QStringLiteral("INSERT INTO  %1(version) VALUES('%2')").arg(DATABASE_VERSION_TABLE_NAME).arg(DB_VERSION)))
   {
      mErrorText = QStringLiteral("Failed to query database: %1").arg(query.lastError().text());
      return false;
   }

   return true;
}

/**
 * @return true if we are punched in. Otherwise false
 */
bool SQLInterface::isPunchedIn() const
{
   /*QSqlQuery query;
   query.exec(QStringLiteral("SELECT * FROM %1").arg(CURRENT_PUNCH_REG_TABLE_NAME));
   if(query.next())
   {
      quint32 punchIn = query.value(0).toUInt();
      if(punchIn > 0)
      {
         return true;
      }
   }

   return false;*/

   return mPunchInTimestamp.isValid();
}

/**
 * @brief SQLInterface::registerProjectWork
 *
 * @param projectID the projectID
 * @param startTimeUTC the start time (UTC)
 * @param endTimeUTC  the end time (UTC)
 *
 * @return true on success, false on faillure
 */
bool SQLInterface::registerProjectWork(int projectID, const QDateTime &startTimeUTC, const QDateTime &endTimeUTC)
{
   // Add ProjectReg History
   QSqlQuery query;
   if(!query.exec(QStringLiteral("INSERT INTO %1(projectID, punchIn, punchOut) VALUES(%2, '%3', '%4')")
                     .arg(PROJECT_REG_HISTORY_TABLE_NAME)
                     .arg(projectID)
                     .arg(startTimeUTC.toString(Qt::ISODate))
                     .arg(endTimeUTC.toString(Qt::ISODate))))
   {
      qDebug() << QStringLiteral("SQLInterface::registerProjectWork: Failed to query database: %1").arg(query.lastError().text());
      return false;
   }

   return true;
}


/**
 * Adds a new project
 *
 * @param name the name (label)
 * @param network the network (or order)
 * @param activity the activity
 * @param projectType the project type (Network or Order)
 * @param includeInHomePage if true. The project is listed on the Home Page
 *
 * @return true on success. False on failure
 */
bool SQLInterface::addNewProject(const QString &name, const QString &network, int activity, int projectType, bool includeInHomePage)
{
   QSqlQuery query;

   QString str = QStringLiteral("insert into projects(name, network, activity, projectType, inclHomePage) VALUES('%1', '%2', '%3', %4, %5)")
                        .arg(name)
                        .arg(network)
                        .arg(activity)
                        .arg(projectType)
                        .arg(includeInHomePage);

   bool ok = query.exec(str);
   if (!ok)
   {
      qDebug() << query.lastError();
   }

   /*if(ok)
   {
      ok = query.exec("SELECT seq FROM sqlite_sequence where name='projects'");
      if(ok)
      {
         if(query.next())
         {
            int projectID = query.value(0).toUInt();

            //if(isDefault)
            //{
            //   setDefaultProject(projectID);
            //}
         }
      }
      else
      {
         qDebug() << query.lastError();
      }
   }*/

   return ok;
}

/**
 * Updates the project with the given ID
 *
 * @param projectID the ID of the project to update
 * @param name the name (label)
 * @param network the network (or order)
 * @param activity the activity
 * @param projectType the project type (Network or Order)
 * @param includeInHomePage if true. The project is listed on the Home Page
 *
 * @return true on success. False on failure
 */
bool SQLInterface::updateProject(int projectID, const QString &name, const QString &network, int activity, int projectType, bool includeInHomePage)
{
   QSqlQuery query;

   QString str = QStringLiteral("UPDATE projects SET name='%1', network='%2', activity='%3', projectType=%4, inclHomePage=%5 WHERE projectID=%6")
                        .arg(name)
                        .arg(network)
                        .arg(activity)
                        .arg(projectType)
                        .arg((int)includeInHomePage)
                        .arg(projectID);

   bool ok = query.exec(str);
   if(ok)
   {
      /*if(isDefault)
      {
         setDefaultProject(projectID);
      }*/

      emit projectUpdated(projectID);
   }
   else
   {
      qDebug() << QStringLiteral("Failed to query database: %1").arg(query.lastError().text());
   }


   return ok;
}

/**
 * Deletes the project with the given ID
 *
 * @param projectID the ID of the project to delete
 *
 * @return true on success. False on failure
 */
bool SQLInterface::deleteProject(int projectID)
{
   QSqlQuery query;
   QString str = QStringLiteral("DELETE FROM projects WHERE projectID=%1").arg(projectID);

   bool ok = query.exec(str);
   if(ok)
   {
      emit projectDeleted(projectID);
   }

   return ok;
}

/**
 * Fetch list of projects
 */
void SQLInterface::fetchProjectList()
{
   QVariantList tmpList;

   mProjectIDCrossRefMap.clear();

   // Get projects from SQL
   QSqlQuery query;
   query.exec(QStringLiteral("SELECT * FROM %1").arg(PROJECTS_TABLE_NAME));
   while(query.next())
   {
      int projectID = query.value(0).toInt();
      QString projectName = query.value(1).toString();

      QVariantMap map;
      map["Id"] = projectID;
      map["Name"] = projectName;
      map["NetworkOrOrder"] = query.value(2).toString();
      map["Activity"] = query.value(3).toString();
      map["Type"] = query.value(4).toInt();
      map["IncludeInHomePage"] = query.value(5).toBool();
      tmpList << map;

      mProjectIDCrossRefMap.insert(projectID, projectName);
   }

   mCurrentWeekReportModel->updateProjectIDCrossRefMap(mProjectIDCrossRefMap);

   setProjectList(tmpList);
}

void SQLInterface::fetchCurrentState()
{

}

void SQLInterface::fetchReport(const QDateTime &fromTimeLocalTime, const QDateTime &toTimeLocalTime)
{
   //QList<TimeRegistration> timeRegistrationList;

   QDateTime fromTimeUTC = fromTimeLocalTime.toUTC();
   QDateTime toTimeUTC = toTimeLocalTime.toUTC();

   WeekReport weekReport(fromTimeUTC, toTimeUTC);

   // Get projects from SQL
   QString queryStr = QStringLiteral("SELECT %1.projectID, name, punchIn, punchOut FROM %1 INNER JOIN %2 ON %2.projectID = %1.projectID WHERE punchIn >= '%3' AND punchIn < '%4' AND punchout >= '%3' AND punchout < '%4' ").
         arg(PROJECT_REG_HISTORY_TABLE_NAME)
         .arg(PROJECTS_TABLE_NAME)
         .arg(fromTimeUTC.toString(Qt::ISODate))
         .arg(toTimeUTC.toString(Qt::ISODate));

   QSqlQuery query;
   query.exec(queryStr);

   while(query.next())
   {
      //qDebug() << query.record();
      qDebug() << query.value(2).toDateTime();

      int projectID = query.value(0).toInt();
      QString projectName = query.value(1).toString();
      QDateTime punchInTimeUTC = query.value(2).toDateTime();
      QDateTime punchOutTimeUTC = query.value(3).toDateTime();

      punchInTimeUTC.setTimeSpec(Qt::UTC);
      punchOutTimeUTC.setTimeSpec(Qt::UTC);

      DayReport &dayReport = weekReport.mDayReportList[punchInTimeUTC.date().dayOfWeek()-1];
      dayReport.mTimeRegistrationList << TimeRegistration(projectID, projectName, punchInTimeUTC, punchOutTimeUTC);

      //timeRegistrationList << TimeRegistration(projectID, projectName, punchIn, punchOut);

      /*QVariantMap map;
      map["Id"] = query.value(0).toInt();
      map["Name"] = query.value(1).toString();
      map["NetworkOrOrder"] = query.value(2).toString();
      map["Activity"] = query.value(3).toString();
      map["Type"] = query.value(4).toInt();
      map["IncludeInHomePage"] = query.value(5).toBool();
      tmpList << map;*/
   }

   mCurrentWeekReportModel->updateWeekReport(weekReport);
}

bool SQLInterface::punchIn(int projectID)
{
   // Check if we are already checked in!
   if(isPunchedIn())
   {
      return false;
   }

   QSqlQuery query;

   /*// Use the first project if no project id default either
   if(projectID == -1)
   {
      query.exec(QStringLiteral("SELECT * FROM %1").arg(PROJECTS_TABLE_NAME));
      if(query.next())
      {
         projectID = query.value(0).toInt();
         changeCurrentProject(projectID);
      }
   }

   // No projects defined
   if(projectID == -1)
   {
      // TODO Report Error
      qDebug() << "No Projects!";
      return false;
   }*/



   mPunchInTimestamp = QDateTime::currentDateTimeUtc();

   // Current project
   if(!query.exec(QStringLiteral("UPDATE %1 SET projectID=%2, punchInTime='%3'").arg(CURRENT_STATE_TABLE_NAME).arg(projectID).arg(mPunchInTimestamp.toString(Qt::ISODate))))
   {
      qDebug() << QStringLiteral("Failed to query database: %1").arg(query.lastError().text());
      return false;
   }

   setActiveProjectID(projectID);

   return true;
}

bool SQLInterface::punchOut()
{
   // Do nothing if we're not chekced in!
   if(!isPunchedIn())
   {
      return false;
   }

   QDateTime endTimeUTC = QDateTime::currentDateTimeUtc();

   return punchOut(endTimeUTC);
}

/**
 * Punches out using the given timestamp
 *
 * @param endTimeUTC the end time
 *
 * @return true on success, false on failure
 */
bool SQLInterface::punchOut(const QDateTime &endTimeUTC)
{
   QSqlQuery query;
   QDateTime punchInTimeUTC;
   QDateTime projectStartTimeUTC;

   int projectID = -1;
   readCurrentStateFromSQL(projectID, projectStartTimeUTC);


   // Add Punch-in History
   if(!query.exec(QStringLiteral("INSERT INTO %1(punchIn, punchOut) VALUES('%2', '%3')")
                  .arg(PROJECT_REG_HISTORY_TABLE_NAME)
                  .arg(punchInTimeUTC.toString(Qt::ISODate))
                  .arg(endTimeUTC.toString(Qt::ISODate))))
   {
      qDebug() << QStringLiteral("Failed to query database: %1").arg(query.lastError().text());
      return false;
   }

   // Add ProjectReg History
   registerProjectWork(projectID,  projectStartTimeUTC, endTimeUTC);

   // Clear Current State
   if(!query.exec(QStringLiteral("UPDATE %1 SET projectID=-1, punchInTime=''").arg(CURRENT_STATE_TABLE_NAME)))
   {
      qDebug() << QStringLiteral("Failed to query database: %1").arg(query.lastError().text());
      return false;
   }

   mPunchInTimestamp = QDateTime();
   setActiveProjectID(-1);

   return true;
}


/**
 * Changes the current project. If we are punched in, we also punch in on this project
 *
 * @param projectID the project ID
 *
 * @return true on success, false on failure
 */
bool SQLInterface::changeCurrentProject(int projectID)
{
   QSqlQuery query;

   // Are we punched in?
   if(isPunchedIn())
   {
      QDateTime timeNowUTC = QDateTime::currentDateTimeUtc();

      // Register work for previously active project
      int prevCurrentProject;
      QDateTime prevCurrentProjectStartUTC;
      if(readCurrentStateFromSQL(prevCurrentProject, prevCurrentProjectStartUTC))
      {
         registerProjectWork(prevCurrentProject,  prevCurrentProjectStartUTC, timeNowUTC);
      }

      // Update current project
      if(!query.exec(QStringLiteral("UPDATE %1 SET projectID=%2, punchInTime='%3'").arg(CURRENT_STATE_TABLE_NAME).arg(projectID).arg(timeNowUTC.toString(Qt::ISODate))))
      {
         qDebug() << QStringLiteral("Failed to query database: %1").arg(query.lastError().text());
         return false;
      }
   }

   setActiveProjectID(projectID);

   return true;
}

/**
 * Deletes the whole database
 */
void SQLInterface::deleteDatabase()
{

   QSqlDatabase database = QSqlDatabase::database();
   database.close();

   const QDir writeDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
   const QString fileName = writeDir.absolutePath() + "/" + DATABASE_FILENAME;
   qDebug() << "DELETE DATABASE: " << QFile::remove(fileName);

   initializeDatabase();

   emit databaseDeleted();
}

/**
 * Calculates the total work time for today and updates the totalWorkedTimeToday property
 */
void SQLInterface::updateTotalTimeWorkedToday()
{
   QDateTime todayStart(QDate::currentDate(), QTime(0, 0, 0));
   QDateTime todayEnd(QDate::currentDate(), QTime(23, 59, 59));

   quint32 todaysTotalSeconds = getPunchInTotal(todayStart.toUTC(), todayEnd.toUTC());

   QString todaysTotalSecondsStr = QString::number(todaysTotalSeconds / 3600., 'f', 2);
   setTotalWorkedHoursToday(todaysTotalSecondsStr);
}

void SQLInterface::insertTestData()
{
   addNewProject("Gjøa Nova", "GM50750000", 110, (int)ProjectTypes::ProjectTypesEnum::Network, true);          // 1
   addNewProject("Gullfaks C", "GM50790000", 110, (int)ProjectTypes::ProjectTypesEnum::Network, true);         // 2
   addNewProject("FCS320 Dev", "48002213", 217, (int)ProjectTypes::ProjectTypesEnum::Network, true);           // 3
   addNewProject("Oseberg Sør", "GM50560000", 110, (int)ProjectTypes::ProjectTypesEnum::Network, true);        // 4
   addNewProject("Morramøte", "318046", -1, (int)ProjectTypes::ProjectTypesEnum::Order, true);                 // 5

   QDate monday = WeekData::getCurrentWeekStartDate();
   QDate tuesday = monday.addDays(1);
   QDate wednesday = monday.addDays(2);
   QDate thursday = monday.addDays(3);
   QDate friday = monday.addDays(4);
   QDate saturday = monday.addDays(5);
   QDate sunday = WeekData::getCurrentWeekEndDate();

   // Monday
   registerProjectWork(5, QDateTime(monday, QTime(8, 30, 0)).toUTC(), QDateTime(monday, QTime(9, 0, 0)).toUTC());       // Morramøte
   registerProjectWork(1, QDateTime(monday, QTime(9, 0, 0)).toUTC(), QDateTime(monday, QTime(16, 0, 0)).toUTC());       // Gjøa

   // Tuesday
   registerProjectWork(2, QDateTime(tuesday, QTime(8, 30, 0)).toUTC(), QDateTime(tuesday, QTime(12, 0, 0)).toUTC());       // Gullfaks C
   registerProjectWork(3, QDateTime(tuesday, QTime(12, 0, 0)).toUTC(), QDateTime(tuesday, QTime(14, 0, 0)).toUTC());       // FCS320 Dev
   registerProjectWork(2, QDateTime(tuesday, QTime(14, 00, 0)).toUTC(), QDateTime(tuesday, QTime(18, 0, 0)).toUTC());       // Gullfaks C

   // Wednesday
   registerProjectWork(2, QDateTime(wednesday, QTime(8, 30, 0)).toUTC(), QDateTime(wednesday, QTime(16, 0, 0)).toUTC());       // Oseberg Sør

   // Tuesday
   registerProjectWork(2, QDateTime(thursday, QTime(8, 30, 0)).toUTC(), QDateTime(thursday, QTime(12, 0, 0)).toUTC());       // Gullfaks C
   registerProjectWork(1, QDateTime(thursday, QTime(14, 00, 0)).toUTC(), QDateTime(thursday, QTime(18, 0, 0)).toUTC());      // Gjøa

   // Friday
   registerProjectWork(2, QDateTime(friday, QTime(8, 30, 0)).toUTC(), QDateTime(friday, QTime(16, 0, 0)).toUTC());       // Oseberg Sør

   // Saturday
   registerProjectWork(2, QDateTime(saturday, QTime(12, 00, 0)).toUTC(), QDateTime(saturday, QTime(16, 30, 0)).toUTC());       // Oseberg Sør

}

/**
 * Reads Current Project data from SQL
 *
 * @param projectID [OUT] the currently registered projectID
 * @param startTimeUTC [OUT] the currently registered start time
 *
 * @return true on success, false on failure
 */
bool SQLInterface::readCurrentStateFromSQL(int &projectID, QDateTime &startTimeUTC)
{
   QSqlQuery query;
   if(!query.exec(QStringLiteral("SELECT * FROM %1").arg(CURRENT_STATE_TABLE_NAME)))
   {
      qDebug() << QStringLiteral("SQLInterface::readCurrentProjectData: Failed to query database: %1").arg(query.lastError().text());
      return false;
   }

   if(query.next())
   {
      projectID = query.value(0).toInt();
      startTimeUTC = query.value(1).toDateTime();
      startTimeUTC.setTimeSpec(Qt::UTC);
      return true;
   }

   return false;
}

/**
 * Calculates the time worked from 'fromTime' to 'toTime'
 *
 * @param fromTimeUTC from time in UTC
 * @param endTimeUTC from time in UTC
 *
 * @return number of seconds worked between fromTime and endTime
 */
quint32 SQLInterface::getPunchInTotal(const QDateTime &fromTimeUTC, const QDateTime &endTimeUTC) const
{
   quint32 totalSecondsCount = 0;

   QSqlQuery query;

   bool ok = query.exec(QStringLiteral("SELECT punchIn, punchOut FROM %1 WHERE punchIn >= '%2' AND punchOut < '%3'")
                        .arg(PROJECT_REG_HISTORY_TABLE_NAME)
                        .arg(fromTimeUTC.toString(Qt::ISODate))
                        .arg(endTimeUTC.toString(Qt::ISODate)));
   if(!ok)
   {
      qDebug() << QStringLiteral("SQLInterface::getPunchInTotal: Failed to query database: %1").arg(query.lastError().text());
      return 0;
   }

   while(query.next())
   {
      quint32 secs = query.value(1).toDateTime().toTime_t() - query.value(0).toDateTime().toTime_t();
      totalSecondsCount += secs;
   }

   // Add current if we're punched in
   if(isPunchedIn())
   {
      query.exec(QStringLiteral("SELECT * FROM %1").arg(CURRENT_STATE_TABLE_NAME));
      if(query.next())
      {
         quint32 startTime = query.value(0).toDateTime().toTime_t();
         if(startTime > 0)
         {
            quint32 secs = QDateTime::currentDateTimeUtc().toTime_t() - query.value(1).toDateTime().toTime_t();
            totalSecondsCount += secs;
         }
      }
   }

   return totalSecondsCount;
}

/**
 * Calculates the time worked on a project from 'fromTime' to 'toTime'
 *
 * @param projectID the projectTime
 * @param fromTimeUTC from time in UTC
 * @param endTimeUTC from time in UTC
 *
 * @return time used on the given project
 */
QTime SQLInterface::getPunchInTotalForProject(int projectID, const QDateTime &fromTimeUTC, const QDateTime &endTimeUTC) const
{
   QTime totalTime(0, 0, 0);

   QSqlQuery query;

   bool ok = query.exec(QStringLiteral("SELECT punchIn, punchOut FROM %1 WHERE projectID = %2 AND punchIn >= '%3' AND punchOut < '%4'")
                        .arg(PROJECT_REG_HISTORY_TABLE_NAME)
                        .arg(projectID)
                        .arg(fromTimeUTC.toString(Qt::ISODate))
                        .arg(endTimeUTC.toString(Qt::ISODate)));
   if(!ok)
   {
      qDebug() << QStringLiteral("SQLInterface::getPunchInTotalForProject: Failed to query database: %1").arg(query.lastError().text());
      return QTime(0, 0);
   }

   while(query.next())
   {

      quint32 secs = query.value(1).toUInt() - query.value(0).toUInt();
      totalTime = totalTime.addSecs(secs);
   }

   // Add current if we're punched in on the given project
   if(isPunchedIn())
   {
      query.exec(QStringLiteral("SELECT punchIn FROM %1 WHERE projectID=%2").arg(CURRENT_STATE_TABLE_NAME).arg(projectID));
      if(query.next())
      {
         quint32 startTime = query.value(0).toUInt();
         if(startTime > 0)
         {
            quint32 secs = QDateTime::currentDateTimeUtc().toTime_t() - query.value(0).toUInt();
            totalTime = totalTime.addSecs(secs);
         }
      }
   }

   return totalTime;
}

/**
 * Returns the punch-data  for the given date
 *
 * @param date the date
 *
 * @return vector of punch-data data for the given date
 */
/*QVector<PunchData> SQLInterface::getPunchInDataForDate(const QDate &date) const
{
   QVector<PunchData> punchVector;

   QDateTime fromLocalTime(date, QTime(0, 0, 0));
   QDateTime toLocalTime(date, QTime(23, 59, 59));

   QSqlQuery query;
   bool ok = query.exec(QStringLiteral("SELECT punchIn, punchOut FROM %1 WHERE punchIn >= '%2' AND punchOut <= '%3'")
                        .arg(PUNCH_HISTORY_TABLE_NAME)
                        .arg(fromLocalTime.toUTC().toString(Qt::ISODate))
                        .arg(toLocalTime.toUTC().toString(Qt::ISODate)));

   if(!ok)
   {
      qDebug() << QStringLiteral("SQLInterface::getPunchInData: Failed to query database: %1").arg(query.lastError().text());
      return QVector<PunchData>();
   }

   while(query.next())
   {
      PunchData punchData;
      punchData.setFromTime(QDateTime::fromTime_t(query.value(0).toUInt()).toLocalTime().time());
      punchData.setToTime(QDateTime::fromTime_t(query.value(1).toUInt()).toLocalTime().time());

      punchVector << punchData;
   }

   return punchVector;
}*/

void SQLInterface::setActiveProjectID(int activeProjectID)
{
   if (activeProjectID != mActiveProjectID)
   {
      mActiveProjectID = activeProjectID;
      emit activeProjectIDChanged();
   }

}

void SQLInterface::setProjectList(const QVariantList &projectList)
{
   if (projectList != mProjectList)
   {
      mProjectList = projectList;
      emit projectListChanged();
   }
}
void SQLInterface::setTotalWorkedHoursToday(const QString &totalWorkedHoursToday)
{
   if (totalWorkedHoursToday != mTotalWorkedHoursToday)
   {
      mTotalWorkedHoursToday = totalWorkedHoursToday;
      emit totalWorkedHoursTodayChanged();
   }
}




