#ifndef __CT_UTILS_LOG_H__
#define __CT_UTILS_LOG_H__

#include <stdio.h>

#ifdef ANDROID
#include <android/log.h>
#endif

enum __e_debug_level {
	LOG_LEVEL_ERROR = 0,
	LOG_LEVEL_WARN,
	LOG_LEVEL_INFO,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_ALL,
};

#ifndef pr_fmt
#define pr_fmt(fmt)	fmt
#endif

#ifdef DEBUG
#ifdef ANDROID
#define pr_debug(s, ...)	pr_log(ANDROID_LOG_DEBUG, pr_fmt(NULL), s, ##__VA_ARGS__)
#else
#define pr_debug(s, ...)	pr_log(LOG_LEVEL_DEBUG, NULL, " d "pr_fmt(s), ##__VA_ARGS__)
#endif
#else
#define pr_debug(s, ...)
#endif

#ifdef ANDROID
#define pr_info(s, ...)		pr_log(ANDROID_LOG_INFO, pr_fmt(NULL), s, ##__VA_ARGS__)
#define pr_warn(s, ...)		pr_log(ANDROID_LOG_WARN, pr_fmt(NULL), s, ##__VA_ARGS__)
#define pr_err(s, ...)		pr_log(ANDROID_LOG_ERROR, pr_fmt(NULL), s, ##__VA_ARGS__)
#else
#define pr_info(s, ...)		pr_log(LOG_LEVEL_INFO, NULL, " i "pr_fmt(s), ##__VA_ARGS__)
#define pr_warn(s, ...)		pr_log(LOG_LEVEL_WARN, NULL, " i "pr_fmt(s), ##__VA_ARGS__)
#define pr_err(s, ...)		pr_log(LOG_LEVEL_ERROR, NULL, " e "pr_fmt(s), ##__VA_ARGS__)
#endif

extern int pr_init(const char *path);
extern int pr_log(int prio, const char * tag, const char *s, ...);
int pr_s2hex(const char *s, ...);

#endif
