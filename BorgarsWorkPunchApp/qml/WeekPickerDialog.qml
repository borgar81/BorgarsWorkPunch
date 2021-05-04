import QtQuick 2.0
import Felgo 3.0 as Felgo
import QtQuick.Controls 2.12 as QtQuickControls
import com.borgarsoft.BorgarsWorkPunch 1.0

Felgo.Dialog
{
   id: root
   property int weekNumber: -1
   property date startDate
   property date endDate


   title: qsTr("Select week")
   positiveActionLabel: qsTr("OK")
   negativeActionLabel: qsTr("Cancel")
   onAccepted:
   {
      root.weekNumber = weekModel.getWeekNumber(weekTumbler.currentIndex);
      root.startDate = weekModel.getStartDate(weekTumbler.currentIndex);
      root.endDate = weekModel.getEndDate(weekTumbler.currentIndex);

      close()
   }
   onCanceled: close()

   Component.onCompleted:
   {
      refreshWeeks();
   }

   QtQuickControls.Tumbler
   {
      id: weekTumbler
      model: WeekDatesModel
      {
         id: weekModel
      }
      //visibleItemCount: 5
      anchors.left: parent.left
      anchors.right: parent.right
      //currentIndex: 12
   }

   function refreshWeeks()
   {
      weekModel.refreshWeekData();

      var tmpWeekNumber = root.weekNumber;
      if(tmpWeekNumber === -1)
      {
         tmpWeekNumber = cppInterface.getCurrentWeekNumber();
      }

      weekTumbler.currentIndex = weekModel.getIndexOfWeekNumber(tmpWeekNumber);
   }
}
