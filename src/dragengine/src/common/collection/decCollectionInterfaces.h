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


/** \brief Comparator. */
template<typename T>
class decTComparator{
public:
	decTComparator() = default;
	
	/**
	 * \brief Compare elements.
	 * \param[in] a First element to compare.
	 * \param[in] b Second element to compare.
	 * \returns 0 if a equals b, >0 if a is greater than b otherwise <0.
	 */
	virtual int operator() (const T &a, const T &b) = 0;
};


/** \brief Visitor. */
template<typename T>
class decTVisitor{
public:
	decTVisitor() = default;
	
	/**
	 * \brief Visit element.
	 * \param[in] element Element to visit.
	 */
	virtual void operator() (T &element) = 0;
};


/** \brief Key/value visitor. */
template<typename K, typename V>
class decTKeyValueVisitor{
public:
	decTKeyValueVisitor() = default;
	
	/**
	 * \brief Visit element.
	 * \param[in] key Key to visit.
	 * \param[in] value Value to visit.
	 */
	virtual void operator() (const K &key, V &value) = 0;
};


/** \brief Evaluator. */
template<typename T>
class decTEvaluator{
public:
	decTEvaluator() = default;
	
	/**
	 * \brief Evaluate element.
	 * \param[in] element Element to evaluate.
	 * \returns true if element matches criterias otherwise false.
	 */
	virtual bool operator() (const T &element) = 0;
};


/** \brief Ascending comparator using Compare() function. */
template<typename T>
class decAscendingComparator : public decTComparator<T>{
public:
	decAscendingComparator() = default;
	int operator() (const T &a, const T &b) override {
		return a.Compare(b);
	}
};

/** \brief Ascending comparator for int, float and double. */
template<>
class decAscendingComparator<int> : public decTComparator<int> {
public:
	decAscendingComparator() = default;
	int operator() (const int &a, const int &b) override {
		return a - b;
	}
};

template<>
class decAscendingComparator<float> : public decTComparator<float> {
public:
	decAscendingComparator() = default;
	int operator() (const float &a, const float &b) override {
		return (a < b) ? -1 : ((a > b) ? 1 : 0);
	}
};

template<>
class decAscendingComparator<double> : public decTComparator<double> {
public:
	decAscendingComparator() = default;
	int operator() (const double &a, const double &b) override {
		return (a < b) ? -1 : ((a > b) ? 1 : 0);
	}
};


/** \brief Descending comparator using Compare() function. */
template<typename T>
class decDesendingComparator : public decTComparator<T>{
public:
	decDesendingComparator() = default;
	int operator() (const T &a, const T &b) override {
		return b.Compare(a);
	}
};

/** \brief Descending comparator for int, float and double. */
template<>
class decDesendingComparator<int> : public decTComparator<int> {
public:
	decDesendingComparator() = default;
	int operator() (const int &a, const int &b) override {
		return b - a;
	}
};

template<>
class decDesendingComparator<float> : public decTComparator<float> {
public:
	decDesendingComparator() = default;
	int operator() (const float &a, const float &b) override {
		return (b < a) ? -1 : ((b > a) ? 1 : 0);
	}
};

template<>
class decDesendingComparator<double> : public decTComparator<double> {
public:
	decDesendingComparator() = default;
	int operator() (const double &a, const double &b) override {
		return (b < a) ? -1 : ((b > a) ? 1 : 0);
	}
};


/** \brief Object comparator. */
using decObjectComparator = decTComparator<deObject*>;

/** \brief Object visitor. */
using decObjectVisitor = decTVisitor<deObject*>;

/** \brief Object evaluator. */
using decObjectEvaluator = decTEvaluator<deObject*>;


/** \brief Pointer comparator. */
using decPointerComparator = decTComparator<void*>;

/** \brief Pointer visitor. */
using decPointerVisitor = decTVisitor<void*>;

/** \brief Pointer evaluator. */
using decPointerEvaluator = decTEvaluator<void*>;


/** \brief ThreadSafeObject comparator. */
using decThreadSafeObjectComparator = decTComparator<deThreadSafeObject*>;

/** \brief ThreadSafeObject visitor. */
using decThreadSafeObjectVisitor = decTVisitor<deThreadSafeObject*>;

/** \brief ThreadSafeObject evaluator. */
using decThreadSafeObjectEvaluator = decTEvaluator<deThreadSafeObject*>;

#endif
