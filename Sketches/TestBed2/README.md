TestBed2
========

This project is a *multi-target* test-bed, each *target* focusing on a particular topic.

- On iOS, the [XCode project](ios/TestBed2.xcodeproj) can produce two distinct apps.  
- FIXME: on OSX and Android, both targets are currently treated via a single app.

### Target 1
- Demonstrates how to work with sound using the [SoundManager class](https://github.com/arielm/new-chronotext-toolkit/blob/develop/src/chronotext/sound/SoundManager.h)
- Requires the [FMOD block](https://github.com/arielm/FMOD)

### Target 2
- Demonstrates how to work with the network using the [HttpTask class](../../Common/Tasks/HttpTask.h)
- Requires the [POCO block](https://github.com/arielm/POCO)

Check the comments at the top of [Sketch.h](src/Sketch.h) for more details regarding the dependencies.

---

### OSX and iOS / XCode

**Prerequisites**
- OSX 10.9.5 (other versions may work)
- XCode 6.1.1
- An entry named `CINDER_PATH_CHR` pointing to the Cinder folder must be defined in *XCode/Preferences/Locations/Source Trees*.

---

### Android / command-line

The instructions can be extrapolated from this [tutorial](https://github.com/arielm/new-chronotext-toolkit/wiki/How-to-run-a-sample-project-on-Android).

---

### iOS / command-line

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
This should produce an `.app` bundle signed with the *default identity* (automatically picked by XCode.) If you have several identities installed, you can force one of them to be used, as follows:

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

**Archiving and packaging**

In the past, this procedure has been successfully used for *ad-hoc* distribution with *test-flight*. The latter was in the meantime acquired by Apple, and the rules may have changed. (TODO: test the new *test-flight* as well as alternatives...)

```
cd Sketches/TestBed2/ios
xcodebuild -scheme Target1 archive -archivePath Target1.xcarchive
```
This should produce a `.xcarchive` bundle signed with the *default identity* (automatically picked by XCode.) If you have several identities installed, you can force one of them to be used, as follows:

```
xcodebuild -scheme Target1 archive -archivePath Target1.xcarchive CODE_SIGN_IDENTITY="iPhone Developer: Ariel Malka (**********)"
```

Then, the following should produce an `.ipa` bundle ready for *distribution*:
```
xcodebuild -exportArchive -exportFormat ipa -archivePath Target1.xcarchive -exportPath Target1.ipa -exportProvisioningProfile "Chronotext_AdHoc"
```
Be sure to replace `Chronotext_AdHoc` with the relevant *provisioning-profile* name (which must be properly registered at the XCode's preferences level.)