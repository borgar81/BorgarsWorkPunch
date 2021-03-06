#include <QGuiApplication>
#include <QWindow>

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <MessageUI/MessageUI.h>

#include "IOSInterface.h"

@interface MailerDelegate : NSObject <MFMailComposeViewControllerDelegate, UINavigationControllerDelegate> {
                                IOSInterface *m_mailer;
}
@end

@implementation MailerDelegate

- (id) initWithObject:(IOSInterface *)mailer
{
    self = [super init];
    if (self) {
        m_mailer = mailer;
    }
    return self;
}

- (void)mailComposeController:(MFMailComposeViewController*)controller didFinishWithResult:(MFMailComposeResult)result error:(NSError*)error
{
    //DEBUG;
    Q_UNUSED(controller)
    Q_UNUSED(error)
    switch (result) {
    case MFMailComposeResultCancelled:
        m_mailer->mailCancelled();
        break;
    case MFMailComposeResultSaved:
        m_mailer->mailSaved();
        break;
    case MFMailComposeResultSent:
        m_mailer->mailSent();
        break;
    case MFMailComposeResultFailed:
        m_mailer->mailFailed();
        break;
    default:
        m_mailer->mailCancelled();
        break;
    }
    [controller dismissViewControllerAnimated:YES completion:nil];
}

@end

QT_BEGIN_NAMESPACE

IOSInterface::IOSInterface(QObject *parent)
   :QObject(parent),
     m_delegate([[MailerDelegate alloc] initWithObject:this])

{
}

IOSInterface::~IOSInterface()
{
}

QString IOSInterface::getMacAddress()
{
    NSString* mac_address = @"00-00-00-00-00-00-00-01";
    if ([[UIDevice currentDevice] respondsToSelector:@selector(identifierForVendor)]) {
        mac_address = [[[UIDevice currentDevice] identifierForVendor] UUIDString];
    }
    return QString::fromNSString(mac_address);
}



/**
 * @brief Mailer::open
 * @param subject
 * @param recipients
 * @param body
 */
void IOSInterface::openNewMailMessage(const QString &subject, const QList<QString> &recipientsList, const QString &body, const QByteArray &attatchmentData, const QString &attatchmentFileName)
{
   /*
    UIView *view = static_cast<UIView *>(
                QGuiApplication::platformNativeInterface()
                ->nativeResourceForWindow("uiview", QGuiApplication::focusWindow()));
    UIViewController *qtController = [[view window] rootViewController];
    MFMailComposeViewController *mailer = [[[MFMailComposeViewController alloc] init] autorelease];
    [mailer setMailComposeDelegate: id(m_delegate)];
    [mailer setSubject:subject.toNSString()];
    NSMutableArray *toRecipients = [[NSMutableArray alloc] init];
    for(int i = 0; i < recipients.length(); i++)
    {
        [toRecipients addObject:recipients.at(i).toNSString()];
    }
    [mailer setToRecipients:toRecipients];
    NSString *emailBody = body.toNSString();
    [mailer setMessageBody:emailBody isHTML:NO];
    [qtController presentViewController:mailer animated:YES completion:nil];
    */

   UIViewController *qtController = [UIApplication sharedApplication].keyWindow.rootViewController;
   MFMailComposeViewController *mailer = [[[MFMailComposeViewController alloc] init] autorelease];
   [mailer setMailComposeDelegate: id(m_delegate)];
   [mailer setSubject:subject.toNSString()];
   //mail.addAttachmentData(fileData as Data, mimeType: "text/txt", fileName: "application.log")
   NSMutableArray *toRecipients = [[NSMutableArray alloc] init];
   for(int i = 0; i < recipientsList.length(); i++)
   {
       [toRecipients addObject:recipientsList.at(i).toNSString()];
   }
   [mailer setToRecipients:toRecipients];
   NSString *emailBody = body.toNSString();
   [mailer setMessageBody:emailBody isHTML:NO];

   NSData *attatchmentDataNS = attatchmentData.toNSData();
   NSString *attatchmentFileNameNS = attatchmentFileName.toNSString();
   [mailer addAttachmentData:attatchmentDataNS mimeType: @"text/txt" fileName: attatchmentFileNameNS];
   [qtController presentViewController:mailer animated:YES completion:nil];
}

QT_END_NAMESPACE
