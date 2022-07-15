// Carbon includes everything necessary for Accessibilty API
#include <Carbon/Carbon.h>

// Taken from https://github.com/koekeishiya/skhd/blob/master/src/skhd.c#L328
static bool hasAccessibilityAPIAuthorization ()
{
    bool result;
    const void *keys[] = { kAXTrustedCheckOptionPrompt };
    const void *values[] = { kCFBooleanTrue };

    CFDictionaryRef options;
    options = CFDictionaryCreate(kCFAllocatorDefault,
                                 keys, values, sizeof(keys) / sizeof(*keys),
                                 &kCFCopyStringDictionaryKeyCallBacks,
                                 &kCFTypeDictionaryValueCallBacks);

    result = AXIsProcessTrustedWithOptions(options);
    CFRelease(options);

    return result;
}

int main (int argc, char ** argv) {
    pid_t pid;
    AXUIElementRef appRef;
    AXError err;

    if (argc < 2) {
        fprintf(stderr, "Pass a pid to enable eletron accessibility mode\n");
        return 1;
    }

    if (!hasAccessibilityAPIAuthorization()) {
        fprintf(stderr, "Do not have permissions to use the accessibility API!\n");
        return 1;
    }

    // Get the pid from the first arg
    pid = (pid_t) strtoll(argv[1], NULL, 0);

    // Get an accessibility reference to the app 
    appRef = AXUIElementCreateApplication(pid);
    if (appRef == NULL) {
        fprintf(stderr, "Could not find accessibility UI Element for %i!\n", pid);
        return 1;
    }

    // Turn on the manual accessibility mode
    err = AXUIElementSetAttributeValue(appRef, CFSTR("AXManualAccessibility"), kCFBooleanTrue);

    // Something went wrong...
    if (err == kAXErrorAttributeUnsupported) {
        // The manual accessibility attribute didn't work (maybe old version on electron?), instead try to pretend we are VoiceOver
        // Note this is known to cause issues: "undocumented and buggy parameter ... breaks window positioning and animations":
        // https://github.com/electron/electron/issues/7206
        // Since our goal is to manually control the electron app with accessiblity rather than user input, hopefully it won't break too bad...
        err = AXUIElementSetAttributeValue(appRef, CFSTR("AXEnhancedUserInterface"), kCFBooleanTrue);
    }

    // Release the ref
    CFRelease(appRef);

    // We're still not working
    if (err != 0) {
        fprintf(stderr, "Failed to turn on accessibility mode. Err: %i!\n", err);
        fprintf(stderr, "Try https://developer.apple.com/search/?q=%i for details\n", err);
        return 1;
    }

    // It worked!
    return 0;
}