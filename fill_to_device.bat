@echo off
call ndk-build
call ant debug -Dsdk.dir=%ANDROID_HOME%
call adb install -r bin/FlappySomething-debug.apk
