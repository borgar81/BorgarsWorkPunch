#pragma once

#include <QObject>

class CppInterface : public QObject
{
   Q_OBJECT

   signals:

   public:
      explicit CppInterface(QObject *parent = nullptr);



};

