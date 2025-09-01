/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DECCOLLECTIONINTERFACES_H_
#define _DECCOLLECTIONINTERFACES_H_

#include "../../dragengine_export.h"

class deObject;
class deThreadSafeObject;


/** \brief Object comparator. */
class DE_DLL_EXPORT decObjectComparator{
public:
	decObjectComparator() = default;
	
	/**
	 * \brief Compare objects.
	 * \param[in] a First object to compare.
	 * \param[in] b Second object to compare.
	 * \returns 0 if a equals b, >0 if a is greater than b otherwise <0.
	 */
	virtual int operator() (deObject *a, deObject *b) = 0;
};

/** \brief Object visitor. */
class DE_DLL_EXPORT decObjectVisitor{
public:
	decObjectVisitor() = default;
	
	/**
	 * \brief Visit object.
	 * \param[in] object Object to visit.
	 */
	virtual void operator() (deObject *object) = 0;
};

/** \brief Object evaluator. */
class DE_DLL_EXPORT decObjectEvaluator{
public:
	decObjectEvaluator() = default;
	
	/**
	 * \brief Evaluate object.
	 * \param[in] object Object to evaluate.
	 * \returns true if object matches criterias otherwise false.
	 */
	virtual bool operator() (deObject *object) = 0;
};


/** \brief Pointer comparator. */
class DE_DLL_EXPORT decPointerComparator{
public:
	decPointerComparator() = default;
	
	/**
	 * \brief Compare pointers.
	 * \param[in] a First pointer to compare.
	 * \param[in] b Second pointer to compare.
	 * \returns 0 if a equals b, >0 if a is greater than b otherwise <0.
	 */
	virtual int operator() (void *a, void *b) = 0;
};

/** \brief Pointer visitor. */
class DE_DLL_EXPORT decPointerVisitor{
public:
	decPointerVisitor() = default;
	
	/**
	 * \brief Visit pointer.
	 * \param[in] pointer Pointer to visit.
	 */
	virtual void operator() (void *pointer) = 0;
};

/** \brief Pointer evaluator. */
class DE_DLL_EXPORT decPointerEvaluator{
public:
	decPointerEvaluator() = default;
	
	/**
	 * \brief Evaluate pointer.
	 * \param[in] pointer Pointer to evaluate.
	 * \returns true if pointer matches criterias otherwise false.
	 */
	virtual bool operator() (void *pointer) = 0;
};


/** \brief ThreadSafeObject comparator. */
class DE_DLL_EXPORT decThreadSafeObjectComparator{
public:
	decThreadSafeObjectComparator() = default;
	
	/**
	 * \brief Compare threadsafeobjects.
	 * \param[in] a First threadsafeobject to compare.
	 * \param[in] b Second threadsafeobject to compare.
	 * \returns 0 if a equals b, >0 if a is greater than b otherwise <0.
	 */
	virtual int operator() (deThreadSafeObject *a, deThreadSafeObject *b) = 0;
};

/** \brief ThreadSafeObject visitor. */
class DE_DLL_EXPORT decThreadSafeObjectVisitor{
public:
	decThreadSafeObjectVisitor() = default;
	
	/**
	 * \brief Visit threadsafeobject.
	 * \param[in] threadsafeobject ThreadSafeObject to visit.
	 */
	virtual void operator() (deThreadSafeObject *threadsafeobject) = 0;
};

/** \brief ThreadSafeObject evaluator. */
class DE_DLL_EXPORT decThreadSafeObjectEvaluator{
public:
	decThreadSafeObjectEvaluator() = default;
	
	/**
	 * \brief Evaluate threadsafeobject.
	 * \param[in] threadsafeobject ThreadSafeObject to evaluate.
	 * \returns true if threadsafeobject matches criterias otherwise false.
	 */
	virtual bool operator() (deThreadSafeObject *threadsafeobject) = 0;
};

#endif
