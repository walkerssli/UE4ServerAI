/**
 *
 * @file    tlog.h
 * @brief   日志模块
 *
 * @author steve
 * @version 1.0
 * @date 2007-04-05
 *
 *
 * Copyright (c)  2007, 腾讯科技有限公司互动娱乐研发部
 * All rights reserved.
 *
 */

#ifndef TLOG_H
#define TLOG_H
#include "tlog/tlogerr.h"
#include "tlog/tlog_category.h"
#include "tlog/tlog_priority_def.h"
#include "tlog/tlog_bill.h"
#if defined(_WIN32) || defined(_WIN64)

#ifdef TSF4G_SMALL_LIB
#undef _TSF4G_LARGE_LIB_
#endif

#ifdef _TSF4G_LARGE_LIB_
#ifdef _DEBUG
#pragma comment(lib, "libtsf4g_d.lib")
#else
#pragma comment(lib, "libtsf4g.lib")
#endif
#else

#ifdef _DEBUG
#pragma comment(lib, "libtlog_d.lib")
#else
#pragma comment(lib, "libtlog.lib")
#endif
#endif

#ifdef TLOG_NO_WARNING
#pragma warning(disable:4204)
#endif

#endif

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct tagTLogCtx *LPTLOGCTX;

/**
 * 获取日志类
 * @param[in] a_pstCtx 日志上下文
 * @param[in] a_pszName 日志类名
 * @return  NULL 失败
 *			非NULL 获取的日志类指针
 */
LPTLOGCATEGORYINST tlog_get_category(LPTLOGCTX a_pstCtx, const char* a_pszName);

/**
 * 获取账单日志类
 * @param[in] a_pstCtx 日志上下文
 * @param[in] a_pszName 日志类名
 * @return  NULL 失败
 *          非NULL 获取的日志类指针
 */
LPTLOGCATEGORYINST tlog_get_bill_category(LPTLOGCTX a_pstCtx,
        const char *a_pszName);

/**
 * 设置日志上下文的主机名，使用tapp的程序，默认为appname
 * @param[in,out] a_pstCtx 日志上下文
 * @param[in] a_pszHostName 主机名
 *
 * @return  0 成功
 *			非0 失败
 */
int tlog_set_host_name(LPTLOGCTX a_pstCtx, const char *a_pszHostName);

/**
 * 设置日志上下文的模块名，使用tapp的程序，默认为default
 * @param[in,out] a_pstCtx 日志上下文
 * @param[in] a_pszModuleName 模块名
 *
 * @return  0 成功
 *			非0 失败
 */
int tlog_set_module_name(LPTLOGCTX a_pstCtx, const char *a_pszModuleName);

/**
 * 复制日志上下文
 * @param[in] a_pstParentCategoryInst 日志上下文
 * @param[in] a_pszChildName 新的日志空间名
 *
 * @return  0 clone成功
 *          1 clone示例已存在
 *			<0 失败
 * @note
 *		a_pstParentCategoryInst的device必须为非空，否则克隆出来的日志类无法写日志
 */
int tlog_clone_category(LPTLOGCATEGORYINST a_pstParentCategoryInst,
        const char *a_pszChildName);

/**
 * @brief 设置当前时间，需要在TAPP框架中调用
 *
 * @pre a_pstTime不能为NULL
 */
void tlog_set_current_time(const struct timeval *a_pstTime);

/**
 * @brief 返回当前时间戳，单位us
 *
 * @retval >0 -- successful
 * @retval <0 -- failed
 * @pre a_pstTime不能为NULL
 */
int64_t tlog_get_current_time();


/**
 * @breif 根据log句柄获取logctx对象指针
 * @return NULL失败，非NULL成功
 */
LPTLOGCTX tlog_get_context(LPTLOGCATEGORYINST a_pstCategoryInst);


#define tlog_category_is_priority_enabled(cat, priority) \
	( (cat) &&  ( NULL==(cat)->piPriorityLow || TLOG_PRIORITY_NULL==*(cat)->piPriorityLow || (priority)<=*(cat)->piPriorityLow ) \
	&&  ( NULL==(cat)->piPriorityHigh || TLOG_PRIORITY_NULL==*(cat)->piPriorityHigh || (priority)>=*(cat)->piPriorityHigh ))

#if !defined(_WIN32) && !defined(_WIN64)

#define TLOG_FMT(fmt) fmt
#define tlog_log(cat, priority, id, cls, fmt, args...)\
	do{\
		if(tlog_category_is_priority_enabled(cat, priority)&& (tlog_category_can_write(cat, priority,id, cls) ))\
		{\
			TLOGEVENT stEvt;\
			const TLOGLOCINFO locinfo = TLOG_LOC_INFO_INITIALIZER(NULL);\
    			stEvt.evt_priority = priority; \
    			stEvt.evt_id = id; \
    			stEvt.evt_cls  = cls; \
		    	stEvt.evt_is_msg_binary = 0; \
		    	stEvt.evt_loc = &locinfo; \
			tlog_category_logv(cat, &stEvt, fmt, ##args);\
		}\
	}while(0)

#define tlog_fatal(cat, id, cls, fmt, args...)		tlog_log(cat, TLOG_PRIORITY_FATAL, id, cls, fmt, ##args)
#define tlog_alert(cat, id, cls, fmt, args...)		tlog_log(cat, TLOG_PRIORITY_ALERT, id, cls, fmt, ##args)
#define tlog_crit(cat, id, cls, fmt, args...)		tlog_log(cat, TLOG_PRIORITY_CRIT, id, cls, fmt, ##args)
#define tlog_error(cat, id, cls, fmt, args...)		tlog_log(cat, TLOG_PRIORITY_ERROR, id, cls, fmt, ##args)
#define tlog_warn(cat, id, cls, fmt, args...)		tlog_log(cat, TLOG_PRIORITY_WARN, id, cls, fmt, ##args)
#define tlog_notice(cat, id, cls, fmt, args...)		tlog_log(cat, TLOG_PRIORITY_NOTICE, id, cls, fmt, ##args)
#define tlog_info(cat, id, cls, fmt, args...)		tlog_log(cat, TLOG_PRIORITY_INFO, id, cls, fmt, ##args)
#define tlog_debug(cat, id, cls, fmt, args...)		tlog_log(cat, TLOG_PRIORITY_DEBUG, id, cls, fmt, ##args)
#define tlog_trace(cat, id, cls, fmt, args...)		tlog_log(cat, TLOG_PRIORITY_TRACE, id, cls, fmt, ##args)

#define tlog_bill(cat, fmt,args...) tlog_error(cat,0,0,fmt,##args)


// 条件日志，tlog频率控制，辅助定义开始
#define tlog_log_ex(cat, priority, id, cls, fmt, args...) \
    do { \
        TLOGEVENT stEvt; \
        const TLOGLOCINFO locinfo = TLOG_LOC_INFO_INITIALIZER(NULL); \
        stEvt.evt_priority = priority; \
        stEvt.evt_id = id; \
        stEvt.evt_cls  = cls; \
        stEvt.evt_is_msg_binary = 0; \
        stEvt.evt_loc = &locinfo; \
        tlog_category_logv(cat, &stEvt, fmt, ##args); \
    } while (0)

#define LOG_VARNAME(prefix, line) LOG_VARNAME_CONCAT(prefix, line)
#define LOG_VARNAME_CONCAT(prefix, line) prefix ## line

#define LOG_CNT_VAR     LOG_VARNAME(s_log_cnt_, __LINE__)
#define START_TIME_VAR  LOG_VARNAME(s_start_time_ ,__LINE__)
#define NOW_TIME_VAR    LOG_VARNAME(s_now_time_ ,__LINE__)

#define tlog_n_every_second(num, cat, pri, id, cls, fmt, args...) \
    do { \
        if (tlog_category_is_priority_enabled(cat, pri) && (tlog_category_can_write(cat, pri, id, cls))) { \
            static __thread int32_t LOG_CNT_VAR = 0; static __thread int64_t START_TIME_VAR = 0; \
            if (START_TIME_VAR == 0) { START_TIME_VAR = tlog_get_current_time(); } \
            int64_t NOW_TIME_VAR = tlog_get_current_time(); \
            if (START_TIME_VAR + 1000000 < NOW_TIME_VAR) { \
                START_TIME_VAR = NOW_TIME_VAR; \
                if (LOG_CNT_VAR > num) { \
                    tlog_log_ex(cat, TLOG_PRIORITY_INFO, 0, 0, "discard %d logs last cycle", LOG_CNT_VAR - num); \
                } \
                LOG_CNT_VAR = 0; \
            } \
            if (++LOG_CNT_VAR <= num) { \
                tlog_log_ex(cat, pri, id, cls, fmt, ##args); \
            } \
        } \
    } while (0)
// 条件日志，tlog频率控制，辅助定义结束

// 条件日志，tlog频率控制log接口
#define tlog_fatal_n_every_second(num, cat, id, cls, fmt, args...)      tlog_n_every_second(num, cat, TLOG_PRIORITY_FATAL, id, cls, fmt, ##args)
#define tlog_alert_n_every_second(num, cat, id, cls, fmt, args...)      tlog_n_every_second(num, cat, TLOG_PRIORITY_ALERT, id, cls, fmt, ##args)
#define tlog_crit_n_every_second(num, cat, id, cls, fmt, args...)       tlog_n_every_second(num, cat, TLOG_PRIORITY_CRIT, id, cls, fmt, ##args)
#define tlog_error_n_every_second(num, cat, id, cls, fmt, args...)      tlog_n_every_second(num, cat, TLOG_PRIORITY_ERROR, id, cls, fmt, ##args)
#define tlog_warn_n_every_second(num, cat, id, cls, fmt, args...)       tlog_n_every_second(num, cat, TLOG_PRIORITY_WARN, id, cls, fmt, ##args)
#define tlog_notice_n_every_second(num, cat, id, cls, fmt, args...)     tlog_n_every_second(num, cat, TLOG_PRIORITY_NOTICE, id, cls, fmt, ##args)
#define tlog_info_n_every_second(num, cat, id, cls, fmt, args...)       tlog_n_every_second(num, cat, TLOG_PRIORITY_INFO, id, cls, fmt, ##args)
#define tlog_debug_n_every_second(num, cat, id, cls, fmt, args...)      tlog_n_every_second(num, cat, TLOG_PRIORITY_DEBUG, id, cls, fmt, ##args)
#define tlog_trace_n_every_second(num, cat, id, cls, fmt, args...)      tlog_n_every_second(num, cat, TLOG_PRIORITY_TRACE, id, cls, fmt, ##args)

#else
/*Win32*/
#if  _MSC_VER < 1400  /*vc2005*/
//# error "Please use a newer compiler that support __VA_ARGS__."
#ifdef __FUNCTION__
#define TLOG_FMT(fmt) "%s:%d:%s()" fmt,basename(__FILE__),__LINE__,__FUNCTION__
#else
#define TLOG_FMT(fmt) "%s:%d:%s()" fmt,basename(__FILE__),__LINE__,"nil"
#endif
int tlog_log(TLOGCATEGORYINST *a_pstCatInst, int priority, unsigned int id, unsigned int cls,
        const char* a_pszFmt, ...);

int tlog_fatal(TLOGCATEGORYINST *a_pstCatInst, unsigned int id, unsigned int cls,
        const char* a_pszFmt, ...);
int tlog_alert(TLOGCATEGORYINST *a_pstCatInst, unsigned int id, unsigned int cls,
        const char* a_pszFmt, ...);
int tlog_crit(TLOGCATEGORYINST *a_pstCatInst, unsigned int id, unsigned int cls,
        const char* a_pszFmt, ...);
int tlog_error(TLOGCATEGORYINST *a_pstCatInst, unsigned int id, unsigned int cls,
        const char* a_pszFmt, ...);
int tlog_warn(TLOGCATEGORYINST *a_pstCatInst, unsigned int id, unsigned int cls,
        const char* a_pszFmt, ...);
int tlog_notice(TLOGCATEGORYINST *a_pstCatInst, unsigned int id, unsigned int cls,
        const char* a_pszFmt, ...);
int tlog_info(TLOGCATEGORYINST *a_pstCatInst, unsigned int id, unsigned int cls,
        const char* a_pszFmt, ...);
int tlog_debug(TLOGCATEGORYINST *a_pstCatInst, unsigned int id, unsigned int cls,
        const char* a_pszFmt, ...);
int tlog_trace(TLOGCATEGORYINST *a_pstCatInst, unsigned int id, unsigned int cls,
        const char* a_pszFmt, ...);

int tlog_bill(TLOGCATEGORYINST *cat,const char *fmt,...);
#else

#define TLOG_FMT(fmt) fmt

#define tlog_log(cat, priority, id, cls, fmt, ...)\
		do{\
			if(tlog_category_is_priority_enabled(cat, priority) && tlog_category_can_write(cat, priority,id, cls))\
			{\
				TLOGEVENT stEvt;\
				const TLOGLOCINFO locinfo = TLOG_LOC_INFO_INITIALIZER(NULL);\
    				stEvt.evt_priority = priority; \
    				stEvt.evt_id = id; \
    				stEvt.evt_cls  = cls; \
				stEvt.evt_is_msg_binary = 0; \
				stEvt.evt_loc = &locinfo; \
				tlog_category_logv(cat, &stEvt, fmt, __VA_ARGS__);\
			}\
		}while(0)

#define tlog_fatal(cat, id, cls, fmt, ...)		tlog_log(cat, TLOG_PRIORITY_FATAL, id, cls, fmt, __VA_ARGS__)
#define tlog_alert(cat, id, cls, fmt, ...)		tlog_log(cat, TLOG_PRIORITY_ALERT, id, cls, fmt, __VA_ARGS__)
#define tlog_crit(cat, id, cls, fmt, ...)		tlog_log(cat, TLOG_PRIORITY_CRIT, id, cls, fmt, __VA_ARGS__)
#define tlog_error(cat, id, cls, fmt, ...)		tlog_log(cat, TLOG_PRIORITY_ERROR, id, cls, fmt, __VA_ARGS__)
#define tlog_warn(cat, id, cls, fmt, ...)		tlog_log(cat, TLOG_PRIORITY_WARN, id, cls, fmt, __VA_ARGS__)
#define tlog_notice(cat, id, cls, fmt, ...)		tlog_log(cat, TLOG_PRIORITY_NOTICE, id, cls, fmt, __VA_ARGS__)
#define tlog_info(cat, id, cls, fmt, ...)		tlog_log(cat, TLOG_PRIORITY_INFO, id, cls, fmt, __VA_ARGS__)
#define tlog_debug(cat, id, cls, fmt, ...)		tlog_log(cat, TLOG_PRIORITY_DEBUG, id, cls, fmt, __VA_ARGS__)
#define tlog_trace(cat, id, cls, fmt, ...)		tlog_log(cat, TLOG_PRIORITY_TRACE, id, cls, fmt, __VA_ARGS__)

#define tlog_bill(cat, fmt,...) tlog_error(cat,0,0,fmt,__VA_ARGS__)

#endif  /* _MSC_VER < 1400  , vc2005*/

#endif

/*Tlog using valist*/

#define tlog_log_va_list(cat, priority, id, cls, fmt, valist)\
	do{\
		if(tlog_category_is_priority_enabled(cat, priority) && (tlog_category_can_write(cat, priority,id, cls) ))\
		{\
			TLOGEVENT stEvt;\
			const TLOGLOCINFO locinfo = TLOG_LOC_INFO_INITIALIZER(NULL);\
				stEvt.evt_priority = priority; \
				stEvt.evt_id = id; \
				stEvt.evt_cls  = cls; \
			stEvt.evt_is_msg_binary = 0; \
			stEvt.evt_loc = &locinfo; \
			tlog_category_logv_va_list(cat, &stEvt, fmt, valist);\
		}\
	}while(0)

#define tlog_fatal_va_list(cat, id, cls, fmt, valist)		tlog_log_va_list(cat, TLOG_PRIORITY_FATAL, id, cls, fmt, valist)
#define tlog_alert_va_list(cat, id, cls, fmt, valist)		tlog_log_va_list(cat, TLOG_PRIORITY_ALERT, id, cls, fmt, valist)
#define tlog_crit_va_list(cat, id, cls, fmt, valist)			tlog_log_va_list(cat, TLOG_PRIORITY_CRIT, id, cls, fmt, valist)
#define tlog_error_va_list(cat, id, cls, fmt, valist)		tlog_log_va_list(cat, TLOG_PRIORITY_ERROR, id, cls, fmt, valist)
#define tlog_warn_va_list(cat, id, cls, fmt, valist)		tlog_log_va_list(cat, TLOG_PRIORITY_WARN, id, cls, fmt, valist)
#define tlog_notice_va_list(cat, id, cls, fmt, valist)		tlog_log_va_list(cat, TLOG_PRIORITY_NOTICE, id, cls, fmt, valist)
#define tlog_info_va_list(cat, id, cls, fmt, valist)		tlog_log_va_list(cat, TLOG_PRIORITY_INFO, id, cls, fmt, valist)
#define tlog_debug_va_list(cat, id, cls, fmt, valist)		tlog_log_va_list(cat, TLOG_PRIORITY_DEBUG, id, cls, fmt, valist)
#define tlog_trace_va_list(cat, id, cls, fmt, valist)		tlog_log_va_list(cat, TLOG_PRIORITY_TRACE, id, cls, fmt, valist)

/*End*/

#define tlog_log_bin(cat, priority, id, cls, type, version, buff, bufflen)\
	do{\
		if(tlog_category_is_priority_enabled(cat, priority)&& (tlog_category_can_write(cat, priority,id, cls) ))\
		{\
			TLOGEVENT stEvt;\
			const TLOGLOCINFO locinfo = TLOG_LOC_INFO_INITIALIZER(NULL);\
			stEvt.evt_priority = priority; \
			stEvt.evt_id = id; \
			stEvt.evt_cls  = cls; \
			stEvt.evt_type = type; \
			stEvt.evt_version = version; \
			stEvt.evt_is_msg_binary = 1; \
			stEvt.evt_msg = buff; \
			stEvt.evt_msg_len = bufflen; \
			stEvt.evt_loc = &locinfo; \
			tlog_category_log(cat, &stEvt);\
		}\
	}while(0)

#define tlog_fatal_bin(cat, id, cls, type, version, buff, len)		tlog_log_bin(cat, TLOG_PRIORITY_FATAL, id, cls, type, version, buff, len)
#define tlog_alert_bin(cat, id, cls, type, version, buff, len)		tlog_log_bin(cat, TLOG_PRIORITY_ALERT, id, cls, type, version, buff, len)
#define tlog_crit_bin(cat, id, cls, type, version, buff, len)		tlog_log_bin(cat, TLOG_PRIORITY_CRIT, id, cls, type, version, buff, len)
#define tlog_error_bin(cat, id, cls, type, version, buff, len)		tlog_log_bin(cat, TLOG_PRIORITY_ERROR, id, cls, type, version, buff, len)
#define tlog_warn_bin(cat, id, cls, type, version, buff, len)		tlog_log_bin(cat, TLOG_PRIORITY_WARN, id, cls, type, version, buff, len)
#define tlog_notice_bin(cat, id, cls, type, version, buff, len)		tlog_log_bin(cat, TLOG_PRIORITY_NOTICE, id, cls, type, version, buff, len)
#define tlog_info_bin(cat, id, cls, type, version, buff, len)		tlog_log_bin(cat, TLOG_PRIORITY_INFO, id, cls, type, version, buff, len)
#define tlog_debug_bin(cat, id, cls, type, version, buff, len)		tlog_log_bin(cat, TLOG_PRIORITY_DEBUG, id, cls, type, version, buff, len)
#define tlog_trace_bin(cat, id, cls, type, version, buff, len)		tlog_log_bin(cat, TLOG_PRIORITY_TRACE, id, cls, type, version, buff, len)

#define tlog_log_text(cat, priority, id, cls, buff, bufflen)\
	do{\
		if(tlog_category_is_priority_enabled(cat, priority)&& (tlog_category_can_write(cat, priority,id, cls) ))\
		{\
			TLOGEVENT stEvt;\
			const TLOGLOCINFO locinfo = TLOG_LOC_INFO_INITIALIZER(NULL);\
			stEvt.evt_priority = priority; \
			stEvt.evt_id = id; \
			stEvt.evt_cls  = cls; \
			stEvt.evt_is_msg_binary = 0; \
			stEvt.evt_msg = buff; \
			stEvt.evt_msg_len = bufflen; \
			stEvt.evt_loc = &locinfo; \
			tlog_category_log(cat, &stEvt);\
		}\
	}while(0)

#define tlog_fatal_text(cat, id, cls, buff, len)		tlog_log_text(cat, TLOG_PRIORITY_FATAL, id, cls, buff, len)
#define tlog_alert_text(cat, id, cls, buff, len)		tlog_log_text(cat, TLOG_PRIORITY_ALERT, id, cls, buff, len)
#define tlog_crit_text(cat, id, cls, buff, len)			tlog_log_text(cat, TLOG_PRIORITY_CRIT, id, cls, buff, len)
#define tlog_error_text(cat, id, cls, buff, len)		tlog_log_text(cat, TLOG_PRIORITY_ERROR, id, cls, buff, len)
#define tlog_warn_text(cat, id, cls, buff, len)			tlog_log_text(cat, TLOG_PRIORITY_WARN, id, cls, buff, len)
#define tlog_notice_text(cat, id, cls, buff, len)		tlog_log_text(cat, TLOG_PRIORITY_NOTICE, id, cls, buff, len)
#define tlog_info_text(cat, id, cls, buff, len)			tlog_log_text(cat, TLOG_PRIORITY_INFO, id, cls, buff, len)
#define tlog_debug_text(cat, id, cls, buff, len)		tlog_log_text(cat, TLOG_PRIORITY_DEBUG, id, cls, buff, len)
#define tlog_trace_text(cat, id, cls, buff, len)		tlog_log_text(cat, TLOG_PRIORITY_TRACE, id, cls, buff, len)

#define tlog_log_str(cat, priority, id, cls, str)		tlog_log_text(cat, priority, id, cls, str, strlen(str))

#define tlog_fatal_str(cat, id, cls, str)			tlog_log_str(cat, TLOG_PRIORITY_FATAL, id, cls, str)
#define tlog_alert_str(cat, id, cls, str)			tlog_log_str(cat, TLOG_PRIORITY_ALERT, id, cls, str)
#define tlog_crit_str(cat, id, cls, str)			tlog_log_str(cat, TLOG_PRIORITY_CRIT, id, cls, str)
#define tlog_error_str(cat, id, cls, str)			tlog_log_str(cat, TLOG_PRIORITY_ERROR, id, cls, str)
#define tlog_warn_str(cat, id, cls, str)			tlog_log_str(cat, TLOG_PRIORITY_WARN, id, cls, str)
#define tlog_notice_str(cat, id, cls, str)			tlog_log_str(cat, TLOG_PRIORITY_NOTICE, id, cls, str)
#define tlog_info_str(cat, id, cls, str)			tlog_log_str(cat, TLOG_PRIORITY_INFO, id, cls, str)
#define tlog_debug_str(cat, id, cls, str)			tlog_log_str(cat, TLOG_PRIORITY_DEBUG, id, cls, str)
#define tlog_trace_str(cat, id, cls, str)			tlog_log_str(cat, TLOG_PRIORITY_TRACE, id, cls, str)

#define tlog_log_dr(a_cat, a_priority, a_id, a_cls, a_pmeta, a_buff, a_bufflen, a_version)\
	do{\
		if((tlog_category_is_priority_enabled(a_cat, a_priority))&& (tlog_category_can_write(a_cat, a_priority,a_id, a_cls) ))\
		{\
			if(1)\
			{\
				TLOGEVENT stEvt;\
				const TLOGLOCINFO locinfo = TLOG_LOC_INFO_INITIALIZER(NULL);\
				stEvt.evt_priority = a_priority; \
			    stEvt.evt_id = a_id; \
			    stEvt.evt_cls  = a_cls; \
				stEvt.evt_loc = &locinfo; 	\
				stEvt.evt_is_msg_binary = 0; \
				tlog_category_log_tdr( a_cat,  &stEvt,  a_pmeta,  a_buff,  a_bufflen, a_version);\
			}\
		}\
	}while(0)

#define tlog_fatal_dr(cat, id, cls, pmeta, buff, bufflen, version)		tlog_log_dr(cat, TLOG_PRIORITY_FATAL, id, cls,  pmeta, buff, bufflen, version)
#define tlog_alert_dr(cat, id, cls, pmeta, buff, bufflen, version)		tlog_log_dr(cat, TLOG_PRIORITY_ALERT,  id, cls, pmeta, buff, bufflen, version)
#define tlog_crit_dr(cat, id, cls, pmeta, buff, bufflen, version)		tlog_log_dr(cat, TLOG_PRIORITY_CRIT,  id, cls, pmeta, buff, bufflen, version)
#define tlog_error_dr(cat, id, cls, pmeta, buff, bufflen, version)		tlog_log_dr(cat, TLOG_PRIORITY_ERROR,  id, cls, pmeta, buff, bufflen, version)
#define tlog_warn_dr(cat, id, cls, pmeta, buff, bufflen, version)		tlog_log_dr(cat, TLOG_PRIORITY_WARN,  id, cls, pmeta, buff, bufflen, version)
#define tlog_notice_dr(cat, id, cls, pmeta, buff, bufflen, version)		tlog_log_dr(cat, TLOG_PRIORITY_NOTICE,  id, cls, pmeta, buff, bufflen, version)
#define tlog_info_dr(cat, id, cls, pmeta, buff, bufflen, version)		tlog_log_dr(cat, TLOG_PRIORITY_INFO,  id, cls, pmeta, buff, bufflen, version)
#define tlog_debug_dr(cat, id, cls, pmeta, buff, bufflen, version)		tlog_log_dr(cat, TLOG_PRIORITY_DEBUG,  id, cls, pmeta, buff, bufflen, version)
#define tlog_trace_dr(cat, id, cls, pmeta, buff, bufflen, version)		tlog_log_dr(cat, TLOG_PRIORITY_TRACE,  id, cls, pmeta, buff, bufflen, version)

/*
 Write hex strings into log file.
 */
#define tlog_log_hex(cat, priority, id, cls, buff, bufflen)\
	do{\
		if(tlog_category_is_priority_enabled(cat, priority)&& (tlog_category_can_write(cat, priority,id, cls) ))\
		{\
			TLOGEVENT stEvt;\
			const TLOGLOCINFO locinfo = TLOG_LOC_INFO_INITIALIZER(NULL);\
			stEvt.evt_priority = priority; \
			stEvt.evt_id = id; \
			stEvt.evt_cls  = cls; \
			stEvt.evt_is_msg_binary = 0; \
			stEvt.evt_msg = buff; \
			stEvt.evt_msg_len = bufflen; \
			stEvt.evt_loc = &locinfo; \
			tlog_category_log_hex(cat, &stEvt);\
		}\
	}while(0)

#define tlog_fatal_hex(cat, id, cls, buff, len)		tlog_log_hex(cat, TLOG_PRIORITY_FATAL, id, cls, buff, len)
#define tlog_alert_hex(cat, id, cls, buff, len)		tlog_log_hex(cat, TLOG_PRIORITY_ALERT, id, cls, buff, len)
#define tlog_crit_hex(cat, id, cls, buff, len) 		tlog_log_hex(cat, TLOG_PRIORITY_CRIT, id, cls, buff, len)
#define tlog_error_hex(cat, id, cls, buff, len)		tlog_log_hex(cat, TLOG_PRIORITY_ERROR, id, cls, buff, len)
#define tlog_warn_hex(cat, id, cls, buff, len) 		tlog_log_hex(cat, TLOG_PRIORITY_WARN, id, cls, buff, len)
#define tlog_notice_hex(cat, id, cls, buff, len)	tlog_log_hex(cat, TLOG_PRIORITY_NOTICE, id, cls, buff, len)
#define tlog_info_hex(cat, id, cls, buff, len) 		tlog_log_hex(cat, TLOG_PRIORITY_INFO, id, cls, buff, len)
#define tlog_debug_hex(cat, id, cls, buff, len)		tlog_log_hex(cat, TLOG_PRIORITY_DEBUG, id, cls, buff, len)
#define tlog_trace_hex(cat, id, cls, buff, len)		tlog_log_hex(cat, TLOG_PRIORITY_TRACE, id, cls, buff, len)

#ifdef __cplusplus
}
#endif

#endif /* TLOG_H */

