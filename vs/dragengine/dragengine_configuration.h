#pragma once

#ifndef OS_W32
#define OS_W32 1
#endif

#ifndef OS_W32_VS
#define OS_W32_VS 1
#endif

#ifndef OS_W32_APPSTORE
#define OS_W32_APPSTORE 1
#endif

#if defined WINAPI_FAMILY && WINAPI_FAMILY == WINAPI_FAMILY_APP
	#ifndef OS_W32_UWP
	#define OS_W32_UWP 1
	#endif

#else
	#ifdef OS_W32_UWP
	#undef OS_W32_UWP
	#endif
#endif
