#include "darwin.hpp"
#include <AppKit/AppKit.h>
#include <string>
#include <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>

void SetClipboard(std::string string) {
  @autoreleasepool {
    NSPasteboard* pasteboard = [NSPasteboard generalPasteboard];
    [pasteboard declareTypes:@[NSPasteboardTypeString] owner:nil];
    [pasteboard setString:[NSString stringWithUTF8String:string.c_str()] forType:NSPasteboardTypeString];
  }
}

std::string GetClipboard() {
  std::string content;
  @autoreleasepool {
    NSPasteboard* pasteboard = [NSPasteboard generalPasteboard];
    NSString* clipboardContent = [pasteboard stringForType:NSPasteboardTypeString];
    content = [clipboardContent UTF8String];
  }
  return content;
}
