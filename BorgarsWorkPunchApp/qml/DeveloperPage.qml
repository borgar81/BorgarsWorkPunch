import QtQuick 2.0
import Felgo 3.0 as Felgo
import QtQuick.Dialogs 1.3 as QtDialogs

Felgo.Page
{
   title: qsTr("Developer Zone")

   Felgo.AppButton
   {
      text: "Delete DB"
      anchors.centerIn: parent
      flat: false
      onClicked:
      {
         messageDialog.open()
      }
   }

   QtDialogs.MessageDialog
   {
      id: messageDialog
      icon: QtDialogs.StandardIcon.Question
      title: "Delete Database"
      text: "Are you sure?"
      standardButtons: QtDialogs.StandardButton.Yes | QtDialogs.StandardButton.No
      onYes:
      {
         sqlInterface.deleteDatabase();
         navigationStack.pop();
      }
   }
}
