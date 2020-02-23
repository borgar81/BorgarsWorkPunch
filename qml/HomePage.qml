import QtQuick 2.0
import QtQuick.Controls 2.0 as QtQuickControls2
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

   QtQuickControls2.ButtonGroup {id: punchButtonGroup }


   QtQuickControls2.Button
   //Felgo.AppButton
   {
      text: "Punch-In"
      flat: false
      checkable: true
      QtQuickControls2.ButtonGroup.group: punchButtonGroup
      anchors.left: parent.left
      anchors.bottom: parent.bottom
   }

   //Felgo.AppButton
   QtQuickControls2.Button
   {
      text: "Punch-Out"
      flat: false
      checkable: true
      checked: true
      QtQuickControls2.ButtonGroup.group: punchButtonGroup
      anchors.right: parent.right
      anchors.bottom: parent.bottom
   }
}
