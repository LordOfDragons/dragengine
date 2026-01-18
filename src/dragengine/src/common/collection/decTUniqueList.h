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

#ifndef _DECTUNIQUELIST_H_
#define _DECTUNIQUELIST_H_

#include <algorithm>
#include <iterator>
#include <cstddef>
#include <utility>
#include <type_traits>

#include "decCollectionInterfaces.h"
#include "../math/decMath.h"
#include "../exceptions_reduced.h"
#include "../../deTypeTraits.h"
#include "../../deTUniqueReference.h"


/**
 * \brief Unique object list template class.
 * 
 * All objects including nullptr are allowed.
 * 
 * This template uses deTUniqueReference.
 */
template<typename T>
class decTUniqueList{
private:
	using Ref = deTUniqueReference<T>;
	using Ptr = T*;
	
	Ref* pElements;
	int pCount, pSize;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new list. */
	decTUniqueList() : pElements(nullptr), pCount(0), pSize(0){
	}
	
	/**
	 * \brief Create a new list with an initial capacity.
	 * \throws deeInvalidParam \em capacity is less than 0.
	 */
	template<typename U>
	requires(std::same_as<U, int>)
	explicit decTUniqueList(U capacity) : pElements(nullptr), pCount(0), pSize(0){
		DEASSERT_TRUE(capacity >= 0)
		if(capacity > 0){
			pElements = new Ref[capacity];
			pSize = capacity;
		}
	}
	
	decTUniqueList(const decTUniqueList& list) = delete;
	
	/** \brief Move list. */
	decTUniqueList(decTUniqueList&& list) noexcept : pElements(list.pElements), pCount(list.pCount), pSize(list.pSize){
		list.pElements = nullptr;
		list.pCount = 0;
		list.pSize = 0;
	}
	
	/** \brief Create list with content from variable count of arguments. */
	template<typename... A>
	requires(sizeof...(A) > 0 &&(std::convertible_to<A, Ref> && ...))
	explicit decTUniqueList(A&&... args) : pElements(nullptr), pCount(0), pSize(0){
		EnlargeCapacity(static_cast<int>(sizeof...(args)));
		(Add(std::forward<A>(args)), ...);
	}
	
	/** \brief Clean up the list. */
	~decTUniqueList() noexcept{
		RemoveAll();
		
		if(pElements){
			delete[] pElements;
		}
	}
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of elements. */
	inline int GetCount() const{
		return pCount;
	}
	
	/** \brief List is empty. */
	inline bool IsEmpty() const{
		return pCount == 0;
	}
	
	/** \brief List is not empty. */
	inline bool IsNotEmpty() const{
		return pCount > 0;
	}
	
	/**
	 * \brief Capacity of list.
	 */
	inline int GetCapacity() const{
		return pSize;
	}
	
	/**
	 * Enlarge capacity of list if smaller.
	 */
	void EnlargeCapacity(int capacity){
		if(capacity <= pSize){
			return;
		}
		
		Ref* const newArray = new Ref[capacity];
		if(pElements){
			int i;
			for(i = 0; i < pSize; i++){
				newArray[i] = std::move(pElements[i]);
			}
			delete[] pElements;
		}
		pElements = newArray;
		pSize = capacity;
	}
	
	/**
	 * \brief Element at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	const Ref& GetAt(int index) const{
		DEASSERT_TRUE(index >= 0)
		DEASSERT_TRUE(index < pCount)
		
		return pElements[index];
	}
	
	/**
	 * \brief Element at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	Ref& GetAt(int index){
		DEASSERT_TRUE(index >= 0)
		DEASSERT_TRUE(index < pCount)
		
		return pElements[index];
	}
	
	/**
	 * \brief First element.
	 * \throws deeInvalidParam if list is empty.
	 */
	inline Ref& First(){
		return GetAt(0);
	}
	
	inline const Ref& First() const{
		return GetAt(0);
	}
	
	/**
	 * \brief Last element.
	 * \throws deeInvalidParam if list is empty.
	 */
	inline Ref& Last(){
		return GetAt(pCount - 1);
	}
	
	inline const Ref& Last() const{
		return GetAt(pCount - 1);
	}
	
	/**
	 * \brief Set element at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void SetAt(int index, Ref&& element){
		DEASSERT_TRUE(index >= 0)
		DEASSERT_TRUE(index < pCount)
		pElements[index] = std::move(element);
	}
	
	/** \brief Index of the first occurance of an element or -1 if not found. */
	int IndexOf(const Ptr element) const{
		return IndexOf(element, 0);
	}
	
	/**
	 * \brief Index of the first occurance of an element or -1 if not found.
	 * \throws deeInvalidParam \em start is less than 0 or larger than GetCount().
	 */
	int IndexOf(const Ptr element, int start) const{
		DEASSERT_TRUE(start >= 0)
		DEASSERT_TRUE(start <= pCount)
		
		int i;
		for(i = start; i < pCount; i++){
			if(pElements[i] == element){
				return i;
			}
		}
		return -1;
	}
	
	/**
	 * \brief Index of first element match condition or -1 if not found.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T).
	 */
	template<typename Evaluator>
	int IndexOfMatching(Evaluator& evaluator) const{
		return IndexOfMatching<Evaluator>(evaluator, 0);
	}
	
	template<typename Evaluator>
	inline int IndexOfMatching(Evaluator&& evaluator) const{
		return IndexOfMatching<Evaluator>(evaluator);
	}
	
	/**
	 * \brief Index of first element match condition or -1 if not found.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T).
	 * \throws deeInvalidParam \em start is less than 0 or larger than GetCount().
	 */
	template<typename Evaluator>
	int IndexOfMatching(Evaluator& evaluator, int start) const{
		DEASSERT_TRUE(start >= 0)
		DEASSERT_TRUE(start <= pCount)

		int i;
		for(i = start; i < pCount; i++){
			if(evaluator(pElements[i])){
				return i;
			}
		}
		return -1;
	}
	
	template<typename Evaluator>
	inline int IndexOfMatching(Evaluator&& evaluator, int start) const{
		return IndexOfMatching<Evaluator>(evaluator, start);
	}
	
	/** \brief Determine if element exists in the list. */
	bool Has(const Ptr element) const{
		int p;
		
		for(p = 0; p < pCount; p++){
			if(pElements[p] == element){
				return true;
			}
		}
		
		return false;
	}
	
	/**
	 * \brief One or more elements match condition.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T).
	 */
	template<typename Evaluator>
	bool HasMatching(Evaluator& evaluator) const{
		const Ref* cfound = nullptr;
		return Find<Evaluator>(evaluator, cfound);
	}
	
	template<typename Evaluator>
	inline bool HasMatching(Evaluator&& evaluator) const{
		return HasMatching<Evaluator>(evaluator);
	}
	
	/**
	 * \brief All elements match condition.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T).
	 *
	 * If list is empty true is returned.
	 */
	template<typename Evaluator>
	bool AllMatching(Evaluator& evaluator) const{
		int i;
		for(i = 0; i < pCount; i++){
			if(!evaluator(pElements[i])){
				return false;
			}
		}
		return true;
	}
	
	template<typename Evaluator>
	inline bool AllMatching(Evaluator&& evaluator) const{
		return AllMatching<Evaluator>(evaluator);
	}
	
	/**
	 * \brief No elements match condition.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T).
	 *
	 * If list is empty true is returned.
	 */
	template<typename Evaluator>
	bool NoneMatching(Evaluator& evaluator) const{
		int i;
		for(i = 0; i < pCount; i++){
			if(evaluator(pElements[i])){
				return false;
			}
		}
		return true;
	}
	
	template<typename Evaluator>
	inline bool NoneMatching(Evaluator&& evaluator) const{
		return NoneMatching<Evaluator>(evaluator);
	}
	
	/** \brief Add element. */
	void Add(Ref&& element){
		if(pCount == pSize){
			int newSize = pSize * 3 / 2 + 1;
			Ref* const newArray = new Ref[newSize];
			if(pElements){
				int i;
				for(i = 0; i < pSize; i++){
					newArray[i] = std::move(pElements[i]);
				}
				delete[] pElements;
			}
			pElements = newArray;
			pSize = newSize;
		}
		
		pElements[pCount++] = std::move(element);
	}
	
	/**
	 * \brief Insert element.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void Insert(Ref&& element, int index){
		DEASSERT_TRUE(index >= 0)
		DEASSERT_TRUE(index <= pCount)
		
		if(pCount == pSize){
			int newSize = pSize * 3 / 2 + 1;
			Ref* const newArray = new Ref[newSize];
			if(pElements){
				int i;
				for(i = 0; i < pSize; i++){
					newArray[i] = std::move(pElements[i]);
				}
				delete[] pElements;
			}
			pElements = newArray;
			pSize = newSize;
		}
		
		int i;
		for(i = pCount; i > index; i--){
			pElements[i] = std::move(pElements[i - 1]);
		}
		pElements[index] = std::move(element);
		pCount++;
	}
	
	/**
	 * \brief Move element.
	 * \throws deeInvalidParam \em from is less than 0 or larger than GetCount()-1.
	 * \throws deeInvalidParam \em to is less than 0 or larger than GetCount().
	 */
	void Move(int from, int to){
		if(from < 0){
			from = pCount + from;
		}
		if(to < 0){
			to = pCount + to;
		}
		DEASSERT_TRUE(from >= 0)
		DEASSERT_TRUE(from < pCount)
		DEASSERT_TRUE(to >= 0)
		DEASSERT_TRUE(to <= pCount)
		
		if(from == to){
			return;
		}
		
		Ref tempElement = std::move(pElements[from]);
		int i;
		
		if(to < from){
			for(i = from; i > to; i--){
				pElements[i] = std::move(pElements[i - 1]);
			}
			pElements[to] = std::move(tempElement);
			
		}else{
			for(i = from; i < to - 1; i++){
				pElements[i] = std::move(pElements[i + 1]);
			}
			pElements[to - 1] = std::move(tempElement);
		}
	}
	
	/**
	 * \brief Swap elements at two indices.
	 * \throws deeInvalidParam \em from or \em to is less than 0 or larger than GetCount()-1.
	 */
	void Swap(int from, int to){
		if(from < 0){
			from = pCount + from;
		}
		if(to < 0){
			to = pCount + to;
		}
		DEASSERT_TRUE(from >= 0)
		DEASSERT_TRUE(from < pCount)
		DEASSERT_TRUE(to >= 0)
		DEASSERT_TRUE(to < pCount)
		
		if(from == to){
			return;
		}
		
		pElements[from].Swap(pElements[to]);
	}
	
	/**
	 * \brief Remove element.
	 * \throws deeInvalidParam \em element not found.
	 */
	void Remove(const Ptr element){
		RemoveFrom(IndexOf(element));
	}
	
	/**
	 * \brief Remove element from index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void RemoveFrom(int index){
		DEASSERT_TRUE(index >= 0)
		DEASSERT_TRUE(index < pCount)
		
		int i;
		for(i = index + 1; i < pCount; i++){
			pElements[i - 1] = std::move(pElements[i]);
		}
		pElements[--pCount].Clear();
	}
	
	/** \brief Remove all elements. */
	void RemoveAll(){
		while(pCount > 0){
			pElements[--pCount].Clear();
		}
	}
	
	
	/**
	 * \brief Extract element from index and return it (moved).
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	Ref ExtractFrom(int index){
		DEASSERT_TRUE(index >= 0)
		DEASSERT_TRUE(index < pCount)
		
		Ref result = std::move(pElements[index]);
		int i;
		for(i = index + 1; i < pCount; i++){
			pElements[i - 1] = std::move(pElements[i]);
		}
		pElements[--pCount].Clear();
		return result;
	}
	
	
	/**
	 * \brief Visit elements.
	 * \param[in] visitor Visitor callable invoked as visitor(T).
	 * \param[in] from First index to visit. Negative counts from end of list.
	 * \param[in] to One past last index to visit. Negative counts from end of list.
	 * \param[in] step Step size. Can be negative but not 0.
	 */
	template<typename Visitor>
	void Visit(Visitor& visitor, int from, int to, int step = 1) const{
		DEASSERT_TRUE(step != 0)
		
		if(from < 0){
			from = pCount + from;
		}
		DEASSERT_TRUE(from >= 0)
		
		if(to < 0){
			to = pCount + to;
		}
		DEASSERT_TRUE(to >= 0)
		
		int i;
		if(step > 0){
			DEASSERT_TRUE(to <= pCount)
			DEASSERT_TRUE(from <= pCount)
			
			for(i = from; i < to; i += step){
				visitor(pElements[i]);
			}
			
		}else{
			DEASSERT_TRUE(to < pCount)
			DEASSERT_TRUE(from < pCount)
			
			for(i = from; i >= to; i += step){
				visitor(pElements[i]);
			}
		}
	}
	
	template<typename Visitor>
	inline void Visit(Visitor&& visitor, int from, int to, int step = 1) const{
		Visit<Visitor>(visitor, from, to, step);
	}
	
	template<typename Visitor>
	void Visit(Visitor& visitor, int from) const{
		if(from < 0){
			from = pCount + from;
		}
		DEASSERT_TRUE(from >= 0)
		DEASSERT_TRUE(from <= pCount)
		
		int i;
		for(i = from; i < pCount; i++){
			visitor(pElements[i]);
		}
	}
	
	template<typename Visitor>
	inline void Visit(Visitor&& visitor, int from) const{
		Visit<Visitor>(visitor, from);
	}
	
	template<typename Visitor>
	void Visit(Visitor& visitor) const{
		int i;
		for(i = 0; i < pCount; i++){
			visitor(pElements[i]);
		}
	}
	
	template<typename Visitor>
	inline void Visit(Visitor&& visitor) const{
		Visit<Visitor>(visitor);
	}
	
	template<typename Visitor>
	void VisitReverse(Visitor& visitor) const{
		int i;
		for(i = pCount - 1; i >= 0; i--){
			visitor(pElements[i]);
		}
	}
	
	template<typename Visitor>
	inline void VisitReverse(Visitor&& visitor) const{
		VisitReverse<Visitor>(visitor);
	}
	
	
	/**
	 * \brief Visit elements with index.
	 * \param[in] visitor Visitor callable invoked as visitor(int,T).
	 * \param[in] from First index to visit. Negative counts from end of list.
	 * \param[in] to One past last index to visit. Negative counts from end of list.
	 * \param[in] step Step size. Can be negative but not 0.
	 */
	template<typename Visitor>
	void VisitIndexed(Visitor& visitor, int from, int to, int step = 1) const{
		DEASSERT_TRUE(step != 0)
		
		if(from < 0){
			from = pCount + from;
		}
		DEASSERT_TRUE(from >= 0)
		
		if(to < 0){
			to = pCount + to;
		}
		DEASSERT_TRUE(to >= 0)
		
		int i;
		if(step > 0){
			DEASSERT_TRUE(to <= pCount)
			DEASSERT_TRUE(from <= pCount)
			
			for(i = from; i < to; i += step){
				visitor(i, pElements[i]);
			}
			
		}else{
			DEASSERT_TRUE(to < pCount)
			DEASSERT_TRUE(from < pCount)
			
			for(i = from; i >= to; i += step){
				visitor(i, pElements[i]);
			}
		}
	}
	
	template<typename Visitor>
	inline void VisitIndexed(Visitor&& visitor, int from, int to, int step = 1) const{
		VisitIndexed<Visitor>(visitor, from, to, step);
	}
	
	template<typename Visitor>
	void VisitIndexed(Visitor& visitor, int from) const{
		if(from < 0){
			from = pCount + from;
		}
		DEASSERT_TRUE(from >= 0)
		DEASSERT_TRUE(from <= pCount)
		
		int i;
		for(i = from; i < pCount; i++){
			visitor(i, pElements[i]);
		}
	}
	
	template<typename Visitor>
	inline void VisitIndexed(Visitor&& visitor, int from) const{
		VisitIndexed<Visitor>(visitor, from);
	}
	
	template<typename Visitor>
	void VisitIndexed(Visitor& visitor) const{
		int i;
		for(i = 0; i < pCount; i++){
			visitor(i, pElements[i]);
		}
	}
	
	template<typename Visitor>
	inline void VisitIndexed(Visitor&& visitor) const{
		VisitIndexed<Visitor>(visitor);
	}
	
	template<typename Visitor>
	void VisitReverseIndexed(Visitor& visitor) const{
		int i;
		for(i = pCount - 1; i >= 0; i--){
			visitor(i, pElements[i]);
		}
	}
	
	template<typename Visitor>
	inline void VisitReverseIndexed(Visitor&& visitor) const{
		VisitReverseIndexed<Visitor>(visitor);
	}
	
	
	/**
	 * \brief Find element.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T).
	 * \param[out] found Found element if true is returned.
	 * \param[in] from First index to visit. Negative counts from end of list.
	 * \param[in] to One past last index to visit. Negative counts from end of list.
	 * \param[in] step Step size. Can be negative but not 0.
	 */
	template<typename Evaluator>
	bool Find(Evaluator& evaluator, const Ref*& found, int from, int to, int step = 1) const{
		DEASSERT_TRUE(step != 0)
		
		if(from < 0){
			from = pCount + from;
		}
		DEASSERT_TRUE(from >= 0)
		
		if(to < 0){
			to = pCount + to;
		}
		DEASSERT_TRUE(to >= 0)
		
		int i;
		if(step > 0){
			DEASSERT_TRUE(to <= pCount)
			DEASSERT_TRUE(from <= pCount)
			
			for(i = from; i < to; i += step){
				if(evaluator(pElements[i])){
					found = &pElements[i];
					return true;
				}
			}
			
		}else{
			DEASSERT_TRUE(to < pCount)
			DEASSERT_TRUE(from < pCount)
			
			for(i = from; i >= to; i += step){
				if(evaluator(pElements[i])){
					found = &pElements[i];
					return true;
				}
			}
		}
		return false;
	}
	
	template<typename Evaluator>
	bool Find(Evaluator& evaluator, Ref*& found, int from, int to, int step = 1){
		const Ref* cfound = nullptr;
		const bool result = Find<Evaluator>(evaluator, cfound, from, to, step);
		found = const_cast<Ref*>(cfound);
		return result;
	}
	
	template<typename Evaluator>
	inline bool Find(Evaluator&& evaluator, const Ref*& found, int from, int to, int step = 1) const{
		return Find<Evaluator>(evaluator, found, from, to, step);
	}
	
	template<typename Evaluator>
	inline bool Find(Evaluator&& evaluator, Ref*& found, int from, int to, int step = 1){
		return Find<Evaluator>(evaluator, found, from, to, step);
	}
	
	template<typename Evaluator>
	bool Find(Evaluator& evaluator, const Ref*& found, int from) const{
		if(from < 0){
			from = pCount + from;
		}
		DEASSERT_TRUE(from >= 0)
		DEASSERT_TRUE(from <= pCount)
		
		int i;
		for(i = from; i < pCount; i++){
			if(evaluator(pElements[i])){
				found = &pElements[i];
				return true;
			}
		}
		return false;
	}
	
	template<typename Evaluator>
	bool Find(Evaluator& evaluator, Ref*& found, int from){
		const Ref* cfound = nullptr;
		const bool result = Find<Evaluator>(evaluator, cfound, from);
		found = const_cast<Ref*>(cfound);
		return result;
	}
	
	template<typename Evaluator>
	inline bool Find(Evaluator&& evaluator, const Ref*& found, int from) const{
		return Find<Evaluator>(evaluator, found, from);
	}
	
	template<typename Evaluator>
	inline bool Find(Evaluator&& evaluator, Ref*& found, int from){
		return Find<Evaluator>(evaluator, found, from);
	}
	
	template<typename Evaluator>
	inline bool Find(Evaluator& evaluator, const Ref*& found) const{
		int i;
		for(i = 0; i < pCount; i++){
			if(evaluator(pElements[i])){
				found = &pElements[i];
				return true;
			}
		}
		return false;
	}
	
	template<typename Evaluator>
	inline bool Find(Evaluator& evaluator, Ref*& found){
		const Ref* cfound = nullptr;
		const bool result = Find<Evaluator>(evaluator, cfound);
		found = const_cast<Ref*>(cfound);
		return result;
	}
	
	template<typename Evaluator>
	inline bool Find(Evaluator&& evaluator, const Ref*& found) const{
		return Find<Evaluator>(evaluator, found);
	}
	
	template<typename Evaluator>
	inline bool Find(Evaluator&& evaluator, Ref*& found){
		return Find<Evaluator>(evaluator, found);
	}
	
	template<typename Evaluator>
	inline bool FindReverse(Evaluator& evaluator, const Ref*& found) const{
		int i;
		for(i = pCount - 1; i >= 0; i--){
			if(evaluator(pElements[i])){
				found = &pElements[i];
				return true;
			}
		}
		return false;
	}
	
	template<typename Evaluator>
	inline bool FindReverse(Evaluator& evaluator, Ref*& found){
		const Ref* cfound = nullptr;
		const bool result = FindReverse<Evaluator>(evaluator, cfound);
		found = const_cast<Ref*>(cfound);
		return result;
	}
	
	template<typename Evaluator>
	inline bool FindReverse(Evaluator&& evaluator, const Ref*& found) const{
		return FindReverse<Evaluator>(evaluator, found);
	}
	
	template<typename Evaluator>
	inline bool FindReverse(Evaluator&& evaluator, Ref*& found){
		return FindReverse<Evaluator>(evaluator, found);
	}
	
	/**
	 * \brief Find element or nullptr if not found.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T).
	 * \param[in] from First index to visit. Negative counts from end of list.
	 * \param[in] to One past last index to visit. Negative counts from end of list.
	 * \param[in] step Step size. Can be negative but not 0.
	 */
	template<typename Evaluator>
	Ptr FindOrNull(Evaluator& evaluator, int from, int to, int step = 1) const{
		const Ref* found = nullptr;
		return Find<Evaluator>(evaluator, found, from, to, step) ? found->Pointer() : nullptr;
	}
	
	template<typename Evaluator>
	inline Ptr FindOrNull(Evaluator&& evaluator, int from, int to, int step = 1) const{
		return FindOrNull<Evaluator>(evaluator, from, to, step);
	}
	
	template<typename Evaluator>
	Ptr FindOrNull(Evaluator& evaluator, int from) const{
		const Ref* found = nullptr;
		return Find<Evaluator>(evaluator, found, from) ? found->Pointer() : nullptr;
	}
	
	template<typename Evaluator>
	inline Ptr FindOrNull(Evaluator&& evaluator, int from) const{
		return FindOrNull<Evaluator>(evaluator, from);
	}
	
	template<typename Evaluator>
	Ptr FindOrNull(Evaluator& evaluator) const{
		const Ref* found = nullptr;
		return Find<Evaluator>(evaluator, found) ? found->Pointer() : nullptr;
	}
	
	template<typename Evaluator>
	inline Ptr FindOrNull(Evaluator&& evaluator) const{
		return FindOrNull<Evaluator>(evaluator);
	}
	
	template<typename Evaluator>
	Ptr FindReverseOrNull(Evaluator& evaluator) const{
		const Ref* found = nullptr;
		return FindReverse<Evaluator>(evaluator, found) ? found->Pointer() : nullptr;
	}
	
	template<typename Evaluator>
	inline Ptr FindReverseOrNull(Evaluator&& evaluator) const{
		return FindReverseOrNull<Evaluator>(evaluator);
	}
	
	
	/**
	 * \brief Inject(reduce) elements using a combiner starting with initial value.
	 * \param[in] value Initial value.
	 * \param[in] combiner Combiner callable invoked as combiner(accumulator, element) -> accumulator.
	 */
	template<typename R, typename Combiner>
	R Inject(const R& value, Combiner& combiner, int from, int to, int step = 1) const{
		DEASSERT_TRUE(step != 0)
		
		if(from < 0){
			from = pCount + from;
		}
		DEASSERT_TRUE(from >= 0)
		
		if(to < 0){
			to = pCount + to;
		}
		DEASSERT_TRUE(to >= 0)
		
		R acc = value;
		int i;
		if(step > 0){
			DEASSERT_TRUE(to <= pCount)
				DEASSERT_TRUE(from <= pCount)
				for(i = from; i < to; i += step){
					acc = combiner(acc, pElements[i]);
				}
				
		}else{
			DEASSERT_TRUE(to < pCount)
				DEASSERT_TRUE(from < pCount)
				for(i = from; i >= to; i += step){
					acc = combiner(acc, pElements[i]);
				}
		}
		return acc;
	}
	
	template<typename R, typename Combiner>
	inline R Inject(const R& value, Combiner&& combiner, int from, int to, int step = 1) const{
		return Inject<R, Combiner>(value, combiner, from, to, step);
	}
	
	template<typename R, typename Combiner>
	R Inject(const R& value, Combiner& combiner, int from) const{
		return Inject<R, Combiner>(value, combiner, from, pCount);
	}
	
	template<typename R, typename Combiner>
	inline R Inject(const R& value, Combiner&& combiner, int from) const{
		return Inject<R, Combiner>(value, combiner, from);
	}
	
	template<typename R, typename Combiner>
	R Inject(const R& value, Combiner& combiner) const{
		R acc = value;
		int i;
		for(i = 0; i < pCount; i++){
			acc = combiner(acc, pElements[i]);
		}
		return acc;
	}
	
	template<typename R, typename Combiner>
	inline R Inject(const R& value, Combiner&& combiner) const{
		return Inject<R, Combiner>(value, combiner);
	}
	
	
	/**
	 * \brief Reverse order of elements.
	 */
	void Reverse(){
		int i;
		for(i = 0; i < pCount / 2; i++){
			pElements[i].Swap(pElements[pCount - 1 - i]);
		}
	}
	
	
	/**
	 * \brief Remove elements matching condition.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T).
	 * \param[in] from First index to visit. Negative counts from end of list.
	 * \param[in] to One past last index to visit. Negative counts from end of list.
	 * \param[in] step Step size. Can be negative but not 0.
	 */
	template<typename Evaluator>
	void RemoveIf(Evaluator& evaluator, int from, int to, int step = 1){
		DEASSERT_TRUE(step != 0)
		
		if(from < 0){
			from = pCount + from;
		}
		DEASSERT_TRUE(from >= 0)
		
		if(to < 0){
			to = pCount + to;
		}
		DEASSERT_TRUE(to >= 0)
		
		int i;
		if(step > 0){
			DEASSERT_TRUE(to <= pCount)
			DEASSERT_TRUE(from <= pCount)
			
			for(i = from; i < to; i += step){
				if(evaluator(pElements[i])){
					RemoveFrom(i);
					i--;
					to--;
				}
			}
			
		}else{
			DEASSERT_TRUE(to < pCount)
			DEASSERT_TRUE(from < pCount)
			
			for(i = from; i >= to; i += step){
				if(evaluator(pElements[i])){
					RemoveFrom(i);
					i++;
					to++;
				}
			}
		}
	}
	
	template<typename Evaluator>
	inline void RemoveIf(Evaluator&& evaluator, int from, int to, int step = 1){
		RemoveIf<Evaluator>(evaluator, from, to, step);
	}
	
	template<typename Evaluator>
	void RemoveIf(Evaluator& evaluator, int from){
		if(from < 0){
			from = pCount + from;
		}
		DEASSERT_TRUE(from >= 0)
		DEASSERT_TRUE(from <= pCount)
		
		int i;
		for(i = from; i < pCount; i++){
			if(evaluator(pElements[i])){
				RemoveFrom(i);
				i--;
			}
		}
	}
	
	template<typename Evaluator>
	inline void RemoveIf(Evaluator&& evaluator, int from){
		RemoveIf<Evaluator>(evaluator, from);
	}
	
	template<typename Evaluator>
	void RemoveIf(Evaluator& evaluator){
		int i, last = 0;
		for(i = 0; i < pCount; i++){
			if(evaluator(pElements[i])){
				continue;
			}
			
			if(last < i){
				pElements[last] = std::move(pElements[i]);
			}
			last++;
		}
		while(pCount > last){
			pElements[--pCount].Clear();
		}
	}
	
	template<typename Evaluator>
	inline void RemoveIf(Evaluator&& evaluator){
		RemoveIf<Evaluator>(evaluator);
	}
	
	
	/** \brief Sort elements in place. */
	template<typename Comparator>
	void Sort(Comparator& comparator){
		if(pCount > 1){
			pSort<Comparator>(comparator, 0, pCount - 1);
		}
	}
	
	template<typename Comparator>
	inline void Sort(Comparator&& comparator){
		Sort<Comparator>(comparator);
	}
	
	/** \brief Sort using decAscendingComparator. */
	void SortAscending(){
		if constexpr(requires(const T & a, const T & b){{ DECompare(a, b) } -> std::convertible_to<int>; }){
			Sort<decAscendingComparator<T>>(decAscendingComparator<T>());
			
		}else{
			DEThrowInvalidParam(__FILE__, __LINE__, DE_CUR_FUNC_NAME);
		}
	}
	
	/** \brief Sort using decDescendingComparator. */
	void SortDescending(){
		if constexpr(requires(const T & a, const T & b){{ DECompare(a, b) } -> std::convertible_to<int>; }){
			Sort<decDesendingComparator<T>>(decDesendingComparator<T>());
			
		}else{
			DEThrowInvalidParam(__FILE__, __LINE__, DE_CUR_FUNC_NAME);
		}
	}
	
	
	/**
	 * \brief Swap content with another list.
	 * \note This also swaps the capacities.
	 */
	void Swap(decTUniqueList& list){
		if(&list == this){
			return;
		}
		
		Ref* tempElements = pElements;
		int tempCount = pCount;
		int tempSize = pSize;
		
		pElements = list.pElements;
		pCount = list.pCount;
		pSize = list.pSize;
		
		list.pElements = tempElements;
		list.pCount = tempCount;
		list.pSize = tempSize;
	}
	/*@}*/
	
	
	/** \name Operators */
	/*@{*/
	bool operator==(const decTUniqueList& list) const = delete;
	
	/**
	 * \brief Element at index.
	 *
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	const Ref& operator[](int index) const{
		return GetAt(index);
	}
	
	/**
	 * \brief Element at index.
	 *
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	Ref& operator[](int index){
		return GetAt(index);
	}
	
	decTUniqueList& operator=(const decTUniqueList& list) = delete;
	
	/** \brief Move list. */
	decTUniqueList& operator=(decTUniqueList&& list){
		if(&list == this){
			return *this;
		}
		
		RemoveAll();
		
		if(pElements){
			delete[] pElements;
		}
		
		pElements = list.pElements;
		pCount = list.pCount;
		pSize = list.pSize;
		
		list.pElements = nullptr;
		list.pCount = 0;
		list.pSize = 0;
		
		return *this;
	}
	/*@}*/
	
	
	/** \name Standard library iterators. */
	/*@{*/
	class const_iterator{
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = Ref;
		using difference_type = std::ptrdiff_t;
		using pointer = const Ptr;
		using reference = const Ref&;
		
		explicit const_iterator(const decTUniqueList* owner = nullptr, int index = 0) :
			pOwner(owner), pIndex(index){
		}
		
		reference operator*() const{
			return pOwner->GetAt(pIndex);
		}
		
		pointer operator->() const{
			return &pOwner->GetAt(pIndex);
		}
		
		const_iterator& operator++(){
			pIndex++;
			return *this;
		}
		
		const_iterator operator++(int){
			const_iterator tmp = *this;
			pIndex++;
			return tmp;
		}
		
		const_iterator& operator--(){
			pIndex--;
			return *this;
		}
		
		const_iterator operator--(int){
			const_iterator tmp = *this;
			pIndex--;
			return tmp;
		}
		
		const_iterator& operator+=(difference_type n){
			pIndex += static_cast<int>(n);
			return *this;
		}
		
		const_iterator& operator-=(difference_type n){
			pIndex -= static_cast<int>(n);
			return *this;
		}
		
		const_iterator operator+(difference_type n) const{
			return const_iterator(pOwner, pIndex + static_cast<int>(n));
		}
		
		const_iterator operator-(difference_type n) const{
			return const_iterator(pOwner, pIndex - static_cast<int>(n));
		}
		
		difference_type operator-(const const_iterator& other) const{
			return static_cast<difference_type>(pIndex - other.pIndex);
		}
		
		reference operator[](difference_type n) const{
			return pOwner->GetAt(pIndex + static_cast<int>(n));
		}
		
		bool operator==(const const_iterator& o) const{
			return pOwner == o.pOwner && pIndex == o.pIndex;
		}
		
		bool operator!=(const const_iterator& o) const{
			return !(*this == o);
		}
		
		bool operator<(const const_iterator& o) const{
			return pIndex < o.pIndex;
		}
		
		bool operator<=(const const_iterator& o) const{
			return pIndex <= o.pIndex;
		}
		
		bool operator>(const const_iterator& o) const{
			return pIndex > o.pIndex;
		}
		
		bool operator>=(const const_iterator& o) const{
			return pIndex >= o.pIndex;
		}
		
	private:
		const decTUniqueList* pOwner;
		int pIndex;
	};
	
	class iterator{
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = Ref;
		using difference_type = std::ptrdiff_t;
		using pointer = Ptr;
		using reference = Ref&;
		
		explicit iterator(decTUniqueList* owner = nullptr, int index = 0) :
			pOwner(owner), pIndex(index){
		}
		
		reference operator*() const{
			return pOwner->GetAt(pIndex);
		}
		
		pointer operator->() const{
			return &pOwner->GetAt(pIndex);
		}
		
		iterator& operator++(){
			pIndex++;
			return *this;
		}
		
		iterator operator++(int){
			iterator tmp = *this;
			pIndex++;
			return tmp;
		}
		
		iterator& operator--(){
			pIndex--;
			return *this;
		}
		
		iterator operator--(int){
			iterator tmp = *this;
			pIndex--;
			return tmp;
		}
		
		iterator& operator+=(difference_type n){
			pIndex += static_cast<int>(n);
			return *this;
		}
		
		iterator& operator-=(difference_type n){
			pIndex -= static_cast<int>(n);
			return *this;
		}
		
		iterator operator+(difference_type n) const{
			return iterator(pOwner, pIndex + static_cast<int>(n));
		}
		
		iterator operator-(difference_type n) const{
			return iterator(pOwner, pIndex - static_cast<int>(n));
		}
		
		difference_type operator-(const iterator& other) const{
			return static_cast<difference_type>(pIndex - other.pIndex);
		}
		
		reference operator[](difference_type n) const{
			return pOwner->GetAt(pIndex + static_cast<int>(n));
		}
		
		bool operator==(const iterator& o) const{
			return pOwner == o.pOwner && pIndex == o.pIndex;
		}
		
		bool operator!=(const iterator& o) const{
			return !(*this == o);
		}
		
		bool operator<(const iterator& o) const{
			return pIndex < o.pIndex;
		}
		
		bool operator<=(const iterator& o) const{
			return pIndex <= o.pIndex;
		}
		
		bool operator>(const iterator& o) const{
			return pIndex > o.pIndex;
		}
		
		bool operator>=(const iterator& o) const{
			return pIndex >= o.pIndex;
		}
		
	private:
		decTUniqueList* pOwner;
		int pIndex;
	};
	
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	
	/** Standard library iterator pointing at first element in list */
	iterator begin(){
		return iterator(this, 0);
	}
	const_iterator begin() const{
		return cbegin();
	}
	const_iterator cbegin() const{
		return const_iterator(this, 0);
	}
	
	/** Standard library iterator pointing at one past last element in list */
	iterator end(){
		return iterator(this, pCount);
	}
	const_iterator end() const{
		return cend();
	}
	const_iterator cend() const{
		return const_iterator(this, pCount);
	}
	
	/** Standard library reverse iterator pointing at one past last element in list */
	reverse_iterator rbegin(){
		return reverse_iterator(end());
	}
	const_reverse_iterator rbegin() const{
		return const_reverse_iterator(end());
	}
	const_reverse_iterator crbegin() const{
		return const_reverse_iterator(cend());
	}
	
	/** Standard library reverse iterator pointing at first element in list */
	reverse_iterator rend(){
		return reverse_iterator(begin());
	}
	const_reverse_iterator rend() const{
		return const_reverse_iterator(begin());
	}
	const_reverse_iterator crend() const{
		return const_reverse_iterator(cbegin());
	}
	/*@}*/
	
	
private:
	template<typename Comparator>
	void pSort(Comparator& comparator, int left, int right){
		Ref pivot = std::move(pElements[left]);
		const int r_hold = right;
		const int l_hold = left;
		
		while(left < right){
			while(left < right && comparator(pElements[right], pivot) >= 0){
				right--;
			}
			if(left != right){
				pElements[left] = std::move(pElements[right]);
				left++;
			}
			while(left < right && comparator(pElements[left], pivot) <= 0){
				left++;
			}
			if(left != right){
				pElements[right] = std::move(pElements[left]);
				right--;
			}
		}
		
		pElements[left] = std::move(pivot);
		if(l_hold < left){
			pSort<Comparator>(comparator, l_hold, left - 1);
		}
		if(r_hold > left){
			pSort<Comparator>(comparator, left + 1, r_hold);
		}
	}
};

#endif
