#include "Log.h"

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
  LOGI << "malloc " << size << endl;
  return malloc(size);
}

void* _realloc(void *ptr, size_t size)
{
  LOGI << "realloc " << size << endl;
  return realloc(ptr, size);
}

void _free(void *ptr)
{
  LOGI << "free" << endl;
  free(ptr);
}

int main(int argc, const char *argv[])
{
  for (int i = 0; i < argc; i++)
  {
    LOGW << "{" << argv[i] << "}" << endl;
  }

  auto executablePath = chr::getExecutablePath(argc, argv);

  auto filePath1 = chr::getResourcePath(executablePath, "credits.txt");
  fs::ifstream in1(filePath1, ios::in | ios::binary | ios::ate);
  
  if (in1)
  {
    auto fileSize = in1.tellg();
    in1.seekg(0, ios::beg);

    string result(fileSize, 0);
    in1.read(&result[0], fileSize);

    LOGI << "[" << result << "]" << endl;
  }
  else
  {
    LOGI << "FILE-NOT-FOUND: " << filePath1 << endl;
  }

  // ---

  string fileName2 = chr::getResourcePath(executablePath, "2008.547.1crop_4.jpg").string();
  int x, y, comp;

  stbi_uc *data = stbi_load(fileName2.data(), &x, &y, &comp, 0);

  if (data)
  {
    LOGI << x << "x" << y << " (" << comp << ")" << endl;
    stbi_image_free(data);
  }
  else
  {
    LOGI << "ERROR WITH: " << fileName2 << endl;
  }

#if defined(CHR_PLATFORM_MINGW)
  LOGI << chr::checkResource(128) << endl;
  LOGI << chr::checkResource(129) << endl;
#elif defined(CHR_PLATFORM_ANDROID)
  LOGI << chr::checkResource("credits.txt") << endl;
  LOGI << chr::checkResource("2008.547.1crop_4.jpg") << endl;
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

  static const JNINativeMethod mainActivityMethods[] =
  {
    {"performInit", "(Landroid/app/Activity;)V", (void*)performInit},
    {"performTest", "([Ljava/lang/String;)I", (void*)performTest},
  };

  namespace chr
  {
    namespace android
    {
      JavaVM *vm = nullptr;
      jobject activity = nullptr;

      AAssetManager *assetManager = nullptr;
      std::string internalDataPath;
      std::string externalDataPath;
      std::string apkPath;
    }
  }

  jint JNI_OnLoad(JavaVM *vm, void *reserved)
  {
    LOGD << "JNI_Onload" << endl;

    JNIEnv *env;
    jint err = vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);

    if (err != JNI_OK)
    {
      LOGF << "JNI ERROR: " << err << endl;
    }

    chr::android::vm = vm;

    /*
     * TODO: SHOULD TAKE PLACE "ELSEWHERE"
     */
    jclass activityClass = env->FindClass("org/chronotext/TestingFileSystem2/MainActivity");
    env->RegisterNatives(activityClass, mainActivityMethods, sizeof(mainActivityMethods) / sizeof(mainActivityMethods[0]));

    return JNI_VERSION_1_6;
  }

  void JNICALL performInit(JNIEnv *env, jobject obj, jobject activity)
  {
    chr::android::activity = env->NewGlobalRef(activity);

    jmethodID getAssetsMethod = env->GetMethodID(env->GetObjectClass(activity), "getAssets", "()Landroid/content/res/AssetManager;");
    chr::android::assetManager = AAssetManager_fromJava(env, env->CallObjectMethod(activity, getAssetsMethod));

    jmethodID getFilesDirMethod = env->GetMethodID(env->GetObjectClass(activity), "getFilesDir", "()Ljava/io/File;");
    jobject filesDirObject = env->CallObjectMethod(activity, getFilesDirMethod);
    jmethodID getAbsolutePathMethod = env->GetMethodID(env->GetObjectClass(filesDirObject), "getAbsolutePath", "()Ljava/lang/String;");
    jstring internalDataPath = (jstring)env->CallObjectMethod(filesDirObject, getAbsolutePathMethod);
    chr::android::internalDataPath = chr::toString(env, internalDataPath);

    jclass environmentClass = env->FindClass("android/os/Environment");
    jmethodID getExternalStorageDirectoryMethod = env->GetStaticMethodID(environmentClass, "getExternalStorageDirectory",  "()Ljava/io/File;");
    jobject externalStorageDirectoryObject = env->CallStaticObjectMethod(environmentClass, getExternalStorageDirectoryMethod);
    jstring externalDataPath = (jstring)env->CallObjectMethod(externalStorageDirectoryObject, getAbsolutePathMethod);
    chr::android::externalDataPath = chr::toString(env, externalDataPath);

    jmethodID getPackageCodePathMethod = env->GetMethodID(env->GetObjectClass(activity), "getPackageCodePath", "()Ljava/lang/String;");
    jstring apkPath = (jstring)env->CallObjectMethod(activity, getPackageCodePathMethod);
    chr::android::apkPath = chr::toString(env, apkPath);
  }

  jint JNICALL performTest(JNIEnv *env, jobject obj, jobjectArray args)
  {
    auto tmp = chr::toStrings(env, args);

    vector<const char*> argv;
    argv.emplace_back(chr::android::apkPath.data());
    
    for (const auto &arg : tmp)
    {
      argv.emplace_back(arg.data());
    }

    return main(argv.size(), argv.data());
  }
#endif
