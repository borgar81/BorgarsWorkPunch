import Felgo 3.0 as Felgo
import QtQuick 2.0
import QtQuick.Window 2.15

Felgo.App
{
   x: (Qt.platform.os === "linux") ? ((Screen.desktopAvailableWidth / 3) - width/2) : 0
   //storeWindowGeometry: false

   // You get free licenseKeys from https://felgo.com/licenseKey
   // With a licenseKey you can:
   //  * Publish your games & apps for the app stores
   //  * Remove the Felgo Splash Screen or set a custom one (available with the Pro Licenses)
   //  * Add plugins to monetize, analyze & improve your apps (available with the Pro Licenses)
   //licenseKey: "<generate one from https://felgo.com/licenseKey>"

   Felgo.Navigation
   {
      Felgo.NavigationItem
      {
         title: "Home"
         icon: Felgo.IconType.home
         Felgo.NavigationStack
         {
            HomePage
            {

            }
         }
      }
      Felgo.NavigationItem
      {
         title: "Projects"
         icon: Felgo.IconType.edit
         Felgo.NavigationStack
         {
            EditProjectsPage
            {
            }
         }
      }
      Felgo.NavigationItem
      {
         title: "Report"
         icon: Felgo.IconType.filetexto
         Felgo.NavigationStack
         {
            GenerateReportPage
            {
            }
         }
      }
      Felgo.NavigationItem
      {
         title: "Developer"
         icon: Felgo.IconType.code
         Felgo.NavigationStack
         {
            DeveloperPage
            {
            }
         }
      }
   }

   /*SignInPage
   {
      visible: authHandler.localID.length == 0
   }*/

}
