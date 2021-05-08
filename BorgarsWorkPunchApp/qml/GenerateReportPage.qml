import QtQuick 2.0
import Felgo 3.0 as Felgo
import QtQuick.Layouts 1.12
import com.borgarsoft.BorgarsWorkPunch 1.0


Felgo.Page
{
   id: root

   title: qsTr("Week Report")

   Component.onCompleted:
   {
      //firebaseInterface.fetchReport(2021, 15)
      sqlInterface.fetchReport(cppInterface.getStartOfCurrentWeekDate(), cppInterface.getEndOfCurrentWeekDate())
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
         icon: Felgo.IconType.envelope
         onClicked:
         {
            cppInterface.sendEmailReport();
         }
      }

      Felgo.AppText
      {

         id: periodLabel
         text: "03.05.2021 - 09.05.2021"
         Layout.fillWidth: true
         horizontalAlignment: Text.AlignHCenter
         verticalAlignment: Text.AlignVCenter
      }
      Felgo.IconButton
      {
         icon: Felgo.IconType.calendar
         onClicked:
         {
            weekPickerDialog.open()
         }
      }
   }

   Felgo.AppListView
   {
      id: detailedReportView
      anchors.topMargin: dp(10)
      anchors.top: headerRow.bottom
      anchors.bottom: reportTypeButtonRow.top
      anchors.left: parent.left
      anchors.right: parent.right

      delegate: Felgo.SimpleRow
      {
         style.showDisclosure: false
         text: model.WeekDayName
         detailText: model.WeekReportDay
      }
      model: sqlInterface.currentWeekReportModel
   }

   Felgo.AppTabBar
   {
      id: reportTypeButtonRow
      //anchors.left: parent.left
      //anchors.right: parent.right
      anchors.bottom: parent.bottom
      Felgo.AppTabButton
      {
         text: qsTr("Hour totals")
         onClicked:
         {
            sqlInterface.currentWeekReportModel.reportFormat = WeekReportTypes.Totals
         }
      }
      Felgo.AppTabButton
      {
         text: qsTr("Punch-Ins")
         onClicked:
         {
            sqlInterface.currentWeekReportModel.reportFormat = WeekReportTypes.PunchIns
         }
      }
   }

   WeekPickerDialog
   {
      id: weekPickerDialog
      onAccepted:
      {
         periodLabel.text = Qt.formatDate(startDate, "dd.MM.yyyy") + " - " + Qt.formatDate(endDate, "dd.MM.yyyy")
         sqlInterface.fetchReport(startDate, endDate)
      }
   }
}
