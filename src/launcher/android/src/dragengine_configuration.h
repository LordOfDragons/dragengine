/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/* enable OS_ANDROID */
#ifndef OS_ANDROID
#define OS_ANDROID 1
#endif

/* enable HAS_LIB_DL */
#ifndef HAS_LIB_DL
#define HAS_LIB_DL 1
#endif

/* enable OS_UNIX */
#ifndef OS_UNIX
#define OS_UNIX 1
#endif

/* disable OS_W32 */
#ifdef OS_W32
#undef OS_W32
#endif

/* enable HAS_FUNC_UTIMENSAT */
#ifndef HAS_FUNC_UTIMENSAT
#define HAS_FUNC_UTIMENSAT 1
#endif

/* disable HAS_FUNC_PTRHEAD_CANCEL */
#ifdef HAS_FUNC_PTRHEAD_CANCEL
#undef HAS_FUNC_PTRHEAD_CANCEL
#endif

/* disable OS_BEOS */
#ifdef OS_BEOS
#undef OS_BEOS
#endif

/* disable HAS_LIB_X11 */
#ifdef HAS_LIB_X11
#undef HAS_LIB_X11
#endif

/* enable HAS_LIB_PTHREAD */
#ifndef HAS_LIB_PTHREAD
#define HAS_LIB_PTHREAD 1
#endif

/* End of configuration file. */
