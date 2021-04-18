import QtQuick 2.12
import QtQuick.Layouts 1.12
import Felgo 3.0 as Felgo


Felgo.Page
{
   id: root


   property string projectID: ""
   property bool isCreateProject : true

   property alias name : nameEdit.text
   property string type: "Network"
   property alias network : networkEdit.text
   property alias order : orderEdit.text
   property alias activity : activityEdit.text


   title: isCreateProject ? qsTr("New Project") : qsTr("Edit Project")


   rightBarItem: Felgo.TextButtonBarItem
   {
      text: qsTr("Save")
      onClicked:
      {
         //navigationStack.push(subPage)

         //-----------------------
         // Create new project
         //-----------------------
         if (isCreateProject)
         {
            if(networkRadio.checked)
            {
               firebaseInterface.addNewProject(nameEdit.text, "Network", networkEdit.text, parseInt(activityEdit.text))
            }
            else
            {
               firebaseInterface.addNewProject(nameEdit.text, "Order", orderEdit.text, parseInt(activityEdit.text))
            }
         }
         //-----------------------
         // Edit Project
         //-----------------------
         else
         {
            if(networkRadio.checked)
            {
               firebaseInterface.updateProject(root.projectID, nameEdit.text, "Network", networkEdit.text, parseInt(activityEdit.text))
            }
            else
            {
               firebaseInterface.updateProject(root.projectID, nameEdit.text, "Order", orderEdit.text, parseInt(activityEdit.text))
            }
         }

         navigationStack.pop()
      }
   }


   GridLayout
   {
      columns:3
      anchors.top: parent.top
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.topMargin: dp(10)
      anchors.leftMargin: dp(10)
      anchors.rightMargin: dp(10)

      //--------------------------------------------
      // Name
      //--------------------------------------------
      Felgo.AppText  { text: qsTr("Name") }
      Felgo.AppTextField
      {
         id: nameEdit
         text: ""
         Layout.columnSpan: 2
         Layout.fillWidth: true
         borderWidth: dp(1)
         placeholderText: "Name"
      }

      //--------------------------------------------
      // Network
      //--------------------------------------------
      Felgo.AppText { text: qsTr("Network") }
      Felgo.AppTextField
      {
         id: networkEdit
         text: ""
         width: parent.width
         borderWidth: dp(1)
         Layout.fillWidth: true
         //text: (root.type === CppInterface.Network) ? root.network : ""
         enabled: root.type === "Network"
      }
      Felgo.AppRadio
      {
         id: networkRadio
         checked: root.type === "Network"
         onClicked:
         {
            root.type = "Network"
            root.order = ""
         }
         //checked: root.type === CppInterface.Network
      }

      //--------------------------------------------
      // Order
      //--------------------------------------------
      Felgo.AppText { text: qsTr("Order") }
      Felgo.AppTextField
      {
         id: orderEdit
         text: ""
         borderWidth: dp(1)
         Layout.fillWidth: true
         enabled: root.type === "Order"
      }
      Felgo.AppRadio
      {
         id: orderRadio
         checked: root.type === "Order"
         onClicked:
         {
            root.type = "Order"
            root.network = ""
         }
         //checked: root.type === CppInterface.Order
      }

      //--------------------------------------------
      // Activity
      //--------------------------------------------
      Felgo.AppText { text: qsTr("Activity") }
      Felgo.AppTextField
      {
         id: activityEdit
         text: ""
         borderWidth: dp(1)
         Layout.fillWidth: true
         Layout.columnSpan: 2
      }
   }

   function clearData()
   {
      root.name = ""
      root.type = "Network"
      root.network = ""
      root.order = ""
      root.activity = ""
   }
}
