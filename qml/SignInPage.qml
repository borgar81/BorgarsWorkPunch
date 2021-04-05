import Felgo 3.0 as Felgo
import QtQuick 2.12
import QtQuick.Layouts 1.12

Felgo.Page
{
   title: "Login"

   Felgo.AppText
   {
      text: "Borgar's WorkPunch"
      anchors.top: parent.top
      anchors.topMargin: dp(10)
      anchors.horizontalCenter: parent.horizontalCenter
   }


   Column
   {
      id: column
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.leftMargin: dp(10)
      anchors.rightMargin: dp(10)
      spacing: dp(16)
      anchors.verticalCenter: parent.verticalCenter
      //anchors.centerIn: parent

      Felgo.AppTextField
      {
         id: emailTextEdit
         text: "Borgar.Ovsthus@technipfmc.com"
         width: parent.width
         borderWidth: dp(1)
         placeholderText: "Email Address"
         inputMode: inputModeEmail
      }
      Felgo.AppTextField
      {
         id: passwordTextEdit
         text: "MyPassword"
         width: parent.width
         placeholderText: "Password"
         borderWidth: dp(1)
         inputMode: inputModePassword
      }
   }

   Felgo.AppButton
   {
      id: signInButton
      enabled: (emailTextEdit.text.length > 0) && (passwordTextEdit.text.length > 0)
      anchors.top: column.bottom
      anchors.horizontalCenter: column.horizontalCenter
      text: "Sign in"
      flat: false
      onClicked:
      {
         authHandler.signUserIn(emailTextEdit.text, passwordTextEdit.text)
      }
   }
}
