#ifndef __CT_UTILS_LOG_H__
#define __CT_UTILS_LOG_H__

enum __e_debug_level {
	LOG_LEVEL_ERROR = 0,
	LOG_LEVEL_INFO,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_ALL,
};

#ifndef pr_fmt
#define pr_fmt(fmt)	fmt
#endif

#define pr_debug(s, ...)	pr_log(" d "pr_fmt(s), ##__VA_ARGS__)
#define pr_info(s, ...)		pr_log(" i "pr_fmt(s), ##__VA_ARGS__)
#define pr_err(s, ...)		pr_log(" e "pr_fmt(s), ##__VA_ARGS__)

extern int pr_init(const char *path);
extern int pr_log(const char *s, ...);
int pr_s2hex(const char *s, ...);

#endif
