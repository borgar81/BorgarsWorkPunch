#pragma once

#include <QNetworkRequest>

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
