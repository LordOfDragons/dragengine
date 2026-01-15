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

#include "decGlobalFunctions_safe.h"


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


/** \brief Indexed visitor. */
template<typename T>
class decTIndexedVisitor{
public:
	decTIndexedVisitor() = default;
	
	/**
	 * \brief Visit element.
	 * \param[in] index Index of visited element.
	 * \param[in] element Element to visit.
	 */
	virtual void operator() (int index, T &element) = 0;
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


/** \brief Ascending comparator using DECompare() function. */
template<typename T>
class decAscendingComparator : public decTComparator<T>{
public:
	decAscendingComparator() = default;
	int operator() (const T &a, const T &b) override{
		return DECompare(a, b);
	}
};


/** \brief Descending comparator using DECompare() function. */
template<typename T>
class decDesendingComparator : public decTComparator<T>{
public:
	decDesendingComparator() = default;
	int operator() (const T &a, const T &b) override{
		return DECompare(b, a);
	}
};


/** \brief Template function bundle for classes supporting queries using GetName(). */
template<typename Base, typename T>
class decTCollectionQueryByName : public Base{
public:
	using Base::Base;
	
	/**
	 * \brief Create copy of collection.
	 * \note Not explicit on purpose.
	 */
	decTCollectionQueryByName(const Base &base) : Base(base){}
	
	/**
	 * \brief Move collection.
	 * \note Not explicit on purpose.
	 */
	decTCollectionQueryByName(Base &&base) : Base(base){}
	
	/** \brief One or more elements match name. */
	inline bool HasNamed(const char *name) const{
		return this->HasMatching([&](const T &e){
			return e.GetName() == name;
		});
	}
	
	/** \brief Index of first element matching name or -1 if absent. */
	inline int IndexOfNamed(const char *name) const{
		return this->IndexOfMatching([&](const T &e){
			return e.GetName() == name;
		});
	}
	
	/** \brief First element matching name or nullptr if absent. */
	inline T *FindNamed(const char *name) const{
		return this->FindOrDefault([&](const T &e){
			return e.GetName() == name;
		});
	}
};


/** \brief Template function bundle for classes supporting queries using GetPath(). */
template<typename Base, typename T>
class decTCollectionQueryByPath : public Base{
public:
	using Base::Base;
	
	/**
	 * \brief Create copy of collection.
	 * \note Not explicit on purpose.
	 */
	decTCollectionQueryByPath(const Base &base) : Base(base){}
	
	/**
	 * \brief Move collection.
	 * \note Not explicit on purpose.
	 */
	decTCollectionQueryByPath(Base &&base) : Base(base){}
	
	/** \brief One or more elements match path. */
	inline bool HasWithPath(const char *path) const{
		return this->HasMatching([&](const T &e){
			return e.GetPath() == path;
		});
	}
	
	/** \brief Index of first element matching path or -1 if absent. */
	inline int IndexOfWithPath(const char *path) const{
		return this->IndexOfMatching([&](const T &e){
			return e.GetPath() == path;
		});
	}
	
	/** \brief First element matching path or nullptr if absent. */
	inline T *FindWithPath(const char *path) const{
		return this->FindOrDefault([&](const T &e){
			return e.GetPath() == path;
		});
	}
};


/** \brief Template function bundle for classes supporting queries using GetPath() and GetName(). */
template<typename Base, typename T>
class decTCollectionQueryByPathOrName : public decTCollectionQueryByPath<decTCollectionQueryByName<Base,T>,T>{
	using BaseType = decTCollectionQueryByPath<decTCollectionQueryByName<Base,T>,T>;
	
public:
	using BaseType::decTCollectionQueryByPath;
	
	/**
	 * \brief Create copy of collection.
	 * \note Not explicit on purpose.
	 */
	decTCollectionQueryByPathOrName(const BaseType &base) : BaseType(base){}
	
	/**
	 * \brief Move collection.
	 * \note Not explicit on purpose.
	 */
	decTCollectionQueryByPathOrName(BaseType &&base) : BaseType(base){}
	
	/** \brief One or more elements match path or name. */
	inline bool HasWithPathOrName(const char *path, const char *name) const{
		return this->HasMatching([&](const T &e){
			return e.GetPath() == path || e.GetName() == name;
		});
	}
	
	/** \brief Index of first element matching path or name or -1 if absent. */
	inline int IndexOfWithPathOrName(const char *path, const char *name) const{
		return this->IndexOfMatching([&](const T &e){
			return e.GetPath() == path || e.GetName() == name;
		});
	}
	
	/** \brief First element matching path or name or nullptr if absent. */
	inline T *FindWithPathOrName(const char *path, const char *name) const{
		return this->FindOrDefault([&](const T &e){
			return e.GetPath() == path || e.GetName() == name;
		});
	}
};


/** \brief Template function bundle for classes supporting queries using GetId() or GetIdentifier(). */
template<typename Base, typename T>
class decTCollectionQueryById : public Base{
public:
	using Base::Base;
	
	/**
	 * \brief Create copy of collection.
	 * \note Not explicit on purpose.
	 */
	decTCollectionQueryById(const Base &base) : Base(base){}
	
	/**
	 * \brief Move collection.
	 * \note Not explicit on purpose.
	 */
	decTCollectionQueryById(Base &&base) : Base(base){}
	
	/** \brief One or more elements match identifier. */
	inline bool HasWithId(const char *id) const{
		return this->HasMatching([&](const T &e){
			if constexpr (requires { e.GetIdentifier(); }) {
				return e.GetIdentifier() == id;
			} else {
				return e.GetId() == id;
			}
		});
	}
	
	/** \brief Index of first element matching identifier or -1 if absent. */
	inline int IndexOfWithId(const char *id) const{
		return this->IndexOfMatching([&](const T &e){
			if constexpr (requires { e.GetIdentifier(); }) {
				return e.GetIdentifier() == id;
			} else {
				return e.GetId() == id;
			}
		});
	}
	
	/** \brief First element matching identifier or nullptr if absent. */
	inline T *FindWithId(const char *id) const{
		return this->FindOrDefault([&](const T &e){
			if constexpr (requires { e.GetIdentifier(); }) {
				return e.GetIdentifier() == id;
			} else {
				return e.GetId() == id;
			}
		});
	}
};


class deObject;

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


class deThreadSafeObject;

/** \brief ThreadSafeObject comparator. */
using decThreadSafeObjectComparator = decTComparator<deThreadSafeObject*>;

/** \brief ThreadSafeObject visitor. */
using decThreadSafeObjectVisitor = decTVisitor<deThreadSafeObject*>;

/** \brief ThreadSafeObject evaluator. */
using decThreadSafeObjectEvaluator = decTEvaluator<deThreadSafeObject*>;

#endif
