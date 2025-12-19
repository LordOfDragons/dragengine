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

#ifndef _DECTUNIQUEORDEREDSET_H_
#define _DECTUNIQUEORDEREDSET_H_

#include "decTOrderedSet.h"

/**
 * \brief decTOrderedSet containing unique elements.
 * 
 * Allows only one element with the same unique property to be present in the set.
 * An evaluator is used to test for uniqueness (template type UE, unique condition type UT).
 */
template<typename T, typename UT, typename UE, typename TP = T>
class decTNamedOrderedSet : public decTOrderedSet<T,TP>{
public:
	/** \brief Uniqueness. */
	class TUnique{
	public:
		TUnique() = default;
		
		/**
		 * \brief Test for uniqueness.
		 * \param[in] set Set to test uniqueness against.
		 * \param[in] element Element to test for uniqueuess.
		 * \param[in] condition Uniqueness condition to test.
		 * \returns true if element matches criterias otherwise false.
		 */
		virtual bool operator() (const decTNamedOrderedSet &set, const T *element, UT &condition) = 0;
	};
	
	
	/** \name Management */
	/*@{*/
	/** \brief Unique element or nullptr if absent. */
	T GetUnique(UT condition) const{
		return this->FindDefault([&condition](const T &element){
			return element->GetName() == name;
		}, {});
	}
	
	/** \brief Index of named element or -1 if absent. */
	int IndexOfNamed(const char *name) const{
		const int count = this->GetCount();
		int i;
		
		for(i=0; i<count; i++){
			if(this->GetAt(i)->GetName() == name){
				return i;
			}
		}
		
		return -1;
	}
	
	/** \brief Named gesture is present. */
	bool HasNamed(const char *name) const{
		return GetNamed(name);
	}
	
	/**
	 * \brief Set element at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount() - 1.
	 * \throws deeInvalidParam \em element with same name is already present in the set.
	 */
	void SetAt(int index, const TP &element){
		if(this->GetAt(index)->GetName() != element->GetName()){
			DEASSERT_FALSE(HasNamed(element->GetName()))
		}
		
		decTOrderedSet<T,TP>::SetAt(index, element);
	}
	
	/**
	 * \brief Add element.
	 * \throws deeInvalidParam \em element is present in the set.
	 * \throws deeInvalidParam \em element with same name is already present in the set.
	 */
	void Add(const TP &element){
		DEASSERT_FALSE(HasNamed(element->GetName()))
		decTOrderedSet<T,TP>::Add(element);
	}
	
	/**
	 * \brief Add element if absent from the set.
	 * \throws deeInvalidParam \em element is present in the set.
	 * \throws deeInvalidParam \em element with same name is already present in the set.
	 * \returns true if added.
	 */
	bool AddIfAbsent(const TP &element){
		if(this->Has(element)){
			return false;
		}
		DEASSERT_FALSE(HasNamed(element->GetName()))
		return decTOrderedSet<T,TP>::Add(element);
	}
	
	/**
	 * \brief Insert element.
	 * \throws deeInvalidParam \em element is present in the set.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 * \throws deeInvalidParam \em element with same name is already present in the set.
	 */
	void Insert(const TP &element, int index){
		DEASSERT_FALSE(HasNamed(element->GetName()))
		decTOrderedSet<T,TP>::Insert(element, index);
	}
	/*@}*/
};



/**
 * \brief Named ordered object set template class.
 * 
 * All objects including nullptr are allowed. Objects can be included only once in the set.
 * 
 * This template uses deTObjectReference.
 */
template<typename T>
using decTNamedObjectOrderedSet = decTNamedOrderedSet<deTObjectReference<T>, T*>;

/**
 * \brief Named thread safe ordered object set template class.
 * 
 * All objects including nullptr are allowed. Objects can be included only once in the set.
 * 
 * This template uses deTThreadSafeObjectReference to get thread safe assignment of
 * object references. This does not make the set itself thread safe though.
 */
template<typename T>
using decTNamedThreadSafeObjectOrderedSet = decTNamedOrderedSet<deTThreadSafeObjectReference<T>, T*>;

#endif
