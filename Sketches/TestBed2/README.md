TestBed2
========

This project is a *multi-target* test-bed, each *target* focusing on a particular aspect.

- On iOS, the [XCode project](ios/TestBed2.xcodeproj) can produce two distinct apps.  
- FIXME: on OSX and Android, both targets are currently treated via a single app.

### Target 1
- Demonstrates how to work with sound using the [SoundManager class](https://github.com/arielm/new-chronotext-toolkit/blob/develop/src/chronotext/sound/SoundManager.h)
- Requires the [FMOD block](https://github.com/arielm/FMOD)

### Target 2
- Demonstrates how to work with the network using the [HttpTask class](../../Common/Tasks/HttpTask.h)
- Requires the [POCO block](https://github.com/arielm/POCO)

---

### iOS / Command-line operations

**Prerequisites**
- OSX 10.9.5 (other versions may work)
- XCode 6.1.1
- [Command Line Tools (OSX 10.9) for XCode 6.1.1](https://developer.apple.com/download)

The following instructions are for *Target 1* (they can be easily extrapolated for *Target 2*.)

**Building and deploying** 
```
cd Sketches/TestBed2/ios
xcrun xcodebuild -project TestBed2.xcodeproj -target Target1 -configuration Release
```
This should produce an `.ipa` file signed with the *default identity* (automatically picked by XCode.) If you have several identities installed, you can force one of them to be used, as follows:

```
xcrun xcodebuild -project TestBed2.xcodeproj -target Target1 -configuration Release CODE_SIGN_IDENTITY="iPhone Developer: Ariel Malka (**********)"
```

Then, the [ios-deploy](https://github.com/phonegap/ios-deploy) executable can be used to transfer the app to a connected device:
```
./ios-deploy --bundle "build/Release-iPhoneos/TestBed2-Target1.app"
```

- This will not launch the app.
- `ios-deploy` is allowing several additional operations.
- TODO: compile a more recent-version and (re)test automatic-launch and debugging.
