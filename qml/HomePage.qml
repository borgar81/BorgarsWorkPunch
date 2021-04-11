import QtQuick 2.0
import QtQuick.Controls 2.15 as QtQuickControls2
import Felgo 3.0 as Felgo

Felgo.Page
{
   title: qsTr("Home")

   Felgo.AppText
   {
      text: "Hours Today: " + "0:00"
      anchors.left: parent.left
      anchors.top: parent.top
      anchors.leftMargin: 10
      anchors.topMargin: 10
   }

   //QtQuickControls2.Button
   Felgo.AppButton
   {
      id: punchInButton
      text: "Punch-In"
      //backgroundColor: Qt.blue
      backgroundColor: checked ? "darkgreen" : Felgo.Theme.appButton.backgroundColor
      textColor: checked ? "white" : Felgo.Theme.appButton.textColor
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
         }
      }
   }

   Felgo.AppButton
   //QtQuickControls2.Button
   {
      id: punchOutButton
      text: "Punch-Out"
      backgroundColor: checked ? "red" : Felgo.Theme.appButton.backgroundColor
      textColor: checked ? "white" : Felgo.Theme.appButton.textColor
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
         }
      }
   }
}
