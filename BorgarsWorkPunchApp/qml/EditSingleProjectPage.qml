import QtQuick 2.12
import QtQuick.Layouts 1.12
import Felgo 3.0 as Felgo
import com.borgarsoft.BorgarsWorkPunch 1.0


Felgo.Page
{
   id: root


   property string projectID: ""
   property bool isCreateProject : true

   property alias aaType : aaTypeEdit.text
   property alias name : nameEdit.text
   property int type: ProjectTypes.Network
   property alias network : networkEdit.text
   property alias order : orderEdit.text
   property alias activity : activityEdit.text
   property alias showOnHomePage : showOnHomePageSwitch.checked


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

               sqlInterface.addNewProject(nameEdit.text, networkEdit.text, getActivityAsInt(), ProjectTypes.Network, showOnHomePageSwitch.checked, aaTypeEdit.text)
            }
            else
            {
               sqlInterface.addNewProject(nameEdit.text, orderEdit.text, getActivityAsInt(), ProjectTypes.Order, showOnHomePageSwitch.checked, aaTypeEdit.text)
            }
         }
         //-----------------------
         // Edit Project
         //-----------------------
         else
         {
            if(networkRadio.checked)
            {
               sqlInterface.updateProject(root.projectID, nameEdit.text, networkEdit.text, getActivityAsInt(), ProjectTypes.Network, showOnHomePageSwitch.checked, aaTypeEdit.text)
            }
            else
            {
               sqlInterface.updateProject(root.projectID, nameEdit.text, orderEdit.text, getActivityAsInt(), ProjectTypes.Order, showOnHomePageSwitch.checked, aaTypeEdit.text)
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
      Felgo.AppText  { text: qsTr("AA_Type") }
      Felgo.AppTextField
      {
         id: aaTypeEdit
         text: ""
         Layout.columnSpan: 2
         Layout.fillWidth: true
         borderWidth: dp(1)
         placeholderText: "AA_Type"
      }

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
         enabled: root.type === ProjectTypes.Network
      }
      Felgo.AppRadio
      {
         id: networkRadio
         checked: root.type === ProjectTypes.Network
         onClicked:
         {
            root.type = ProjectTypes.Network
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
         enabled: root.type === ProjectTypes.Order
      }
      Felgo.AppRadio
      {
         id: orderRadio
         checked: root.type === ProjectTypes.Order
         onClicked:
         {
            root.type = ProjectTypes.Order
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

      //--------------------------------------------
      // Activity
      //--------------------------------------------
      Felgo.AppText
      {
         text: qsTr("Show on home page")
         Layout.columnSpan: 2
      }
      Felgo.AppSwitch
      {
         id: showOnHomePageSwitch
         checked: true
         //Layout.fillWidth: true
         //Layout.columnSpan: 2
      }
   }

   function clearData()
   {
      root.aaType = ""
      root.name = ""
      root.type = ProjectTypes.Network
      root.network = ""
      root.order = ""
      root.activity = ""
   }

   function getActivityAsInt()
   {
      var activity = -1;
      if (activityEdit.text.length > 0)
      {
         activity = parseInt(activityEdit.text);
      }

      return activity;
   }

}
