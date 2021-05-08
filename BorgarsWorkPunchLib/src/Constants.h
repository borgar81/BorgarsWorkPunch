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
