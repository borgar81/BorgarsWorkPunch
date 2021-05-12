//MyObject.mm
#import "MyObject.h"

@implementation MyObject


IOSInterface::IOSInterface( void ) : self(NULL){
    init();
}

IOSInterface::~IOSInterface( void ){
    [(id)self dealloc];
}

void IOSInterface::init( void ){
    self = [ [MyObject alloc] init];
}


void IOSInterface::showEmail(char *aCstr){
    NSLog(@"logMyMessage");
    [(id)self showEmailPopup:aCstr];
}


- (void) showEmailPopup:(char *)aCStr{
    NSLog(@"LogMyMessage in Obj");
    NSLog([NSString stringWithUTF8String:aCStr]);

    //Email Subject
    NSString *emailTitle = @"Test Email";
    
    //Email Content
    NSString *messageBody = @"ios programming is so fun!";
    
    //To Address
    NSArray *toRecipents = [NSArray arrayWithObject:@"support@MyCompany.com"];
    
    MFMailComposeViewController *mc = [[MFMailComposeViewController alloc] init];
    mc.mailComposeDelegate = self;
    
    [mc setSubject:emailTitle];
    [mc setMessageBody:messageBody isHTML:false];
    [mc setToRecipients:toRecipents];
    
    UIViewController * topController = [UIApplication sharedApplication].keyWindow.rootViewController;
    while (topController.presentedViewController){
        topController = topController.presentedViewController;
    }
    
    [topController presentViewController:mc animated:YES completion:NULL];
}


- (void) mailComposeController:(MFMailComposeViewController *)controller didFinishWithResult:(MFMailComposeResult)result error:(nullable NSError *)error{
    switch (result) {
        case MFMailComposeResultCancelled:
            NSLog(@"Mail cancelled");
            break;
        case MFMailComposeResultSaved:
            NSLog(@"Mail saved");break;
        case MFMailComposeResultSent:
            NSLog(@"Mail sent");break;
        case MFMailComposeResultFailed:
            NSLog(@"Mail sent failure: %@", [error localizedDescription]);
            break;
        default:
            break;
    }
    
    [controller dismissViewControllerAnimated:YES completion:NULL];
}

@end
