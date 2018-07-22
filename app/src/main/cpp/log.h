#pragma once
#include <android/log.h>

#define APP_NAME "Pipyakas"
#define LOGD(...) __android_log_print(ANDROID_LOG_INFO, APP_NAME, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, APP_NAME, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, APP_NAME, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_WARN, APP_NAME, __VA_ARGS__)
