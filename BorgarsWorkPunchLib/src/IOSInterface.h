#pragma once

//#include <QObject>
#include <QString>

class IOSInterface
{
   //Q_OBJECT
   
   public:
      IOSInterface();
      ~IOSInterface();

      QString getMacAddress();
};
