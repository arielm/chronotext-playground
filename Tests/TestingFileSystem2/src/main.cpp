#include "Platform.h"

#include <iostream>
#include <vector>

#include "boost/filesystem/fstream.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG

void* _malloc(size_t size);
void* _realloc(void *ptr, size_t size);
void _free(void *ptr);

#define STBI_MALLOC(sz)    _malloc(sz)
#define STBI_REALLOC(p,sz) _realloc(p,sz)
#define STBI_FREE(p)       _free(p)

#include "stb_image.h"

using namespace std;

void* _malloc(size_t size)
{
  cout << "malloc " << size << endl;
  return malloc(size);
}

void* _realloc(void *ptr, size_t size)
{
  cout << "realloc " << size << endl;
  return realloc(ptr, size);
}

void _free(void *ptr)
{
  cout << "free" << endl;
  free(ptr);
}

#if defined (CHR_PLATFORM_ANDROID)
  #include <jni.h>
  #include <android/log.h>
#endif

int main(int argc, char *argv[])
{
  auto executablePath = chr::getExecutablePath(argc, argv);

  auto filePath1 = chr::getResourcePath(executablePath, "credits.txt");
  fs::ifstream in1(filePath1, ios::in | ios::binary | ios::ate);
  
  if (in1)
  {
    auto fileSize = in1.tellg();
    in1.seekg(0, ios::beg);

    string result(fileSize, 0);
    in1.read(&result[0], fileSize);

    cout << "[" << result << "]" << endl;
  }
  else
  {
    cout << "FILE-NOT-FOUND: " << filePath1 << endl;
  }

  // ---

  string fileName2 = chr::getResourcePath(executablePath, "2008.547.1crop_4.jpg").string();
  int x, y, comp;

  stbi_uc *data = stbi_load(fileName2.data(), &x, &y, &comp, 0);

  if (data)
  {
    cout << x << "x" << y << " (" << comp << ")" << endl;
    stbi_image_free(data);
  }
  else
  {
    cout << "ERROR WITH: " << fileName2 << endl;
  }

#if defined(CHR_PLATFORM_MINGW)
  cout << chr::checkResource(128) << endl;
  cout << chr::checkResource(129) << endl;
#endif

  return 0;
}

#if defined (CHR_PLATFORM_ANDROID)
  extern "C"
  {
    jint JNI_OnLoad(JavaVM *vm, void *reserved);

    JNIEXPORT void JNICALL performInit(JNIEnv *, jobject, jobject);
    JNIEXPORT jint JNICALL performTest(JNIEnv *, jobject, jobjectArray);
  }

  jobject gActivity = nullptr;
  string gInternalDataPath;

  static const JNINativeMethod mainActivityMethods[] =
  {
    {"performInit", "(Landroid/app/Activity;)V", (void*)performInit},
    {"performTest", "([Ljava/lang/String;)I", (void*)performTest},
  };

  jint JNI_OnLoad(JavaVM *vm, void *reserved)
  {
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK)
    {
        return -1;
    }

    jclass activityClass = env->FindClass("org/chronotext/TestingFileSystem2/MainActivity"); // XXX
    if (!activityClass)
    {
      return -1;
    }

    env->RegisterNatives(activityClass, mainActivityMethods, sizeof(mainActivityMethods) / sizeof(mainActivityMethods[0]));

    __android_log_print(ANDROID_LOG_INFO, "main.cpp", "***** JNI_OnLoad() *****");
    return JNI_VERSION_1_6;
  }

  void JNICALL performInit(JNIEnv *env, jobject obj, jobject activity)
  {
    gActivity = env->NewGlobalRef(activity);

    jmethodID getFilesDirMethod = env->GetMethodID(env->GetObjectClass(activity), "getFilesDir", "()Ljava/io/File;");
    jobject filesDirObject = env->CallObjectMethod(activity, getFilesDirMethod);
    jmethodID getAbsolutePathMethod = env->GetMethodID(env->GetObjectClass(filesDirObject), "getAbsolutePath", "()Ljava/lang/String;");
    jstring internalDataPath = (jstring)env->CallObjectMethod(filesDirObject, getAbsolutePathMethod);

    const char *chars = env->GetStringUTFChars(internalDataPath, nullptr);
    assert(chars);

    gInternalDataPath = chars;
    env->ReleaseStringUTFChars(internalDataPath, chars);
  }

  jint JNICALL performTest(JNIEnv *env, jobject obj, jobjectArray args)
  {
    auto count = env->GetArrayLength(args);
    vector<string> tmp;

    for (auto i = 0; i < count; i++)
    {
      jstring arg = (jstring)env->GetObjectArrayElement(args, i);
      const char *chars = env->GetStringUTFChars(arg, nullptr);
      tmp.push_back(chars);
      env->ReleaseStringUTFChars(arg, chars);
    }

    int argc = count + 1;
    char **argv = new char*[argc];

    argv[0] = const_cast<char*>(gInternalDataPath.data());

    for (auto i = 0; i < count; i++)
    {
      argv[i + 1] = const_cast<char*>(tmp[i].data());
    }

    int result = main(argc, argv); // TODO: TEST
    delete[] argv;

    return result;
  }
#endif
