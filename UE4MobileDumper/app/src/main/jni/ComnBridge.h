//
// Created by Ascarre on 01-02-2023.
//

#ifndef PRIVATE_DUMPER_COMNBRIDGE_H
#define PRIVATE_DUMPER_COMNBRIDGE_H

#include <sstream>

/******************** Get System Api Level ********************/
__BEGIN_DECLS
int __system_property_get(const char *__name, char *__value);
int atoi(const char *__s) __attribute_pure__;
int Api_Level() {
    char value[92] = {0};
    if (__system_property_get("ro.build.version.sdk", value) < 1) return -1;
    int api_level = atoi(value);
    return (api_level > 0) ? api_level : -1;
}
__END_DECLS

/******************** Toast Function ********************/
void MakeToast(JNIEnv *env, jobject thiz, const char *text, int length) {
    jstring ToastText = env->NewStringUTF(text);
    jclass ToastClass = env->FindClass("android/widget/Toast");
    jmethodID ToastMethod = env->GetStaticMethodID(ToastClass, "makeText","(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;");
    jobject ToastObject = env->CallStaticObjectMethod(ToastClass, ToastMethod, thiz, ToastText,length);
    jmethodID ToastMethodShow = env->GetMethodID(ToastClass, "show", "()V");
    env->CallVoidMethod(ToastObject, ToastMethodShow);
}

/******************** Start Service Function ********************/
void startService(JNIEnv *env, jobject ctx) {
    jclass native_context = env->GetObjectClass(ctx);
    jclass intentClass = env->FindClass("android/content/Intent");
    jclass actionString = env->FindClass("ashope/tech/dumper/Floater");
    jmethodID newIntent = env->GetMethodID(intentClass, "<init>","(Landroid/content/Context;Ljava/lang/Class;)V");
    jobject intent = env->NewObject(intentClass, newIntent, ctx, actionString);
    jmethodID startActivityMethodId = env->GetMethodID(native_context, "startService","(Landroid/content/Intent;)Landroid/content/ComponentName;");
    env->CallObjectMethod(ctx, startActivityMethodId, intent);
}

/******************** Open Settings for Overlay Function ********************/
void startActivityPermisson(JNIEnv *env, jobject ctx) {
    jclass native_context = env->GetObjectClass(ctx);
    jmethodID startActivity = env->GetMethodID(native_context, "startActivity","(Landroid/content/Intent;)V");

    jmethodID pack = env->GetMethodID(native_context, "getPackageName", "()Ljava/lang/String;");
    jstring packageName = static_cast<jstring>(env->CallObjectMethod(ctx, pack));

    const char *pkg = env->GetStringUTFChars(packageName, 0);

    std::stringstream pkgg;
    pkgg << "package:";
    pkgg << pkg;
    std::string pakg = pkgg.str();

    jclass Uri = env->FindClass("android/net/Uri");
    jmethodID Parce = env->GetStaticMethodID(Uri, "parse", "(Ljava/lang/String;)Landroid/net/Uri;");
    jobject UriMethod = env->CallStaticObjectMethod(Uri, Parce, env->NewStringUTF(pakg.c_str()));

    jclass intentclass = env->FindClass("android/content/Intent");
    jmethodID newIntent = env->GetMethodID(intentclass, "<init>","(Ljava/lang/String;Landroid/net/Uri;)V");
    jobject intent = env->NewObject(intentclass, newIntent, env->NewStringUTF("android.settings.action.MANAGE_OVERLAY_PERMISSION"), UriMethod);

    env->CallVoidMethod(ctx, startActivity, intent);
}

/******************** Check Overlay Permission Function ********************/
void CheckOverlayPermission(JNIEnv *env, jclass thiz, jobject ctx) {
    int sdkVer = Api_Level();
    if (sdkVer >= 23) {
        jclass Settings = env->FindClass("android/provider/Settings");
        jmethodID canDraw = env->GetStaticMethodID(Settings, "canDrawOverlays","(Landroid/content/Context;)Z");
        if (!env->CallStaticBooleanMethod(Settings, canDraw, ctx)) {
            MakeToast(env, ctx, "Please Give Overlay Permission to start Mod Menu", 1);
            startActivityPermisson(env, ctx);

            sleep(5);
            exit(0);
        }
    }
    sleep(5);
    MakeToast(env, ctx, "Made By Ascarre", 1);
    MakeToast(env, ctx, "Made By Ascarre", 1);
    startService(env, ctx);
};

/******************** Get PackageName of Apk ********************/
void GetPackageName(JNIEnv *env, jclass thiz, jobject ctx) {
    jclass contextClass = env->FindClass("android/content/Context");
    jmethodID getPackageNameMethod = env->GetMethodID(contextClass, "getPackageName", "()Ljava/lang/String;");
    Offsets::ProcessName = env->GetStringUTFChars((jstring) env->CallObjectMethod(ctx, getPackageNameMethod), 0);
}

#endif //PRIVATE_DUMPER_COMNBRIDGE_H
