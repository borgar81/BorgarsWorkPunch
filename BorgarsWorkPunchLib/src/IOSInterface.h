#pragma once

#include <QObject>
#include <QString>

class IOSInterface : public QObject
{
   Q_OBJECT

   signals:
       void mailCancelled();
       void mailSaved();
       void mailSent();
       void mailFailed();

   private:
       void *m_delegate;
   
   public:
      IOSInterface(QObject *parent=nullptr);
      ~IOSInterface();


      QString getMacAddress();

      Q_INVOKABLE void open(QString subject, QList<QString> toRecipients, QString body);
};
