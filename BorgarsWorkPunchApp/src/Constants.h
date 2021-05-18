#pragma once

#include <QNetworkRequest>
#include <qobjectdefs.h>

namespace MessageTypes
{
   enum MessageTypesEnum
   {
      FetchProjectList = 10,
      FetchCurrentState,
      NewProject,
      UpdateProject,
      PunchIn,
      PunchOut,
      RegisterProjectWork,
      SignUserUp,
      SignUserIn,
      RefreshToken,
      FetchReport
   };

   static const int MessageTypeAttribute = QNetworkRequest::User + 1;
}

namespace WeekDays
{
   enum WeekDaysEnum
   {
      Monday = 1,
      Tuesday,
      Wednesday,
      Thursday,
      Friday,
      Saturday,
      Sunday
   };
}

namespace SAPColumns
{
   enum SAPColumnsEnum
   {
      Send_CCtr = 0,
      ActTyp,
      Rec_CCtr,
      Rec_Order,
      Network,
      Activity,
      AA_Type,

      Monday = 9,
      Monday_From,
      Monday_To,
      Tuesday,
      Tuesday_From,
      Tuesday_To,
      Wednesday,
      Wednesday_From,
      Wednesday_To,
      Thursday,
      Thursday_From,
      Thursday_To,
      Friday,
      Friday_From,
      Friday_To,
      Saturday,
      Saturday_From,
      Saturday_To,
      Sundayday,
      Sundayday_From,
      Sundayday_To
   };
}

namespace ProjectTypes
{
   Q_NAMESPACE
   enum class ProjectTypesEnum
   {
      Network = 0,
      Order
   };
   Q_ENUM_NS(ProjectTypesEnum)
}

namespace WeekReportTypes
{
   Q_NAMESPACE
   enum WeekReportTypesEnum
   {
      Totals = 0,
      PunchIns
   };
   Q_ENUM_NS(WeekReportTypesEnum)
}
