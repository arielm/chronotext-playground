#pragma once

#include <string>

#include <android/asset_manager_jni.h>

extern "C"
{
  jint JNI_OnLoad(JavaVM *vm, void *reserved);

  /*
   * TODO INSTEAD:
   *
   * void Java_org_chronotext_Bridge_init(JNIEnv *env, jobject obj, jobject activty);
   * void Java_org_chronotext_Bridge_main(JNIEnv *env, jobject obj, jobjectArray args);
   */
   
  JNIEXPORT void JNICALL performInit(JNIEnv *, jobject, jobject);
  JNIEXPORT jint JNICALL performTest(JNIEnv *, jobject, jobjectArray);
}

static const JNINativeMethod mainActivityMethods[] =
{
  {"performInit", "(Landroid/app/Activity;)V", (void*)performInit},
  {"performTest", "([Ljava/lang/String;)I", (void*)performTest},
};

namespace chr
{
  namespace android
  {
    extern JavaVM *vm;
    extern jobject activity;

    extern AAssetManager *assetManager;
    extern std::string internalDataPath;
    extern std::string externalDataPath;
    extern std::string apkPath;
  }
}

extern int main(int argc, const char *argv[]);
