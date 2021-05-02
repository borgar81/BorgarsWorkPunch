import QtQuick 2.0
import Felgo 3.0 as Felgo
import com.borgarsoft.BorgarsWorkPunch 1.0

Felgo.ListPage
{
   title: qsTr("Projects")

   /*Felgo.AppText
   {
      text: "Edit Projects"
      anchors.centerIn: parent
   }*/

   onAppeared:
   {
      sqlInterface.fetchProjectList();
   }

   rightBarItem: Felgo.IconButtonBarItem
   {
     // Use one of the pre-defined icons, in this case the 'plus' icon.
      icon: Felgo.IconType.plus
      onClicked:
      {
         navigationStack.push(subPage)
         navigationStack.currentPage.clearData();
         navigationStack.currentPage.isCreateProject = true
         navigationStack.currentPage.title = qsTr("New Project")
      }
   }

   Felgo.AppListView
   {
      id: projectListView

      delegate: Felgo.SimpleRow
      {
         text: model.Name
         detailText: (model.Type === ProjectTypes.Network ? "Network: " + model.NetworkOrOrder : "Order: " + model.NetworkOrOrder) + " " + "Activity: " + model.Activity
         onSelected:
         {
            navigationStack.push(subPage)
            navigationStack.currentPage.projectID = model.Id
            navigationStack.currentPage.type = model.Type
            navigationStack.currentPage.isCreateProject = false
            navigationStack.currentPage.name = model.Name
            if (model.Type === ProjectTypes.Network)
            {
               navigationStack.currentPage.network = model.NetworkOrOrder
            }
            else
            {
               navigationStack.currentPage.order = model.NetworkOrOrder
            }
            navigationStack.currentPage.activity = model.Activity
         }
      }

      model: Felgo.JsonListModel
      {
         keyField: "Id"
         fields: ["Id", "Name", "Type", "NetworkOrOrder", "Activity"]
         source: sqlInterface.projectList
      }
   }


   Component
   {
      id: subPage
      EditSingleProjectPage
      {
      }
   }
}
