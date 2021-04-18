#include <QCoreApplication>

#include "TestApplication.h"

int main(int argc, char *argv[])
{
   QCoreApplication a(argc, argv);

   TestApplication testApplication;

   return a.exec();
}
