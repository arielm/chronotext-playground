TestBed2
--------

This project is a *multi-target* test-bed, each *target* focusing on a particular aspect.

- On iOS, the [XCode project](ios/TestBed2.xcodeproj) can produce two distinct apps.  
- FIXME: on OSX and Android, both targets are currently treated via a single app.

**Target 1**
- Demonstrates how to work with sound using the [SoundManager class](https://github.com/arielm/new-chronotext-toolkit/blob/develop/src/chronotext/sound/SoundManager.h)
- Requires the [FMOD block](https://github.com/arielm/FMOD)

**Target 2**
- Demonstrates how to work with the network using the [HttpTask class](../../Common/Tasks/HttpTask.h)
- Requires the [POCO block](https://github.com/arielm/POCO)
