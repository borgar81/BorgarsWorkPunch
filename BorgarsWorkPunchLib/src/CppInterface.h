#pragma once

#include <QObject>

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

   public slots:
      void onApplicationStateChanged(Qt::ApplicationState state);
};

