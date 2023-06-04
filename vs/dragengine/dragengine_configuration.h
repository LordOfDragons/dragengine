#pragma once

#ifndef DE_VERSION
#define DE_VERSION "%BuildVersion%"
#endif

#ifndef OS_W32
#define OS_W32 1
#endif

#ifndef OS_W32_VS
#define OS_W32_VS 1
#endif

#ifdef OS_W32_APPSTORE
#undef OS_W32_APPSTORE
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
