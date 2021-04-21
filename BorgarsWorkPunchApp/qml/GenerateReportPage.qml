import QtQuick 2.0
import Felgo 3.0 as Felgo
import QtQuick.Layouts 1.12

Felgo.Page
{
   title: qsTr("Generate Report")
   Felgo.AppText
   {
      text: "Report page"
      anchors.centerIn: parent
   }

   Component.onCompleted:
   {
      firebaseInterface.fetchReport(2021, 15)
   }

   Rectangle
   {
      color: "lightgray"
      anchors.fill: headerRow

   }


   RowLayout
   {
      id: headerRow
      anchors.left: parent.left
      anchors.right: parent.right

      Felgo.IconButton
      {
         icon: Felgo.IconType.calendar
      }

      Felgo.AppText
      {

         id: periodLabel
         text: "12.04.2021 - 18.04.2021"
         Layout.fillWidth: true
         horizontalAlignment: Text.AlignHCenter
         verticalAlignment: Text.AlignVCenter
         //onClicked:
      }
      Felgo.IconButton
      {
         icon: Felgo.IconType.envelope
         onClicked:
         {
            cppInterface.sendEmailReport();
         }
      }

   }

   Felgo.AppListView
   {
      anchors.topMargin: dp(10)
      anchors.top: headerRow.bottom
      anchors.bottom: parent.bottom
      anchors.left: parent.left
      anchors.right: parent.right

      delegate: Felgo.SimpleRow
      {
         style.showDisclosure: false
         text: model.WeekDayName
         detailText: model.WeekReportDay
         //iconSource: Felgo.IconType.edit
      }

      model: firebaseInterface.currentWeekReportModel
   }


}
