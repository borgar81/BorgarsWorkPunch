import QtQuick 2.0
import QtQuick.Controls 2.15 as QtQuickControls2
import Felgo 3.0 as Felgo

Felgo.Page
{
   title: qsTr("Home")

   Felgo.AppText
   {
      id: todayCounterText
      text: "Today: " + "0:00"
      anchors.left: parent.left
      anchors.top: parent.top
      anchors.leftMargin: 10
      anchors.topMargin: 10
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
         text: model.Name
         active: index == projectListView.currentIndex
         /*{
            if (firebaseInterface.activeProjectID.length == 0)
            {
               return index === 0
            }
            else
            {
               return firebaseInterface.activeProjectID === model.Id
            }

         }*/

         detailText: (model.Type === "Network" ? "Network: " + model.NetworkOrOrder : "Order: " + model.NetworkOrOrder) + " " + "Activity: " + model.Activity
         onSelected:
         {
            projectListView.currentIndex = index

            if (firebaseInterface.isPunchedIn())
            {
               firebaseInterface.changeCurrentProject(model.Id)
            }

            //firebaseInterface.activeProjectID = model.Id
         }
      }

      model: Felgo.JsonListModel
      {
         keyField: "Id"
         fields: ["Id", "Name", "Type", "NetworkOrOrder", "Activity"]
         source: firebaseInterface.projectList
      }
   }

   //QtQuickControls2.Button
   Felgo.AppButton
   {
      id: punchInButton
      text: "Punch-In"
      //backgroundColor: Qt.blue
      backgroundColor: checked ? Felgo.Theme.appButton.backgroundColor : "darkgreen"
      textColor: checked ? Felgo.Theme.appButton.textColor : "white"
      flat: false
      checked: false
      checkable: true
      anchors.left: parent.left
      anchors.bottom: parent.bottom
      onClicked:
      {
         if (!checked)
         {
            checked = true
            punchOutButton.checked = false

            var projectID = projectListView.model.get(projectListView.currentIndex).Id
            if(projectID)
            {
               firebaseInterface.punchIn(projectID)
            }
         }
      }
   }

   Felgo.AppButton
   //QtQuickControls2.Button
   {
      id: punchOutButton
      text: "Punch-Out"
      backgroundColor: checked ? Felgo.Theme.appButton.backgroundColor : "red"
      textColor: checked ? Felgo.Theme.appButton.textColor : "white"
      flat: false
      checkable: true
      checked: true
      anchors.right: parent.right
      anchors.bottom: parent.bottom
      onClicked:
      {
         if (!checked)
         {
            checked = true
            punchInButton.checked = false
            firebaseInterface.punchOut()
         }
      }
   }
}
