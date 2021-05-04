#pragma once

#include <QObject>
#include <QDateTime>

class SQLInterface;

class CppInterface : public QObject
{
   Q_OBJECT

   signals:

   private:
      SQLInterface *mSQLInterface;


   public:
      explicit CppInterface(SQLInterface *sqlinterface, QObject *parent = nullptr);

      Q_INVOKABLE bool sendEmailReport();
      Q_INVOKABLE int getCurrentWeekNumber() const;

      Q_INVOKABLE QDateTime getStartOfCurrentWeekDate() const;
      Q_INVOKABLE QDateTime getEndOfCurrentWeekDate() const;

   public slots:
      void onApplicationStateChanged(Qt::ApplicationState state);
};

