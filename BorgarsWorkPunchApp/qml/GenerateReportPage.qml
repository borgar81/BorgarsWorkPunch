import QtQuick 2.0
import Felgo 3.0 as Felgo
import QtQuick.Layouts 1.12
import com.borgarsoft.BorgarsWorkPunch 1.0


Felgo.Page
{
   id: root

   title: qsTr("Week Report")

   property var fromDateTime
   property var toDateTime

   Component.onCompleted:
   {
      root.fromDateTime = cppInterface.getStartOfCurrentWeekDate()
      root.toDateTime = cppInterface.getEndOfCurrentWeekDate()
      sqlInterface.fetchReport(fromDateTime, toDateTime)
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
         icon: Felgo.IconType.arrowleft
         onClicked:
         {
            root.fromDateTime = cppInterface.addDaysToDateTime(root.fromDateTime, -7);
            root.toDateTime = cppInterface.addDaysToDateTime(root.toDateTime, -7);
            sqlInterface.fetchReport(fromDateTime, toDateTime);
         }
      }

      Felgo.AppText
      {
         id: periodLabel
         text: Qt.formatDate(fromDateTime, "dd.MM.yyyy") + " - " + Qt.formatDate(toDateTime, "dd.MM.yyyy")
         //text: "03.05.2021 - 09.05.2021"
         Layout.fillWidth: true
         horizontalAlignment: Text.AlignHCenter
         verticalAlignment: Text.AlignVCenter
      }
      Felgo.IconButton
      {
         icon: Felgo.IconType.arrowright
         onClicked:
         {
            root.fromDateTime = cppInterface.addDaysToDateTime(root.fromDateTime, 7);
            root.toDateTime = cppInterface.addDaysToDateTime(root.toDateTime, 7);
            sqlInterface.fetchReport(fromDateTime, toDateTime);
         }
      }
   }

   RowLayout
   {
      id: subHeaderRow
      anchors.top: headerRow.bottom
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
         id: weekTotalHoursText
         text: "Week Total: " + sqlInterface.totalWorkedHoursWeek
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
      anchors.top: subHeaderRow.bottom
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

         console.debug("FROM: " + startDate)
         console.debug("TO  : " + endDate)

         sqlInterface.fetchReport(startDate, endDate)
      }
   }
}
