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

#ifndef _EXCEPTIONS_REDUCED_H_
#define _EXCEPTIONS_REDUCED_H_

#include "../dragengine_export.h"

extern DE_DLL_EXPORT void DEThrowNullPointer(const char *file, int line, const char *message);
extern DE_DLL_EXPORT void DEThrowInvalidParam(const char *file, int line, const char *message);


/**
 * \brief Throw deeNullPointer exception if pointer is nullptr.
 * \version 1.10
 */
#define DEASSERT_NOTNULL(pointer) if(!(pointer)) DEThrowNullPointer(__FILE__, __LINE__, "assertNotNull(" #pointer ")");

/**
 * \brief Throw deeInvalidParam exception if pointer is not nullptr.
 * \version 1.10
 */
#define DEASSERT_NULL(pointer) if(pointer) DEThrowInvalidParam(__FILE__, __LINE__, "assertNull(" #pointer ")");

/**
 * \brief Throw deeInvalidParam exception if condition is false.
 * \version 1.10
 */
#define DEASSERT_TRUE(condition) if(!(condition)) DEThrowInvalidParam(__FILE__, __LINE__, "assertTrue(" #condition ")");

/**
 * \brief Throw deeInvalidParam exception if condition is true.
 * \version 1.10
 */
#define DEASSERT_FALSE(condition) if(condition) DEThrowInvalidParam(__FILE__, __LINE__, "assertFalse(" #condition ")");





/**
 * \brief If precondition is true throw deeNullPointer exception if pointer is nullptr.
 * \version 1.25
 */
#define DEASSERT_NOTNULL_IF(precondition, pointer) if((precondition) && !(pointer)) \
	DEThrowNullPointer(__FILE__, __LINE__, "assertNotNull(" #precondition " -> " #pointer ")");

/**
 * \brief If precondition is true throw deeInvalidParam exception if pointer is not nullptr.
 * \version 1.25
 */
#define DEASSERT_NULL_IF(precondition, pointer) if((precondition) && (pointer)) \
	DEThrowInvalidParam(__FILE__, __LINE__, "assertNull(" #precondition " -> " #pointer ")");

/**
 * \brief If precondition is true throw deeInvalidParam exception if condition is false.
 * \version 1.25
 */
#define DEASSERT_TRUE_IF(precondition, condition) if((precondition) && !(condition)) \
	DEThrowInvalidParam(__FILE__, __LINE__, "assertTrue(" #precondition " -> " #condition ")");

/**
 * \brief If precondition is true throw deeInvalidParam exception if condition is true.
 * \version 1.25
 */
#define DEASSERT_FALSE_IF(precondition, condition) if((precondition) && (condition)) \
	DEThrowInvalidParam(__FILE__, __LINE__, "assertFalse(" #precondition " -> " #condition ")");

#endif
