#pragma once

#include <QObject>
#include <QDateTime>


class IOSInterface;

class SQLInterface;

class CppInterface : public QObject
{
   Q_OBJECT

   signals:
      void debugMessage(const QString &text);

   private:
      SQLInterface *mSQLInterface;

      IOSInterface *mIOSInterface;


   private slots:
      void onMailCancelled();
      void onMailSaved();
      void onMailSent();
      void onMailFailed();

   public:
      explicit CppInterface(SQLInterface *sqlinterface, QObject *parent = nullptr);

      Q_INVOKABLE bool sendEmailReport();
      Q_INVOKABLE int getCurrentWeekNumber() const;

      Q_INVOKABLE QDateTime getStartOfCurrentWeekDate() const;
      Q_INVOKABLE QDateTime getEndOfCurrentWeekDate() const;

   public slots:
      void onApplicationStateChanged(Qt::ApplicationState state);
};

