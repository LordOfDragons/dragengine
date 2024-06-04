/* ==============================================================================
 * Copyright (c) Microsoft Corporation.  All rights reserved.
 * =========================================================================== */
#pragma once

#ifndef _GRDK_
#define _GRDK_

#ifdef _GRDK_VER
#undef _GRDK_VER
#endif

/* Version */
#define _GRDK_VER                0x63360C63

#if !defined (_GRDK_NO_XBLD_INFO)

#define _GRDK_STRINGIZE(s)       #s
#define _GRDK_STRINGIZE_W(s)     L## #s
#define GRDK_STRINGIZE(s)        _GRDK_STRINGIZE(s)
#define GRDK_STRINGIZE_W(s)      _GRDK_STRINGIZE_W(s)

/*  Edition  */
#define _GRDK_YY                 24
#define _GRDK_MM_NUM             3
#define _GRDK_MM                 03
#define _GRDK_QFE_NUM            1            /* QFE version used in comparisons */
#define _GRDK_QFE                01        /* This goes into the 6 digit edition */
#define _GRDK_FULLYY             2024
#define _GRDK_EDITION            240301


#if (_GRDK_YY > 0)
    #if    (_GRDK_YY > 99)
        #pragma error "Invalid _GRDK_YY value, must be in the range 01-99"
    #endif
#else
    #pragma error "Invalid _GRDK_YY value, must be in the range 01-99"
#endif

#if    (_GRDK_QFE_NUM > 0)
    #define _GRDK_QFE_NAME_VALID
    #define _GRDK_QFE_NAME       "Update " GRDK_STRINGIZE(_GRDK_QFE_NUM)
    #define _GRDK_QFE_NAME_W    L"Update " GRDK_STRINGIZE_W(_GRDK_QFE_NUM)
#elif  (_GRDK_QFE_NUM == 0)
    #define _GRDK_QFE_NAME       ""
    #define _GRDK_QFE_NAME_W    L""
#else
    #pragma error "Invalid _GRDK_QFE value, must be in the range 0+"
#endif

#if    (_GRDK_MM_NUM == 12)
    #define _GRDK_MM_NAME        "December"
    #define _GRDK_MM_NAME_W     L"December"
#elif  (_GRDK_MM_NUM == 11)
    #define _GRDK_MM_NAME        "November"
    #define _GRDK_MM_NAME_W     L"November"
#elif  (_GRDK_MM_NUM == 10)
    #define _GRDK_MM_NAME        "October"
    #define _GRDK_MM_NAME_W     L"October"
#elif  (_GRDK_MM_NUM == 9)
    #define _GRDK_MM_NAME        "September"
    #define _GRDK_MM_NAME_W     L"September"
#elif  (_GRDK_MM_NUM == 8)
    #define _GRDK_MM_NAME        "August"
    #define _GRDK_MM_NAME_W     L"August"
#elif  (_GRDK_MM_NUM == 7)
    #define _GRDK_MM_NAME        "July"
    #define _GRDK_MM_NAME_W     L"July"
#elif  (_GRDK_MM_NUM == 6)
    #define _GRDK_MM_NAME        "June"
    #define _GRDK_MM_NAME_W     L"June"
#elif  (_GRDK_MM_NUM == 5)
    #define _GRDK_MM_NAME        "May"
    #define _GRDK_MM_NAME_W     L"May"
#elif  (_GRDK_MM_NUM == 4)
    #define _GRDK_MM_NAME        "April"
    #define _GRDK_MM_NAME_W     L"April"
#elif  (_GRDK_MM_NUM == 3)
    #define _GRDK_MM_NAME        "March"
    #define _GRDK_MM_NAME_W     L"March"
#elif  (_GRDK_MM_NUM == 2)
    #define _GRDK_MM_NAME        "February"
    #define _GRDK_MM_NAME_W     L"February"
#elif  (_GRDK_MM_NUM == 1)
    #define _GRDK_MM_NAME        "January"
    #define _GRDK_MM_NAME_W     L"January"
#else
    #pragma error "Invalid _GRDK_MM value, must be in the range 01-12"
#endif

#ifdef _GRDK_VER_STRING
#undef _GRDK_VER_STRING
#endif

#if defined(_GRDK_QFE_NAME_VALID)
    #define _GRDK_VER_STRING   _GRDK_MM_NAME    " " GRDK_STRINGIZE(_GRDK_FULLYY)    " GRDK " _GRDK_QFE_NAME
    #define _GRDK_VER_STRING_W _GRDK_MM_NAME_W L" " GRDK_STRINGIZE_W(_GRDK_FULLYY) L" GRDK " _GRDK_QFE_NAME_W
    #define _GRDK_VER_STRING_COMPACT_W _GRDK_MM_NAME_W L" " GRDK_STRINGIZE_W(_GRDK_FULLYY) L" " _GRDK_QFE_NAME_W
#else
    #define _GRDK_VER_STRING   _GRDK_MM_NAME    " " GRDK_STRINGIZE(_GRDK_FULLYY)    " GRDK"
    #define _GRDK_VER_STRING_W _GRDK_MM_NAME_W L" " GRDK_STRINGIZE_W(_GRDK_FULLYY) L" GRDK"
    #define _GRDK_VER_STRING_COMPACT_W _GRDK_MM_NAME_W L" " GRDK_STRINGIZE_W(_GRDK_FULLYY)
#endif


/* Release / debug / profile */
#if defined (PROFILE)
    #define _GRDK_CLIENTBLDMODE     "profile"
    #define _GRDK_CLIENTBLDMODE_W  L"profile"
#endif


#define _GRDKVER_PRODUCTMAJORVERSION     10
#define _GRDKVER_PRODUCTMINORVERSION     0
#define _GRDKVER_PRODUCTBUILD_QFE        3171
#define _GRDKVER_PRODUCTBUILDVER_FULL    10.0.25398.3171

/* Compiler */
#ifdef __clang__
#define _GRDK_FULLMSCVVER_               "clang." __clang_version__
#define _GRDK_FULLMSCVVER_W_             L"clang." __clang_version__
#elif defined(_MSC_FULL_VER)
#define _GRDK_FULLMSCVVER_               GRDK_STRINGIZE(_MSC_FULL_VER)    "." GRDK_STRINGIZE(_MSC_BUILD)
#define _GRDK_FULLMSCVVER_W_             GRDK_STRINGIZE_W(_MSC_FULL_VER) L"." GRDK_STRINGIZE_W(_MSC_BUILD)
#else
#define _GRDK_FULLMSCVVER_               "UnknownCompiler"
#define _GRDK_FULLMSCVVER_W_             L"UnknownCompiler"
#endif

/* .xbld linker section */
#ifdef __cplusplus
extern "C" {
#endif

#pragma section(".xbld", read, write, discard)

#ifdef _M_IX86
#define underscore(s) s
#else
#define underscore(s) _##s
#endif

/* Leaving this variable for backwards compatibility but only need the edition-specific one preserved by the linker */
    __declspec(allocate(".xbld")) __declspec(selectany) char underscore(XBLD_GRDK_NAME)[] = "_xbld_name=" _GRDK_VER_STRING;
#pragma comment(linker, "/include:_XBLD_GRDK_NAME_240301")
    __declspec(allocate(".xbld")) __declspec(selectany) char underscore(XBLD_GRDK_NAME_240301)[] = "_xbld_edition_name=GRDK," _GRDK_VER_STRING;
/* Leaving this variable for backwards compatibility but only need the edition-specific one preserved by the linker */
    __declspec(allocate(".xbld")) __declspec(selectany) char underscore(XBLD_GRDK_PRODUCTBUILDVER_FULL)[] = "_xbld_full_productbuild=" GRDK_STRINGIZE(_GRDKVER_PRODUCTBUILDVER_FULL);
#pragma comment(linker, "/include:_XBLD_GRDK_PRODUCTBUILDVER_FULL_240301")
    __declspec(allocate(".xbld")) __declspec(selectany) char underscore(XBLD_GRDK_PRODUCTBUILDVER_FULL_240301)[] = "_xbld_edition_full_productbuild=GRDK," GRDK_STRINGIZE(_GRDKVER_PRODUCTBUILDVER_FULL);
/* Leaving this variable for backwards compatibility but only need the edition-specific one preserved by the linker */
    __declspec(allocate(".xbld")) __declspec(selectany) char underscore(XBLD_GRDK_EDITION)[] = "_xbld_edition=" GRDK_STRINGIZE(_GRDK_EDITION);
#pragma comment(linker, "/include:_XBLD_GRDK_EDITION_240301")
    __declspec(allocate(".xbld")) __declspec(selectany) char underscore(XBLD_GRDK_EDITION_240301)[] = "_xbld_edition_sdktype=GRDK," GRDK_STRINGIZE(_GRDK_EDITION);
/* Leaving this variable for backwards compatibility but only need the edition-specific one preserved by the linker */
 __declspec(allocate(".xbld")) __declspec(selectany) char underscore(XBLD_GRDK_MSCVER)[]    = "_xbld_mscver=" _GRDK_FULLMSCVVER_;
#pragma comment(linker, "/include:_XBLD_GRDK_MSCVER_240301")
    __declspec(allocate(".xbld")) __declspec(selectany) char underscore(XBLD_GRDK_MSCVER_240301)[]    = "_xbld_edition_mscver=GRDK," _GRDK_FULLMSCVVER_;
/* Leaving this variable for backwards compatibility but only need the edition-specific one preserved by the linker */
__declspec(allocate(".xbld")) __declspec(selectany) char underscore(XBLD_GRDK_BUILD)[] = "_xbld_build=" GRDK_STRINGIZE(_GRDK_VER);
#pragma comment(linker, "/include:_XBLD_GRDK_BUILD_240301")
    __declspec(allocate(".xbld")) __declspec(selectany) char underscore(XBLD_GRDK_BUILD_240301)[] = "_xbld_edition_build=GRDK," GRDK_STRINGIZE(_GRDK_VER);


#if defined (_GRDK_CLIENTBLDMODE)
/* Leaving this variable for backwards compatibility but only need the edition-specific one preserved by the linker */
   __declspec(allocate(".xbld")) __declspec(selectany) char underscore(XBLD_GRDK_CLIENT_BUILD_TYPE)[] = "_xbld_clientbldtype=" _GRDK_CLIENTBLDMODE;
#pragma comment(linker, "/include:_XBLD_GRDK_CLIENT_BUILD_TYPE_240301")
    __declspec(allocate(".xbld")) __declspec(selectany) char underscore(XBLD_GRDK_CLIENT_BUILD_TYPE_240301)[] = "_xbld_edition_clientbldtype=GRDK," _GRDK_CLIENTBLDMODE;
#endif

#pragma data_seg()

#ifdef __cplusplus
}
#endif


#endif /* _GRDK_NO_XBLD_INFO */
#endif /* _GRDK_ */
