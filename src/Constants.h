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
      RefreshToken
   };

   static const int MessageTypeAttribute = QNetworkRequest::User + 1;
}
