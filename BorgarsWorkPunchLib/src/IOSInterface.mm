#include "IOSInterface.h"

IOSInterface::IOSInterface()
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

