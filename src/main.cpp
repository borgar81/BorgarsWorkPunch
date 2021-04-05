#include <QApplication>
#include <FelgoApplication>
#include <QQmlContext>

#include <QQmlApplicationEngine>


#include "FirebaseAuthHandler.h"

// uncomment this line to add the Live Client Module and use live reloading with your custom C++ code
//#include <FelgoLiveClient>

// Firebase API key: AIzaSyCaK51HBaXFPKlMOd3-SxgQVAeFt9ygFXE

const QString API_KEY = "AIzaSyCaK51HBaXFPKlMOd3-SxgQVAeFt9ygFXE";


int main(int argc, char *argv[])
{
   QApplication app(argc, argv);

   QCoreApplication::setApplicationName("BorgarsWorkPunch");
   QCoreApplication::setOrganizationName("BorgarSoft");
   QCoreApplication::setOrganizationDomain("org.borgarsoft");

   FirebaseAuthHandler authHandler;
   authHandler.setAPIKey(API_KEY);

   //authHandler.signUserIn("Borgar.Ovsthus@technipfmc.com", "MyPassword");

   FelgoApplication felgo;

   // Use platform-specific fonts instead of Felgo's default font
   felgo.setPreservePlatformFonts(true);

   QQmlApplicationEngine engine;
   felgo.initialize(&engine);

   engine.rootContext()->setContextProperty("authHandler", &authHandler);

   // Set an optional license key from project file
   // This does not work if using Felgo Live, only for Felgo Cloud Builds and local builds
   felgo.setLicenseKey(PRODUCT_LICENSE_KEY);

   // use this during development
   // for PUBLISHING, use the entry point below
   felgo.setMainQmlFileName(QStringLiteral("qml/Main.qml"));

   // use this instead of the above call to avoid deployment of the qml files and compile them into the binary with qt's resource system qrc
   // this is the preferred deployment option for publishing games to the app stores, because then your qml files and js files are protected
   // to avoid deployment of your qml files and images, also comment the DEPLOYMENTFOLDERS command in the .pro file
   // also see the .pro file for more details
   // felgo.setMainQmlFileName(QStringLiteral("qrc:/qml/Main.qml"));

   engine.load(QUrl(felgo.mainQmlFileName()));

   // to start your project as Live Client, comment (remove) the lines "felgo.setMainQmlFileName ..." & "engine.load ...",
   // and uncomment the line below
   //FelgoLiveClient client (&engine);

   return app.exec();
}
