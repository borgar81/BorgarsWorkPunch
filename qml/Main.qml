import Felgo 3.0 as Felgo
import QtQuick 2.0

Felgo.App
{
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
         title: "Edit Projects"
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
         icon: Felgo.IconType.edit
         Felgo.NavigationStack
         {
            GenerateReportPage
            {
            }
         }
      }
   }
}
