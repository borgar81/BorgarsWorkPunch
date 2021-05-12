//MyObject.h
#import "IOSInterface.h"
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <MessageUI/MessageUI.h>

@interface MyObject : UIViewController  <MFMailComposeViewControllerDelegate>
{

}

- (void)showEmailPopup:(char *) aCStr;

@end
