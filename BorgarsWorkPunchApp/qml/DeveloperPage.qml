import QtQuick 2.0
import Felgo 3.0 as Felgo
import QtQuick.Dialogs 1.3 as QtDialogs
import QtQuick.Controls 2.12 as QtQuickControls

Felgo.Page
{
   title: qsTr("Developer Zone")

   Connections
   {
       target: cppInterface
       onDebugMessage:
       {
           debugEdit.append(text + "\n");
       }
   }

   Column
   {
      anchors.fill: parent

      Felgo.AppButton
      {
         text: "Delete DB"
         flat: false
         onClicked:
         {
            deleteDatabaseMessageBox.open()
         }
      }

      Felgo.AppButton
      {
         text: "Insert TestData"
         flat: false
         onClicked:
         {
            insertTestDataMessageBox.open()
         }
      }

      TextEdit
      {
         id: debugEdit
         //readOnly: true
         wrapMode: TextEdit.WrapAnywhere
      }
   }

   QtDialogs.MessageDialog
   {
      id: deleteDatabaseMessageBox
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

   QtDialogs.MessageDialog
   {
      id: insertTestDataMessageBox
      icon: QtDialogs.StandardIcon.Question
      title: "Insert Test-data"
      text: "Are you sure?"
      standardButtons: QtDialogs.StandardButton.Yes | QtDialogs.StandardButton.No
      onYes:
      {
         sqlInterface.insertTestData();
         navigationStack.pop();
      }
   }

}
