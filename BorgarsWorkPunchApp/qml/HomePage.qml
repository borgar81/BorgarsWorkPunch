import QtQuick 2.0
import QtQuick.Controls 2.15 as QtQuickControls2
import Felgo 3.0 as Felgo
import com.borgarsoft.BorgarsWorkPunch 1.0

Felgo.Page
{
   title: qsTr("Home")

   onAppeared:
   {
      sqlInterface.fetchProjectList();
   }

   Felgo.AppText
   {
      id: todayCounterText
      //text: "Today: " + "0:00"
      text: "Today: " + Qt.formatTime(sqlInterface.totalWorkedTimeToday, "h:mm")
      anchors.left: parent.left
      anchors.top: parent.top
      anchors.leftMargin: 10
      anchors.topMargin: 10
   }
   Felgo.IconButton
   {
      id: refreshButton
      anchors.left: todayCounterText.right
      anchors.verticalCenter: todayCounterText.verticalCenter
      anchors.leftMargin: 10
      anchors.topMargin: 10
      icon: Felgo.IconType.refresh
      onClicked:
      {
         sqlInterface.updateTotalTimeWorkedToday();
      }
   }

   Felgo.AppListView
   {
      id: projectListView
      anchors.topMargin: dp(10)
      anchors.top: todayCounterText.bottom
      anchors.bottom: punchInButton.top
      anchors.left: parent.left
      anchors.right: parent.right

      delegate: Felgo.SimpleRow
      {
         style.showDisclosure: false
         text: model.Name
         active:
         {
            if (sqlInterface.activeProjectID === -1)
            {
               return index == projectListView.currentIndex
            }
            else
            {
               return sqlInterface.activeProjectID === model.Id
            }

         }

         detailText: (model.Type === ProjectTypes.Network ? "Network: " + model.NetworkOrOrder : "Order: " + model.NetworkOrOrder) + " " + "Activity: " + model.Activity
         onSelected:
         {
            projectListView.currentIndex = index

            if (sqlInterface.isPunchedIn())
            {
               sqlInterface.changeCurrentProject(model.Id)
            }

            //firebaseInterface.activeProjectID = model.Id
         }
      }

      model: Felgo.JsonListModel
      {
         keyField: "Id"
         fields: ["Id", "Name", "Type", "NetworkOrOrder", "Activity"]
         source: sqlInterface.projectList
      }
   }

   Felgo.AppButton
   {
      id: punchInButton
      text: "Punch-In"
      //backgroundColor: Qt.blue
      backgroundColor: checked ? Felgo.Theme.appButton.backgroundColor : "darkgreen"
      textColor: checked ? Felgo.Theme.appButton.textColor : "white"
      flat: false
      checked: sqlInterface.activeProjectID !== -1
      checkable: true
      anchors.left: parent.left
      anchors.bottom: parent.bottom
      onClicked:
      {
         var projectID = projectListView.model.get(projectListView.currentIndex).Id
         if(projectID)
         {
            sqlInterface.punchIn(projectID)
         }
      }
   }

   Felgo.AppButton
   {
      id: punchOutButton
      text: "Punch-Out"
      backgroundColor: checked ? Felgo.Theme.appButton.backgroundColor : "red"
      textColor: checked ? Felgo.Theme.appButton.textColor : "white"
      flat: false
      checkable: true
      checked: sqlInterface.activeProjectID === -1
      anchors.right: parent.right
      anchors.bottom: parent.bottom
      onClicked:
      {
         sqlInterface.punchOut()
      }
   }
}
