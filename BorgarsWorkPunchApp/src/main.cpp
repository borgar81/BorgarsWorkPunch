#include <QApplication>
#include <FelgoApplication>
#include <QQmlContext>
#include <FelgoLiveClient>

#include <QQmlApplicationEngine>

//DISABLE_FIREGBASE #include "BorgarsWorkPunchLib/src/FirebaseInterface.h"
//DISABLE_FIREGBASE #include "BorgarsWorkPunchLib/src/FirebaseAuthHandler.h"
#include "BorgarsWorkPunchLib/src/Constants.h"
#include "BorgarsWorkPunchLib/src/CppInterface.h"
#include "BorgarsWorkPunchLib/src/WeekReport.h"
#include "BorgarsWorkPunchLib/src/DayReport.h"
#include "BorgarsWorkPunchLib/src/WeekReportModel.h"
#include "BorgarsWorkPunchLib/src/SQLInterface.h"
#include "BorgarsWorkPunchLib/src/WeekDatesModel.h"

// uncomment this line to add the Live Client Module and use live reloading with your custom C++ code
//#include <FelgoLiveClient>

// Firebase API key: AIzaSyCaK51HBaXFPKlMOd3-SxgQVAeFt9ygFXE

//DISABLE_FIREGBASE const QString API_KEY = "AIzaSyCaK51HBaXFPKlMOd3-SxgQVAeFt9ygFXE";


int main(int argc, char *argv[])
{
   QApplication app(argc, argv);

   qRegisterMetaType<WeekReport>();
   qRegisterMetaType<DayReport>();
   qRegisterMetaType<WeekReportModel *>("WeekReportModel *");


   QCoreApplication::setApplicationName("BorgarsWorkPunch");
   QCoreApplication::setOrganizationName("BorgarSoft");
   QCoreApplication::setOrganizationDomain("org.borgarsoft");

   //DISABLE_FIREGBASE FirebaseAuthHandler authHandler;
   //DISABLE_FIREGBASE authHandler.setAPIKey(API_KEY);

   //DISABLE_FIREGBASE FirebaseInterface firebaseInterface;

   //DISABLE_FIREGBASE QObject::connect(&authHandler, &FirebaseAuthHandler::userSignedIn, &firebaseInterface, &FirebaseInterface::onUserLoggedIn);
   //DISABLE_FIREGBASE QObject::connect(&authHandler, &FirebaseAuthHandler::idTokenChanged, &firebaseInterface, &FirebaseInterface::onIDTokenChanged);

   //DISABLE_FIREGBASE authHandler.signUserIn("Borgar.Ovsthus@technipfmc.com", "MyPassword");

   //----------------------------
   // Build the Database
   //----------------------------
   QString dbErrorText;
   SQLInterface sqlInterface;
   bool hasDbError = sqlInterface.initializeDatabase();
   if(hasDbError)
   {
      dbErrorText = sqlInterface.getErrorText();
   }

   FelgoApplication felgo;

   // Use platform-specific fonts instead of Felgo's default font
   felgo.setPreservePlatformFonts(true);

   QQmlApplicationEngine engine;
   felgo.initialize(&engine);

   qmlRegisterUncreatableMetaObject(ProjectTypes::staticMetaObject, // static meta object
                                    "com.borgarsoft.BorgarsWorkPunch",        // import statement
                                    1, 0,                                     // major and minor version of the import
                                    "ProjectTypes",                           // name in QML
                                    "Error: only enums");                     // error in case someone tries to create a MyNamespace object

   qmlRegisterType<WeekDatesModel>("com.borgarsoft.BorgarsWorkPunch", 1, 0, "WeekDatesModel");

   CppInterface cppInterface(&sqlInterface);

   //DISABLE_FIREGBASE engine.rootContext()->setContextProperty("authHandler", &authHandler);
   //DISABLE_FIREGBASE engine.rootContext()->setContextProperty("firebaseInterface", &firebaseInterface);
   engine.rootContext()->setContextProperty("sqlInterface", &sqlInterface);
   engine.rootContext()->setContextProperty("cppInterface", &cppInterface);

   QObject::connect(&app, &QApplication::applicationStateChanged, &cppInterface, &CppInterface::onApplicationStateChanged);

   // Set an optional license key from project file
   // This does not work if using Felgo Live, only for Felgo Cloud Builds and local builds
   felgo.setLicenseKey(PRODUCT_LICENSE_KEY);

   // use this during development
   // for PUBLISHING, use the entry point below
   // felgo.setMainQmlFileName(QStringLiteral("qml/Main.qml"));

   // use this instead of the above call to avoid deployment of the qml files and compile them into the binary with qt's resource system qrc
   // this is the preferred deployment option for publishing games to the app stores, because then your qml files and js files are protected
   // to avoid deployment of your qml files and images, also comment the DEPLOYMENTFOLDERS command in the .pro file
   // also see the .pro file for more details
   felgo.setMainQmlFileName(QStringLiteral("qrc:/qml/Main.qml"));

   engine.load(QUrl(felgo.mainQmlFileName()));

   // to start your project as Live Client, comment (remove) the lines "felgo.setMainQmlFileName ..." & "engine.load ...",
   // and uncomment the line below
   //FelgoLiveClient client (&engine);

   return app.exec();
}
