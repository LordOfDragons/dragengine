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

#ifndef _DECTLIST_H_
#define _DECTLIST_H_

#include <algorithm>
#include <iterator>
#include <cstddef>
#include <utility>
#include <type_traits>

#include "decCollectionInterfaces.h"
#include "../math/decMath.h"
#include "../exceptions_reduced.h"
#include "../../deTypeTraits.h"
#include "../../deTObjectReference.h"
#include "../../threading/deTThreadSafeObjectReference.h"


/**
 * \brief List template class.
 * 
 * All elements including default constructed values are allowed. Elements can be included
 * multiple times in the list.
 */
template<typename T, typename TP = T>
class decTList{
private:
	T *pElements;
	int pCount, pSize;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new list. */
	decTList() : pElements(nullptr), pCount(0), pSize(0){
	}
	
	/**
	 * \brief Create a new list with an initial capacity.
	 * \throws deeInvalidParam \em capacity is less than 0.
	 */
	template<typename U>
	requires (std::same_as<U, int>)
	explicit decTList(U capacity) : pElements(nullptr), pCount(0), pSize(0){
		DEASSERT_TRUE(capacity >= 0)
		
		if(capacity > 0){
			pElements = new T[capacity];
			pSize = capacity;
		}
	}
	
	/**
	 * \brief Create a new list with an initial count of elements all set to element.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	template<typename U>
	requires (std::same_as<U, int>)
	decTList(U count, const std::type_identity_t<T> &element) : pElements(nullptr), pCount(0), pSize(0){
		DEASSERT_TRUE(count >= 0)
		
		if(count > 0){
			pElements = new T[count];
			pSize = count;
			std::fill_n(pElements, count, element);
			pCount = count;
		}
	}
	
	/** \brief Create a copy of a list. */
	decTList(const decTList &list) : pElements(nullptr), pCount(0), pSize(0){
		if(list.pCount == 0){
			return;
		}
		
		pElements = new T[list.pCount];
		pSize = list.pCount;
		
		std::copy_n(list.pElements, list.pCount, pElements);
		pCount = list.pCount;
	}
	
	/** \brief Move list. */
	decTList(decTList &&list) noexcept : pElements(list.pElements), pCount(list.pCount), pSize(list.pSize){
		list.pElements = nullptr;
		list.pCount = 0;
		list.pSize = 0;
	}
	
	/** \brief Create list with content provided by iterators. */
	template <std::input_iterator InputIt>
	decTList(InputIt first, InputIt last) : pElements(nullptr), pCount(0), pSize(0){
		for(; first != last; ++first){
			Add(*first);
		}
	}
	
	/** \brief Create list with content from another collection. */
	template<typename C>
	explicit decTList(const C &collection,
		typename std::enable_if_t<
			std::is_same_v<decltype(std::declval<C>().cbegin()),
				decltype(std::declval<C>().cbegin())>, int> = 0)
	: pElements(nullptr), pCount(0), pSize(0){
		auto first = collection.cbegin();
		auto last = collection.cend();
		for(; first != last; ++first){
			Add(*first);
		}
	}
	
	/** \brief Create list with content from variable count of arguments. */
	template<typename... A>
	explicit decTList(de_variadic_constructor_tag_t, A&&... args) : pElements(nullptr), pCount(0), pSize(0){
		EnlargeCapacity(static_cast<int>(sizeof...(args)));
		(Add(std::forward<A>(args)), ...);
	}
	
	/** \brief Clean up the list. */
	~decTList() noexcept{
		if(pElements){
			delete [] pElements;
		}
	}
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of elements. */
	inline int GetCount() const{
		return pCount;
	}
	
	/**
	 * \brief Set number of elements.
	 * 
	 * If new count is less than current count calls RemoveTail(count - newCount).
	 * If new count is larger than current count calls AddRange(newCount - currentCount, element).
	 * If new count is equal to current count does nothing.
	 */
	void SetCount(int count, const TP &element){
		DEASSERT_TRUE(count >= 0)
		
		if(count < pCount){
			RemoveTail(pCount - count);
			
		}else if(count > pCount){
			AddRange(count - pCount, element);
		}
	}
	
	/**
	 * \brief Set number of elements if less than or equal to the current count.
	 * 
	 * If new count is less than current count calls RemoveTail(count - newCount).
	 * If new count is equal to current count does nothing.
	 * 
	 * \throws deeInvalidParam \em count is less than 0 or larger than GetCount().
	 */
	void SetCount(int count){
		DEASSERT_TRUE(count >= 0)
		DEASSERT_TRUE(count <= pCount)
		
		if(count < pCount){
			RemoveTail(pCount - count);
		}
	}
	
	/**
	 * \brief Set number of elements without retaining content.
	 * 
	 * For use in performance critical situations where the content after changing the count
	 * is not required to be preserved. In particular this means removed elements are not
	 * cleared and new elements are not reset to a known state. Typical use cases are
	 * "SetCountDiscard(0)" to fast clear a list and "SetCountDiscard(count)" to alter the
	 * count of elements in a buffer afterwards fully refilled with new content.
	 */
	void SetCountDiscard(int count){
		DEASSERT_TRUE(count >= 0)
		
		if(count > pSize){
			EnlargeCapacityDiscard(count);
		}
		pCount = count;
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
	inline int GetCapacity() const {
		return pSize;
	}
	
	/**
	 * \brief Enlarge capacity of list if smaller.
	 */
	void EnlargeCapacity(int capacity){
		if(capacity <= pSize){
			return;
		}
		
		T * const newArray = new T[capacity];
		if(pElements){
			std::move(pElements, pElements + pSize, newArray);
			delete [] pElements;
		}
		pElements = newArray;
		pSize = capacity;
	}
	
	/**
	 * \brief Enlarge capacity of list if smaller without retaining content.
	 * 
	 * For use in performance critical situations where the content after changing the capacity
	 * is not required to be preserved.
	 */
	void EnlargeCapacityDiscard(int capacity){
		if(capacity <= pSize){
			return;
		}
		
		T * const newArray = new T[capacity];
		if(pElements){
			delete [] pElements;
		}
		pElements = newArray;
		pSize = capacity;
	}
	
	/**
	 * \brief Element at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	const T &GetAt(int index) const{
		DEASSERT_TRUE(index >= 0)
		DEASSERT_TRUE(index < pCount)
		
		return pElements[index];
	}
	
	/**
	 * \brief Element at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	T &GetAt(int index){
		DEASSERT_TRUE(index >= 0)
		DEASSERT_TRUE(index < pCount)
		
		return pElements[index];
	}
	
	/**
	 * \brief First element.
	 * \throws deeInvalidParam if list is empty.
	 */
	inline T &First(){
		return GetAt(0);
	}
	
	inline const T &First() const{
		return GetAt(0);
	}
	
	/**
	 * \brief Last element.
	 * \throws deeInvalidParam if list is empty.
	 */
	inline T &Last(){
		return GetAt(pCount - 1);
	}
	
	inline const T &Last() const{
		return GetAt(pCount - 1);
	}
	
	/**
	 * \brief Set element at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void SetAt(int index, const TP &element){
		DEASSERT_TRUE(index >= 0)
		DEASSERT_TRUE(index < pCount)
		pElements[index] = element;
	}
	
	template<typename U = T>
	requires (!std::is_lvalue_reference_v<U>)
	void SetAt(int index, T &&element){
		DEASSERT_TRUE(index >= 0)
		DEASSERT_TRUE(index < pCount)
		pElements[index] = std::move(element);
	}
	
	/**
	 * \brief Set range of elements starting at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-count.
	 */
	void SetRangeAt(int index, int count, const TP &element){
		DEASSERT_TRUE(index >= 0)
		DEASSERT_TRUE(index + count <= pCount)
		
		std::fill_n(pElements + index, count, element);
	}
	
	void SetRangeAt(int index, const decTList &list){
		DEASSERT_TRUE(index >= 0)
		DEASSERT_TRUE(index + list.pCount <= pCount)
		
		std::copy_n(list.pElements, list.pCount, pElements + index);
	}
	
	/**
	 * \brief Set list to count elements of same value.
	 */
	void SetAll(int count, const TP &element){
		DEASSERT_TRUE(count >= 0)
		
		RemoveAll();
		EnlargeCapacity(count);
		std::fill_n(pElements, count, element);
		pCount = count;
	}
	
	/** \brief Index of the first occurance of an element or -1 if not found. */
	int IndexOf(const TP &element) const{
		return IndexOf(element, 0);
	}
	
	template<typename U = T>
	requires (!std::same_as<U, TP>)
	int IndexOf(const T &element) const{
		return IndexOf(element, 0);
	}
	
	/**
	 * \brief Index of the first occurance of an element or -1 if not found.
	 * \throws deeInvalidParam \em start is less than 0 or larger than GetCount().
	 */
	int IndexOf(const TP &element, int start) const{
		DEASSERT_TRUE(start >= 0)
		DEASSERT_TRUE(start <= pCount)
		
		int i;
		for(i=start; i<pCount; i++){
			if(pElements[i] == element){
				return i;
			}
		}
		return -1;
	}
	
	template<typename U = T>
	requires (!std::same_as<U, TP>)
	int IndexOf(const T &element, int start) const{
		DEASSERT_TRUE(start >= 0)
		DEASSERT_TRUE(start <= pCount)
		
		int i;
		for(i=start; i<pCount; i++){
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
	int IndexOfMatching(Evaluator &evaluator) const{
		return IndexOfMatching<Evaluator>(0, evaluator);
	}
	
	template<typename Evaluator>
	inline int IndexOfMatching(Evaluator &&evaluator) const{
		return IndexOfMatching<Evaluator>(evaluator);
	}
	
	/**
	 * \brief Index of first element match condition or -1 if not found.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T).
	 * \throws deeInvalidParam \em start is less than 0 or larger than GetCount().
	 */
	template<typename Evaluator>
	int IndexOfMatching(int start, Evaluator &evaluator) const{
		DEASSERT_TRUE(start >= 0)
		DEASSERT_TRUE(start <= pCount)
		
		int i;
		for(i=start; i<pCount; i++){
			if(evaluator(pElements[i])){
				return i;
			}
		}
		return -1;
	}
	
	template<typename Evaluator>
	inline int IndexOfMatching(int start, Evaluator &&evaluator) const{
		return IndexOfMatching<Evaluator>(start, evaluator);
	}
	
	/** \brief Determine if element exists in the list. */
	bool Has(const TP &element) const{
		int p;
		
		for(p=0; p<pCount; p++){
			if(pElements[p] == element){
				return true;
			}
		}
		
		return false;
	}
	
	template<typename U = T>
	requires (!std::same_as<U, TP>)
	bool Has(const T &element) const{
		int p;
		
		for(p=0; p<pCount; p++){
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
	bool HasMatching(Evaluator &evaluator) const{
		const T *f;
		return Find<Evaluator>(f, evaluator);
	}
	
	template<typename Evaluator>
	inline bool HasMatching(Evaluator &&evaluator) const{
		return HasMatching<Evaluator>(evaluator);
	}
	
	/**
	 * \brief All elements match condition.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T).
	 * 
	 * If list is empty true is returned.
	 */
	template<typename Evaluator>
	bool AllMatching(Evaluator &evaluator) const{
		int i;
		for(i=0; i<pCount; i++){
			if(!evaluator(pElements[i])){
				return false;
			}
		}
		return true;
	}
	
	template<typename Evaluator>
	inline bool AllMatching(Evaluator &&evaluator) const{
		return AllMatching<Evaluator>(evaluator);
	}
	
	/**
	 * \brief No elements match condition.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T).
	 * 
	 * If list is empty true is returned.
	 */
	template<typename Evaluator>
	bool NoneMatching(Evaluator &evaluator) const{
		int i;
		for(i=0; i<pCount; i++){
			if(evaluator(pElements[i])){
				return false;
			}
		}
		return true;
	}
	
	template<typename Evaluator>
	inline bool NoneMatching(Evaluator &&evaluator) const{
		return NoneMatching<Evaluator>(evaluator);
	}
	
	/** \brief Number of times element exists in the list. */
	int CountOccurance(const TP &element) const{
		int count = 0;
		int p;
		
		for(p=0; p<pCount; p++){
			if(pElements[p] == element){
				count++;
			}
		}
		
		return count;
	}
	
	template<typename U = T>
	requires (!std::same_as<U, TP>)
	int CountOccurance(const T &element) const{
		int count = 0;
		int p;
		
		for(p=0; p<pCount; p++){
			if(pElements[p] == element){
				count++;
			}
		}
		
		return count;
	}
	
	/** \brief Add element. */
	void Add(const TP &element){
		if(pCount == pSize){
			int newSize = pSize * 3 / 2 + 1;
			T * const newArray = new T[newSize];
			if(pElements){
				std::move(pElements, pElements + pSize, newArray);
				delete [] pElements;
			}
			pElements = newArray;
			pSize = newSize;
		}
		
		pElements[pCount++] = element;
	}
	
	template<typename U = T>
	requires (!std::is_lvalue_reference_v<U>)
	void Add(T &&element){
		if(pCount == pSize){
			int newSize = pSize * 3 / 2 + 1;
			T * const newArray = new T[newSize];
			if(pElements){
				std::move(pElements, pElements + pSize, newArray);
				delete [] pElements;
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
	void Insert(const TP &element, int index){
		DEASSERT_TRUE(index >= 0)
		DEASSERT_TRUE(index <= pCount)
		
		if(pCount == pSize){
			int newSize = pSize * 3 / 2 + 1;
			T * const newArray = new T[newSize];
			if(pElements){
				std::move(pElements, pElements + pSize, newArray);
				delete [] pElements;
			}
			pElements = newArray;
			pSize = newSize;
		}
		
		std::move_backward(pElements + index, pElements + pCount, pElements + pCount + 1);
		pElements[index] = element;
		pCount++;
	}
	
	template<typename U = T>
	requires (!std::is_lvalue_reference_v<U>)
	void Insert(T &&element, int index){
		DEASSERT_TRUE(index >= 0)
		DEASSERT_TRUE(index <= pCount)
		
		if(pCount == pSize){
			int newSize = pSize * 3 / 2 + 1;
			T * const newArray = new T[newSize];
			if(pElements){
				std::move(pElements, pElements + pSize, newArray);
				delete [] pElements;
			}
			pElements = newArray;
			pSize = newSize;
		}
		
		std::move_backward(pElements + index, pElements + pCount, pElements + pCount + 1);
		pElements[index] = std::move(element);
		pCount++;
	}
	
	/** \brief Add range of element. */
	void AddRange(int count, const TP &element){
		DEASSERT_TRUE(count >= 0)
		if(count == 0){
			return;
		}
		
		EnlargeCapacity(pCount + count);
		std::fill_n(pElements + pCount, count, element);
		pCount += count;
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
		
		T tempElement = std::move(pElements[from]);
		
		if(to < from){
			std::move_backward(pElements + to, pElements + from, pElements + from + 1);
			pElements[to] = std::move(tempElement);
			
		}else{
			std::move(pElements + from + 1, pElements + to, pElements + from);
			pElements[to - 1] = std::move(tempElement);
		}
	}
	
	/**
	 * \brief Swap two elements by index.
	 * \throws deeInvalidParam \em index1 or \em index2 is out of range.
	 */
	void SwapItems(int index1, int index2){
		if(index1 < 0){
			index1 = pCount + index1;
		}
		if(index2 < 0){
			index2 = pCount + index2;
		}
		DEASSERT_TRUE(index1 >= 0)
		DEASSERT_TRUE(index1 < pCount)
		DEASSERT_TRUE(index2 >= 0)
		DEASSERT_TRUE(index2 < pCount)
		
		if(index1 == index2){
			return;
		}
		
		std::swap(pElements[index1], pElements[index2]);
	}
	
	/**
	 * \brief Remove element from index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void RemoveFrom(int index){
		DEASSERT_TRUE(index >= 0)
		DEASSERT_TRUE(index < pCount)
		
		std::move(pElements + index + 1, pElements + pCount, pElements + index);
		pElements[--pCount] = T();
	}
	
	/** \brief Remove first element. */
	inline void RemoveFirst(){
		RemoveFrom(0);
	}
	
	/** \brief Remove last element. */
	inline void RemoveLast(){
		RemoveFrom(pCount - 1);
	}
	
	/** \brief Remove all elements. */
	void RemoveAll(){
		while(pCount > 0){
			pElements[--pCount] = T();
		}
	}
	
	/** \brief Remove elements from begin of list. */
	void RemoveHead(int count){
		DEASSERT_TRUE(count >= 0)
		DEASSERT_TRUE(count <= pCount)
		
		std::move(pElements + count, pElements + pCount, pElements);
		
		while(count > 0){
			pElements[--pCount] = T();
			count--;
		}
	}
	
	/** \brief Remove elements from end of list. */
	void RemoveTail(int count){
		DEASSERT_TRUE(count >= 0)
		DEASSERT_TRUE(count <= pCount)
		
		while(count > 0){
			pElements[--pCount] = T();
			count--;
		}
	}
	
	/** \brief Determine if this list is equal to another list. */
	bool Equals(const decTList &list) const{
		int p;
		
		if(list.pCount != pCount){
			return false;
		}
		
		for(p=0; p<pCount; p++){
			if(pElements[p] != list.pElements[p]){
				return false;
			}
		}
		
		return true;
	}
	
	/**
	 * \brief New list with the values from the beginning of this list.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decTList GetHead(int count) const{
		DEASSERT_TRUE(count >= 0)
		
		if(count > pCount){
			count = pCount;
		}
		if(count == 0){
			return decTList();
		}
		
		decTList list(count);
		std::copy_n(pElements, count, list.pElements);
		list.pCount = count;
		return list;
	}
	
	/**
	 * \brief Set list to values from the beginning of this list.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetHead(decTList &list, int count) const{
		DEASSERT_TRUE(count >= 0)
		
		if(count > pCount){
			count = pCount;
		}
		
		if(count > list.pSize){
			T * const newArray = new T[count];
			if(list.pElements){
				delete [] list.pElements;
			}
			list.pElements = newArray;
			list.pSize = count;
		}
		
		std::copy_n(pElements, count, list.pElements);
		list.pCount = count;
	}
	
	/**
	 * \brief New list with values from the end of this list.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decTList GetTail(int count) const{
		DEASSERT_TRUE(count >= 0)
		
		if(count > pCount){
			count = pCount;
		}
		if(count == 0){
			return decTList();
		}
		
		decTList list(count);
		std::copy_n(pElements + (pCount - count), count, list.pElements);
		list.pCount = count;
		return list;
	}
	
	/**
	 * \brief Set list to values from the end of this list.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetTail(decTList &list, int count) const{
		DEASSERT_TRUE(count >= 0)
		
		if(count > pCount){
			count = pCount;
		}
		
		if(count > list.pSize){
			T * const newArray = new T[count];
			if(list.pElements){
				delete [] list.pElements;
			}
			list.pElements = newArray;
			list.pSize = count;
		}
		
		std::copy_n(pElements + (pCount - count), count, list.pElements);
		list.pCount = count;
	}
	
	/**
	 * \brief New list with values from the middle of this list.
	 * 
	 * \em from and \em to are clamped to the last index in the list if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 */
	decTList GetMiddle(int from, int to) const{
		if(from < 0){
			from = pCount + from;
		}
		if(to < 0){
			to = pCount + to;
		}
		DEASSERT_TRUE(from >= 0)
		DEASSERT_TRUE(to >= from)
		
		int count = to - from + 1;
		if(count > pCount){
			count = pCount - from;
		}
		if(count == 0){
			return decTList();
		}
		
		decTList list(count);
		std::copy_n(pElements + from, count, list.pElements);
		list.pCount = count;
		return list;
	}
	
	/**
	 * \brief Set list to values from the middle of this list.
	 * 
	 * \em from and \em to are clamped to the last index in the list if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 */
	void GetMiddle(decTList &list, int from, int to) const{
		if(from < 0){
			from = pCount + from;
		}
		if(to < 0){
			to = pCount + to;
		}
		DEASSERT_TRUE(from >= 0)
		DEASSERT_TRUE(to >= from)
		
		int count = to - from + 1;
		if(count > pCount){
			count = pCount - from;
		}
		
		if(count > list.pSize){
			T * const newArray = new T[count];
			if(list.pElements){
				delete [] list.pElements;
			}
			list.pElements = newArray;
			list.pSize = count;
		}
		
		std::copy_n(pElements + from, count, list.pElements);
		list.pCount = count;
	}
	
	/**
	 * \brief New list with values from the middle of this list using a step size.
	 * 
	 * \em from and \em to are clamped to the last index in the list if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 * \throws deeInvalidParam \em step is less than 1.
	 */
	decTList GetSliced(int from, int to, int step) const{
		if(step == 1){
			return GetMiddle(from, to);
			
		}else{
			decTList list;
			GetSliced(list, from, to, step);
			return list;
		}
	}
	
	/**
	 * \brief Set list to values from the middle of this list using a step size.
	 * 
	 * \em from and \em to are clamped to the last index in the list if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 * \throws deeInvalidParam \em step is less than 1.
	 */
	void GetSliced(decTList &list, int from, int to, int step) const{
		if(step == 1){
			GetMiddle(list, from, to);
			return;
		}
		
		if(from < 0){
			from = pCount + from;
		}
		if(to < 0){
			to = pCount + to;
		}
		DEASSERT_TRUE(from >= 0)
		DEASSERT_TRUE(to >= from)
		DEASSERT_TRUE(step >= 1)
		
		const int count = decMath::min((to - from) / step + 1, pCount - from);
		if(count <= 0){
			list.RemoveAll();
			return;
		}
		
		if(count > list.pSize){
			T * const newArray = new T[count];
			if(list.pElements){
				delete [] list.pElements;
			}
			list.pElements = newArray;
			list.pSize = count;
		}
		
		int i, written = 0;
		for(i=from; written<count; i+=step, written++){
			list.pElements[written] = pElements[i];
		}
		list.pCount = written;
	}
	
	
	/**
	 * \brief Visit elements.
	 * \param[in] from First index to visit. Negative counts from end of list.
	 * \param[in] to One past last index to visit. Negative counts from end of list.
	 * \param[in] step Step size. Can be negative but not 0.
	 * \param[in] visitor Visitor callable invoked as visitor(T).
	 */
	template<typename Visitor>
	void Visit(int from, int to, int step, Visitor &visitor) const{
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
			
			for(i=from; i<to; i+=step){
				visitor(pElements[i]);
			}
			
		}else{
			DEASSERT_TRUE(to < pCount)
			DEASSERT_TRUE(from < pCount)
			
			for(i=from; i>=to; i+=step){
				visitor(pElements[i]);
			}
		}
	}
	
	template<typename Visitor>
	inline void Visit(int from, int to, int step, Visitor &&visitor) const{
		Visit<Visitor>(from, to, step, visitor);
	}
	
	template<typename Visitor>
	inline void Visit(int from, int to, Visitor &visitor) const{
		Visit<Visitor>(from, to, 1, visitor);
	}
	
	template<typename Visitor>
	inline void Visit(int from, int to, Visitor &&visitor) const{
		Visit<Visitor>(from, to, visitor);
	}
	
	template<typename Visitor>
	inline void Visit(int from, Visitor &visitor) const {
		Visit<Visitor>(from, pCount, visitor);
	}
	
	template<typename Visitor>
	inline void Visit(int from, Visitor &&visitor) const{
		Visit<Visitor>(from, visitor);
	}
	
	template<typename Visitor>
	void Visit(Visitor &visitor) const{
		int i;
		for(i=0; i<pCount; i++){
			visitor(pElements[i]);
		}
	}
	
	template<typename Visitor>
	inline void Visit(Visitor &&visitor) const{
		Visit<Visitor>(visitor);
	}
	
	
	/**
	 * \brief Visit elements in reverse order.
	 * \param[in] visitor Visitor callable invoked as visitor(T).
	 */
	template<typename Visitor>
	void VisitReverse(Visitor &visitor) const{
		int i;
		for(i=pCount-1; i>=0; i--){
			visitor(pElements[i]);
		}
	}
	
	template<typename Visitor>
	inline void VisitReverse(Visitor &&visitor) const{
		VisitReverse<Visitor>(visitor);
	}
	
	
	/**
	 * \brief Visit elements with index.
	 * \param[in] from First index to visit. Negative counts from end of list.
	 * \param[in] to One past last index to visit. Negative counts from end of list.
	 * \param[in] step Step size. Can be negative but not 0.
	 * \param[in] visitor Visitor callable invoked as visitor(int,T).
	 */
	template<typename Visitor>
	void VisitIndexed(int from, int to, int step, Visitor &visitor) const {
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
			
			for(i=from; i<to; i+=step){
				visitor(i, pElements[i]);
			}
			
		}else{
			DEASSERT_TRUE(to < pCount)
			DEASSERT_TRUE(from < pCount)
			
			for(i=from; i>=to; i+=step){
				visitor(i, pElements[i]);
			}
		}
	}
	
	template<typename Visitor>
	inline void VisitIndexed(int from, int to, int step, Visitor &&visitor) const{
		VisitIndexed<Visitor>(from, to, step, visitor);
	}
	
	template<typename Visitor>
	inline void VisitIndexed(int from, int to, Visitor &visitor) const {
		VisitIndexed<Visitor>(from, to, 1, visitor);
	}
	
	template<typename Visitor>
	inline void VisitIndexed(int from, int to, Visitor &&visitor) const{
		VisitIndexed<Visitor>(from, to, visitor);
	}
	
	template<typename Visitor>
	inline void VisitIndexed(int from, Visitor &visitor) const {
		VisitIndexed<Visitor>(from, pCount, visitor);
	}
	
	template<typename Visitor>
	inline void VisitIndexed(int from, Visitor &&visitor) const{
		VisitIndexed<Visitor>(from, visitor);
	}
	
	template<typename Visitor>
	void VisitIndexed(Visitor &visitor) const{
		int i;
		for(i=0; i<pCount; i++){
			visitor(i, pElements[i]);
		}
	}
	
	template<typename Visitor>
	inline void VisitIndexed(Visitor &&visitor) const{
		VisitIndexed<Visitor>(visitor);
	}
	
	
	/**
	 * \brief Visit elements in reverse order with index.
	 * \param[in] visitor Visitor callable invoked as visitor(int,T).
	 */
	template<typename Visitor>
	void VisitReverseIndexed(Visitor &visitor) const{
		int i;
		for(i=pCount-1; i>=0; i--){
			visitor(i, pElements[i]);
		}
	}
	
	template<typename Visitor>
	inline void VisitReverseIndexed(Visitor &&visitor) const{
		VisitReverseIndexed<Visitor>(visitor);
	}
	
	
	/**
	 * \brief Visit elements while evaluator returns true.
	 * \param[in] from First index to visit. Negative counts from end of list.
	 * \param[in] to One past last index to visit. Negative counts from end of list.
	 * \param[in] step Step size. Can be negative but not 0.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T).
	 */
	template<typename Evaluator>
	void VisitWhile(int from, int to, int step, Evaluator &evaluator) const{
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
			
			for(i=from; i<to; i+=step){
				if(!evaluator(pElements[i])){
					return;
				}
			}
			
		}else{
			DEASSERT_TRUE(to < pCount)
			DEASSERT_TRUE(from < pCount)
			
			for(i=from; i>=to; i+=step){
				if(!evaluator(pElements[i])){
					return;
				}
			}
		}
	}
	
	template<typename Evaluator>
	inline void VisitWhile(int from, int to, int step, Evaluator &&evaluator) const{
		VisitWhile<Evaluator>(from, to, step, evaluator);
	}
	
	template<typename Evaluator>
	inline void VisitWhile(int from, int to, Evaluator &evaluator) const{
		VisitWhile<Evaluator>(from, to, 1, evaluator);
	}
	
	template<typename Evaluator>
	inline void VisitWhile(int from, int to, Evaluator &&evaluator) const{
		VisitWhile<Evaluator>(from, to, evaluator);
	}
	
	template<typename Evaluator>
	inline void VisitWhile(int from, Evaluator &evaluator) const {
		VisitWhile<Evaluator>(from, pCount, evaluator);
	}
	
	template<typename Evaluator>
	inline void VisitWhile(int from, Evaluator &&evaluator) const{
		VisitWhile<Evaluator>(from, evaluator);
	}
	
	template<typename Evaluator>
	void VisitWhile(Evaluator &evaluator) const{
		int i;
		for(i=0; i<pCount; i++){
			if(!evaluator(pElements[i])){
				return;
			}
		}
	}
	
	template<typename Evaluator>
	inline void VisitWhile(Evaluator &&evaluator) const{
		VisitWhile<Evaluator>(evaluator);
	}
	
	
	/**
	 * \brief Visit elements in reverse order while evaluator returns true.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T).
	 */
	template<typename Evaluator>
	void VisitWhileReverse(Evaluator &evaluator) const{
		int i;
		for(i=pCount-1; i>=0; i--){
			if(!evaluator(pElements[i])){
				return;
			}
		}
	}
	
	template<typename Evaluator>
	inline void VisitWhileReverse(Evaluator &&evaluator) const{
		VisitWhileReverse<Evaluator>(evaluator);
	}
	
	
	/**
	 * \brief Visit elements with index while evaluator returns true.
	 * \param[in] from First index to visit. Negative counts from end of list.
	 * \param[in] to One past last index to visit. Negative counts from end of list.
	 * \param[in] step Step size. Can be negative but not 0.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(int, T).
	 */
	template<typename Evaluator>
	void VisitWhileIndexed(int from, int to, int step, Evaluator &evaluator) const{
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
			
			for(i=from; i<to; i+=step){
				if(!evaluator(pElements[i])){
					return;
				}
			}
			
		}else{
			DEASSERT_TRUE(to < pCount)
			DEASSERT_TRUE(from < pCount)
			
			for(i=from; i>=to; i+=step){
				if(!evaluator(pElements[i])){
					return;
				}
			}
		}
	}
	
	template<typename Evaluator>
	inline void VisitWhileIndexed(int from, int to, int step, Evaluator &&evaluator) const{
		VisitWhileIndexed<Evaluator>(from, to, step, evaluator);
	}
	
	template<typename Evaluator>
	inline void VisitWhileIndexed(int from, int to, Evaluator &evaluator) const{
		VisitWhileIndexed<Evaluator>(from, to, 1, evaluator);
	}
	
	template<typename Evaluator>
	inline void VisitWhileIndexed(int from, int to, Evaluator &&evaluator) const{
		VisitWhileIndexed<Evaluator>(from, to, evaluator);
	}
	
	template<typename Evaluator>
	inline void VisitWhileIndexed(int from, Evaluator &evaluator) const {
		VisitWhileIndexed<Evaluator>(from, pCount, evaluator);
	}
	
	template<typename Evaluator>
	inline void VisitWhileIndexed(int from, Evaluator &&evaluator) const{
		VisitWhileIndexed<Evaluator>(from, evaluator);
	}
	
	template<typename Evaluator>
	void VisitWhileIndexed(Evaluator &evaluator) const{
		int i;
		for(i=0; i<pCount; i++){
			if(!evaluator(pElements[i])){
				return;
			}
		}
	}
	
	template<typename Evaluator>
	inline void VisitWhileIndexed(Evaluator &&evaluator) const{
		VisitWhileIndexed<Evaluator>(evaluator);
	}
	
	
	/**
	 * \brief Visit elements with index in reverse order while evaluator returns true.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T).
	 */
	template<typename Evaluator>
	void VisitWhileReverseIndexed(Evaluator &evaluator) const{
		int i;
		for(i=pCount-1; i>=0; i--){
			if(!evaluator(i, pElements[i])){
				return;
			}
		}
	}
	
	template<typename Evaluator>
	inline void VisitWhileReverseIndexed(Evaluator &&evaluator) const{
		VisitWhileReverseIndexed<Evaluator>(evaluator);
	}
	
	
	/**
	 * \brief Find element.
	 * \param[out] found Found element if true is returned.
	 * \param[in] from First index to visit. Negative counts from end of list.
	 * \param[in] to One past last index to visit. Negative counts from end of list.
	 * \param[in] step Step size. Can be negative but not 0.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T).
	 */
	template<typename Evaluator>
	bool Find(const T* &found, int from, int to, int step, Evaluator &evaluator) const{
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
			
			for(i=from; i<to; i+=step){
				if(evaluator(pElements[i])){
					found = &pElements[i];
					return true;
				}
			}
			
		}else{
			DEASSERT_TRUE(to < pCount)
			DEASSERT_TRUE(from < pCount)
			
			for(i=from; i>=to; i+=step){
				if(evaluator(pElements[i])){
					found = &pElements[i];
					return true;
				}
			}
		}
		return false;
	}
	
	template<typename Evaluator>
	bool Find(T* &found, int from, int to, int step, Evaluator &evaluator){
		const T *cfound = nullptr;
		const bool result = Find<Evaluator>(cfound, from, to, step, evaluator);
		found = const_cast<T*>(cfound);
		return result;
	}
	
	template<typename Evaluator>
	inline bool Find(const T* &found, int from, int to, int step, Evaluator &&evaluator) const{
		return Find<Evaluator>(found, from, to, step, evaluator);
	}
	
	template<typename Evaluator>
	inline bool Find(T* &found, int from, int to, int step, Evaluator &&evaluator){
		return Find<Evaluator>(found, from, to, step, evaluator);
	}
	
	template<typename Evaluator>
	inline bool Find(const T* &found, int from, int to, Evaluator &evaluator) const{
		return Find<Evaluator>(found, from, to, 1, evaluator);
	}
	
	template<typename Evaluator>
	inline bool Find(T* &found, int from, int to, Evaluator &evaluator){
		return Find<Evaluator>(found, from, to, 1, evaluator);
	}
	
	template<typename Evaluator>
	inline bool Find(const T* &found, int from, int to, Evaluator &&evaluator) const{
		return Find<Evaluator>(found, from, to, evaluator);
	}
	
	template<typename Evaluator>
	inline bool Find(T* &found, int from, int to, Evaluator &&evaluator){
		return Find<Evaluator>(found, from, to, evaluator);
	}
	
	template<typename Evaluator>
	inline bool Find(const T* &found, int from, Evaluator &evaluator) const{
		return Find<Evaluator>(found, from, pCount, 1, evaluator);
	}
	
	template<typename Evaluator>
	inline bool Find(T* &found, int from, Evaluator &evaluator){
		return Find<Evaluator>(found, from, pCount, 1, evaluator);
	}
	
	template<typename Evaluator>
	inline bool Find(const T* &found, int from, Evaluator &&evaluator) const{
		return Find<Evaluator>(found, from, pCount, evaluator);
	}
	
	template<typename Evaluator>
	inline bool Find(T* &found, int from, Evaluator &&evaluator){
		return Find<Evaluator>(found, from, pCount, evaluator);
	}
	
	template<typename Evaluator>
	bool Find(const T* &found, Evaluator &evaluator) const{
		int i;
		for(i=0; i<pCount; i++){
			if(evaluator(pElements[i])){
				found = &pElements[i];
				return true;
			}
		}
		return false;
	}
	
	template<typename Evaluator>
	bool Find(T* &found, Evaluator &evaluator){
		const T *cfound = nullptr;
		const bool result = Find<Evaluator>(cfound, evaluator);
		found = const_cast<T*>(cfound);
		return result;
	}
	
	template<typename Evaluator>
	inline bool Find(const T* &found, Evaluator &&evaluator) const{
		return Find<Evaluator>(found, evaluator);
	}
	
	template<typename Evaluator>
	inline bool Find(T* &found, Evaluator &&evaluator){
		return Find<Evaluator>(found, evaluator);
	}
	
	
	/**
	 * \brief Find element in reverse order.
	 * \param[out] found Found element if true is returned.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T).
	 */
	template<typename Evaluator>
	inline bool FindReverse(const T* &found, Evaluator &evaluator) const{
		int i;
		for(i=pCount-1; i>=0; i--){
			if(evaluator(pElements[i])){
				found = &pElements[i];
				return true;
			}
		}
		return false;
	}
	
	template<typename Evaluator>
	inline bool FindReverse(T* &found, Evaluator &evaluator){
		const T *cfound = nullptr;
		const bool result = FindReverse<Evaluator>(cfound, evaluator);
		found = const_cast<T*>(cfound);
		return result;
	}
	
	template<typename Evaluator>
	inline bool FindReverse(const T* &found, Evaluator &&evaluator) const{
		return FindReverse<Evaluator>(found, evaluator);
	}
	
	template<typename Evaluator>
	inline bool FindReverse(T* &found, Evaluator &&evaluator){
		return FindReverse<Evaluator>(found, evaluator);
	}
	
	
	/**
	 * \brief Find element or default value if absent.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T).
	 * \param[in] from First index to visit. Negative counts from end of list.
	 * \param[in] to One past last index to visit. Negative counts from end of list.
	 * \param[in] step Step size. Can be negative but not 0.
	 * \return Found element or default value if not found.
	 */
	template<typename Evaluator>
	inline T FindOrDefault(const T &defaultValue, int from, int to, int step, Evaluator &evaluator) const{
		const T *found = nullptr;
		return Find<Evaluator>(found, from, to, step, evaluator) ? *found : defaultValue;
	}
	
	template<typename Evaluator>
	inline T FindOrDefault(const T &defaultValue, int from, int to, int step, Evaluator &&evaluator) const{
		return FindOrDefault<Evaluator>(defaultValue, from, to, step, evaluator);
	}
	
	template<typename Evaluator>
	inline T FindOrDefault(const T &defaultValue, int from, int to, Evaluator &evaluator) const{
		const T *found = nullptr;
		return Find<Evaluator>(found, from, to, evaluator) ? *found : defaultValue;
	}
	
	template<typename Evaluator>
	inline T FindOrDefault(const T &defaultValue, int from, int to, Evaluator &&evaluator) const{
		return FindOrDefault<Evaluator>(defaultValue, from, to, evaluator);
	}
	
	template<typename Evaluator>
	inline T FindOrDefault(const T &defaultValue, int from, Evaluator &evaluator) const{
		const T *found = nullptr;
		return Find<Evaluator>(found, from, evaluator) ? *found : defaultValue;
	}
	
	template<typename Evaluator>
	inline T FindOrDefault(const T &defaultValue, int from, Evaluator &&evaluator) const{
		return FindOrDefault<Evaluator>(defaultValue, from, evaluator);
	}
	
	template<typename Evaluator>
	inline T FindOrDefault(const T &defaultValue, Evaluator &evaluator) const{
		const T *found = nullptr;
		return Find<Evaluator>(found, evaluator) ? *found : defaultValue;
	}
	
	template<typename Evaluator>
	inline T FindOrDefault(const T &defaultValue, Evaluator &&evaluator) const{
		return FindOrDefault<Evaluator>(defaultValue, evaluator);
	}
	
	template<typename Evaluator>
	inline T FindOrDefault(Evaluator &evaluator) const{
		const T *found = nullptr;
		return Find<Evaluator>(found, evaluator) ? *found : T();
	}
	
	template<typename Evaluator>
	inline T FindOrDefault(Evaluator &&evaluator) const{
		return FindOrDefault<Evaluator>(evaluator);
	}
	
	
	/**
	 * \brief Find element in reverse order or default value if absent.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T).
	 * \return Found element or default value if not found.
	 */
	template<typename Evaluator>
	inline T FindReverseOrDefault(const T &defaultValue, Evaluator &evaluator) const{
		const T *found = nullptr;
		return FindReverse<Evaluator>(found, evaluator) ? *found : defaultValue;
	}
	
	template<typename Evaluator>
	inline T FindReverseOrDefault(const T &defaultValue, Evaluator &&evaluator) const{
		return FindReverseOrDefault<Evaluator>(defaultValue, evaluator);
	}
	
	template<typename Evaluator>
	inline T FindReverseOrDefault(Evaluator &evaluator) const{
		const T *found = nullptr;
		return FindReverse<Evaluator>(found, evaluator) ? *found : T();
	}
	
	template<typename Evaluator>
	inline T FindReverseOrDefault(Evaluator &&evaluator) const{
		return FindReverseOrDefault<Evaluator>(evaluator);
	}
	
	
	/**
	 * \brief Collect element into a new list.
	 * \param[in] from First index to visit. Negative counts from end of list.
	 * \param[in] to One past last index to visit. Negative counts from end of list.
	 * \param[in] step Step size. Can be negative but not 0.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T).
	 */
	template<typename Evaluator>
	decTList Collect(int from, int to, int step, Evaluator &evaluator) const{
		DEASSERT_TRUE(step != 0)
		
		if(from < 0){
			from = pCount + from;
		}
		DEASSERT_TRUE(from >= 0)
		
		if(to < 0){
			to = pCount + to;
		}
		DEASSERT_TRUE(to >= 0)
		
		decTList collected;
		int i;
		if(step > 0){
			DEASSERT_TRUE(to <= pCount)
			DEASSERT_TRUE(from <= pCount)
			
			for(i=from; i<to; i+=step){
				if(evaluator(pElements[i])){
					collected.Add(pElements[i]);
				}
			}
			
		}else{
			DEASSERT_TRUE(to < pCount)
			DEASSERT_TRUE(from < pCount)
			
			for(i=from; i>=to; i+=step){
				if(evaluator(pElements[i])){
					collected.Add(pElements[i]);
				}
			}
		}
		return collected;
	}
	
	template<typename Evaluator>
	inline decTList Collect(int from, int to, int step, Evaluator &&evaluator) const{
		return Collect<Evaluator>(from, to, step, evaluator);
	}
	
	template<typename Evaluator>
	decTList Collect(int from, int to, Evaluator &evaluator) const{
		return Collect<Evaluator>(from, to, 1, evaluator);
	}
	
	template<typename Evaluator>
	inline decTList Collect(int from, int to, Evaluator &&evaluator) const{
		return Collect<Evaluator>(from, to, evaluator);
	}
	
	template<typename Evaluator>
	decTList Collect(int from, Evaluator &evaluator) const{
		return Collect<Evaluator>(from, pCount, evaluator);
	}
	
	template<typename Evaluator>
	inline decTList Collect(int from, Evaluator &&evaluator) const{
		return Collect<Evaluator>(from, evaluator);
	}
	
	template<typename Evaluator>
	inline decTList Collect(Evaluator &evaluator) const{
		decTList collected;
		int i;
		for(i=0; i<pCount; i++){
			if(evaluator(pElements[i])){
				collected.Add(pElements[i]);
			}
		}
		return collected;
	}
	
	template<typename Evaluator>
	inline decTList Collect(Evaluator &&evaluator) const{
		return Collect<Evaluator>(evaluator);
	}
	
	
	/**
	 * \brief Fold over a range [from, to) (to is one-past-last for positive step,
	 *        inclusive for negative step) with given step.
	 * \param[in] combiner Combiner callable invoked as combiner(accumulator, element) -> accumulator.
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than 0.
	 * \throws deeInvalidParam \em step is 0.
	 * \return Accumulated value or default constructed T() if no elements in range.
	 */
	template<typename Combiner>
	T Fold(int from, int to, int step, Combiner &combiner) const{
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
			if(from == pCount){
				return T();
			}
			
			T acc = pElements[from];
			for(i=from+step; i<to; i+=step){
				acc = combiner(acc, pElements[i]);
			}
			return acc;
			
		}else{
			DEASSERT_TRUE(to < pCount)
			DEASSERT_TRUE(from < pCount)
			if(from == to){
				return T();
			}
			
			T acc = pElements[from];
			for(i=from+step; i>=to; i+=step){
				acc = combiner(acc, pElements[i]);
			}
			return acc;
		}
	}
	
	template<typename Combiner>
	inline T Fold(int from, int to, int step, Combiner &&combiner) const{
		return Fold<Combiner>(from, to, step, combiner);
	}
	
	template<typename Combiner>
	T Fold(int from, int to, Combiner &combiner) const{
		return Fold<Combiner>(from, to, 1, combiner);
	}
	
	template<typename Combiner>
	inline T Fold(int from, int to, Combiner &&combiner) const{
		return Fold<Combiner>(from, to, combiner);
	}
	
	/**
	 * \brief Fold starting at index from to end.
	 * \param[in] combiner Combiner callable invoked as combiner(accumulator, element) -> accumulator.
	 * \throws deeInvalidParam \em from is less than 0 or larger than GetCount()-1.
	 * \return Accumulated value or default constructed T() if no elements in range.
	 */
	template<typename Combiner>
	T Fold(int from, Combiner &combiner) const{
		return Fold<Combiner>(from, pCount, combiner);
	}
	
	template<typename Combiner>
	inline T Fold(int from, Combiner &&combiner) const{
		return Fold<Combiner>(from, combiner);
	}
	
	/**
	 * \brief Fold (reduce) elements using a combiner.
	 * \param[in] combiner Combiner callable invoked as combiner(accumulator, element) -> accumulator.
	 * \return Accumulated value or default constructed T() if no elements.
	 */
	template<typename Combiner>
	T Fold(Combiner &combiner) const{
		DEASSERT_TRUE(IsNotEmpty())
		T acc = pElements[0];
		int i;
		for(i=1; i<pCount; i++){
			acc = combiner(acc, pElements[i]);
		}
		return acc;
	}
	
	template<typename Combiner>
	inline T Fold(Combiner &&combiner) const{
		return Fold<Combiner>(combiner);
	}
	
	
	/**
	 * \brief Inject (reduce) elements using a combiner starting with initial value.
	 * \param[in] value Initial value.
	 * \param[in] combiner Combiner callable invoked as combiner(accumulator, element) -> accumulator.
	 */
	template<typename R, typename Combiner>
	R Inject(const R &value, int from, int to, int step, Combiner &combiner) const{
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
			for(i=from; i<to; i+=step){
				acc = combiner(acc, pElements[i]);
			}
			
		}else{
			DEASSERT_TRUE(to < pCount)
			DEASSERT_TRUE(from < pCount)
			for(i=from; i>=to; i+=step){
				acc = combiner(acc, pElements[i]);
			}
		}
		return acc;
	}
	
	template<typename R, typename Combiner>
	inline R Inject(const R &value, int from, int to, int step, Combiner &&combiner) const{
		return Inject<R,Combiner>(value, from, to, step, combiner);
	}
	
	template<typename R, typename Combiner>
	R Inject(const R &value, int from, int to, Combiner &combiner) const{
		return Inject<R,Combiner>(value, from, to, 1, combiner);
	}
	
	template<typename R, typename Combiner>
	inline R Inject(const R &value, int from, int to, Combiner &&combiner) const{
		return Inject<R,Combiner>(value, from, to, combiner);
	}
	
	template<typename R, typename Combiner>
	R Inject(const R &value, int from, Combiner &combiner) const{
		return Inject<R,Combiner>(value, from, pCount, combiner);
	}
	
	template<typename R, typename Combiner>
	inline R Inject(const R &value, int from, Combiner &&combiner) const{
		return Inject<R,Combiner>(value, from, combiner);
	}
	
	template<typename R, typename Combiner>
	R Inject(const R &value, Combiner &combiner) const{
		R acc = value;
		int i;
		for(i=0; i<pCount; i++){
			acc = combiner(acc, pElements[i]);
		}
		return acc;
	}
	
	template<typename R, typename Combiner>
	inline R Inject(const R &value, Combiner &&combiner) const{
		return Inject<R,Combiner>(value, combiner);
	}
	
	
	/**
	 * \brief Reverse order of elements.
	 */
	void Reverse(){
		int i;
		for(i=0; i<pCount / 2; i++){
			std::swap(pElements[i], pElements[pCount - 1 - i]);
		}
	}
	
	/**
	 * \brief Set with elements in reverse order.
	 */
	decTList GetReversed() const{
		decTList reversed(pCount);
		int i;
		for(i=0; i<pCount; i++){
			reversed.pElements[i] = pElements[pCount - 1 - i];
		}
		reversed.pCount = pCount;
		return reversed;
	}
	
	
	/**
	 * \brief Remove elements matching condition.
	 * \param[in] from First index to visit. Negative counts from end of list.
	 * \param[in] to One past last index to visit. Negative counts from end of list.
	 * \param[in] step Step size. Can be negative but not 0.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T).
	 */
	template<typename Evaluator>
	void RemoveIf(int from, int to, int step, Evaluator &evaluator){
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
			
			for(i=from; i<to; i+=step){
				if(evaluator(pElements[i])){
					RemoveFrom(i);
					i--;
					to--;
				}
			}
			
		}else{
			DEASSERT_TRUE(to < pCount)
			DEASSERT_TRUE(from < pCount)
			
			for(i=from; i>=to; i+=step){
				if(evaluator(pElements[i])){
					RemoveFrom(i);
					i++;
					to++;
				}
			}
		}
	}
	
	template<typename Evaluator>
	inline void RemoveIf(int from, int to, int step, Evaluator &&evaluator){
		RemoveIf<Evaluator>(from, to, step, evaluator);
	}
	
	template<typename Evaluator>
	void RemoveIf(int from, int to, Evaluator &evaluator){
		RemoveIf<Evaluator>(from, to, 1, evaluator);
	}
	
	template<typename Evaluator>
	inline void RemoveIf(int from, int to, Evaluator &&evaluator){
		RemoveIf<Evaluator>(from, to, evaluator);
	}
	
	template<typename Evaluator>
	void RemoveIf(int from, Evaluator &evaluator){
		RemoveIf<Evaluator>(from, pCount, evaluator);
	}
	
	template<typename Evaluator>
	inline void RemoveIf(int from, Evaluator &&evaluator){
		RemoveIf<Evaluator>(from, evaluator);
	}
	
	template<typename Evaluator>
	void RemoveIf(Evaluator &evaluator){
		int i, last = 0;
		for(i=0; i<pCount; i++){
			if(evaluator(pElements[i])){
				continue;
			}
			
			if(last < i){
				pElements[last] = std::move(pElements[i]);
			}
			last++;
		}
		while(pCount > last){
			pElements[--pCount] = T();
		}
	}
	
	template<typename Evaluator>
	inline void RemoveIf(Evaluator &&evaluator){
		RemoveIf<Evaluator>(evaluator);
	}
	
	
	/** \brief Sort elements in place. */
	template<typename Comparator>
	void Sort(Comparator &comparator){
		if(pCount > 1){
			pSort<Comparator>(comparator, 0, pCount - 1);
		}
	}
	
	template<typename Comparator>
	inline void Sort(Comparator &&comparator){
		Sort<Comparator>(comparator);
	}
	
	/** \brief Sort using decAscendingComparator. */
	void SortAscending(){
		if constexpr (requires(const T& a, const T& b) { { DECompare(a, b) } -> std::convertible_to<int>; }) {
			Sort<decAscendingComparator<T>>(decAscendingComparator<T>());

		}else{
			DEThrowInvalidParam(__FILE__, __LINE__, DE_CUR_FUNC_NAME);
		}
	}
	
	/** \brief Sort using decDescendingComparator. */
	void SortDescending(){
		if constexpr (requires(const T & a, const T & b) { { DECompare(a, b) } -> std::convertible_to<int>; }) {
			Sort<decDesendingComparator<T>>(decDesendingComparator<T>());

		} else {
			DEThrowInvalidParam(__FILE__, __LINE__, DE_CUR_FUNC_NAME);
		}
	}
	
	/** \brief Sort elements as new list. */
	template<typename Comparator>
	decTList GetSorted(Comparator &comparator) const{
		decTList copy(*this);
		copy.Sort<Comparator>(comparator);
		return copy;
	}
	
	template<typename Comparator>
	inline decTList GetSorted(Comparator &&comparator) const{
		return GetSorted<Comparator>(comparator);
	}
	
	/** \brief Sort as new list using decAscendingComparator. */
	decTList GetSortedAscending() const{
		if constexpr (requires(const T & a, const T & b) { { DECompare(a, b) } -> std::convertible_to<int>; }) {
			return GetSorted<decAscendingComparator<T>>(decAscendingComparator<T>());

		}else{
			DEThrowInvalidParam(__FILE__, __LINE__, DE_CUR_FUNC_NAME);
			return {}; // we never get here. keep MSVC from complaining
		}
	}
	
	/** \brief Sort as new list using decDescendingComparator. */
	decTList GetSortedDescending() const{
		if constexpr (requires(const T & a, const T & b) { { DECompare(a, b) } -> std::convertible_to<int>; }) {
			return GetSorted<decDesendingComparator<T>>(decDesendingComparator<T>());

		}else{
			DEThrowInvalidParam(__FILE__, __LINE__, DE_CUR_FUNC_NAME);
			return {}; // we never get here. keep MSVC from complaining
		}
	}
	
	
	/**
	 * \brief Swap content with another list.
	 * \note This also swaps the capacities.
	 */
	void Swap(decTList &list){
		if(&list == this){
			return;
		}
		
		T *tempElements = pElements;
		int tempCount = pCount;
		int tempSize = pSize;
		
		pElements = list.pElements;
		pCount = list.pCount;
		pSize = list.pSize;
		
		list.pElements = tempElements;
		list.pCount = tempCount;
		list.pSize = tempSize;
	}
	
	
	/**
	 * \brief Direct access to array pointer.
	 * \warning Use only if necessary.
	 */
	inline T* GetArrayPointer(){
		return pElements;
	}
	
	inline const T* GetArrayPointer() const{
		return pElements;
	}
	
	
	/** \brief Reduce capacity to element count. */
	void CompactCapacity(){
		if(pCount == pSize){
			return;
		}
		
		if(pCount == 0){
			if(pElements){
				delete [] pElements;
				pElements = nullptr;
			}
			pSize = 0;
			return;
		}
		
		T * const newArray = new T[pCount];
		std::move(pElements, pElements + pCount, newArray);
		delete [] pElements;
		pElements = newArray;
		pSize = pCount;
	}
	/*@}*/
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Determine if this list is equal to another list. */
	bool operator==(const decTList &list) const{
		return Equals(list);
	}
	
	/** \brief Determine if this list is not equal to another list. */
	bool operator!=(const decTList &list) const{
		return !Equals(list);
	}
	
	/** \brief New list containing all values of this list followed by all values of another list. */
	decTList operator+(const decTList &list) const{
		decTList nlist(pCount + list.pCount);
		std::copy_n(pElements, pCount, nlist.pElements);
		std::copy_n(list.pElements, list.pCount, nlist.pElements + pCount);
		nlist.pCount = pCount + list.pCount;
		return nlist;
	}
	
	/**
	 * \brief Element at index.
	 * 
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	const T &operator[](int index) const{
		return GetAt(index);
	}
	
	/**
	 * \brief Element at index.
	 * 
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	T &operator[](int index){
		return GetAt(index);
	}
	
	/** \brief Copy list to this list. */
	decTList &operator=(const decTList &list){
		if(&list == this){
			return *this;
		}
		
		RemoveAll();
		return *this += list;
	}
	
	/** \brief Move list. */
	decTList &operator=(decTList &&list){
		if(&list == this){
			return *this;
		}
		
		RemoveAll();
		
		if(pElements){
			delete [] pElements;
		}
		
		pElements = list.pElements;
		pCount = list.pCount;
		pSize = list.pSize;
		
		list.pElements = nullptr;
		list.pCount = 0;
		list.pSize = 0;
		
		return *this;
	}
	
	/** \brief Append values of list to this list. */
	decTList &operator+=(const decTList &list){
		if(list.pCount > 0){
			const int count = pCount + list.pCount;
			EnlargeCapacity(count);
			
			std::copy_n(list.pElements, list.pCount, pElements + pCount);
			pCount = count;
		}
		
		return *this;
	}
	/*@}*/
	
	
	/** \name Standard library iterators. */
	/*@{*/
	class const_iterator{
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = const T*;
		using reference = const T&;
		
		explicit const_iterator(const decTList* owner = nullptr, int index = 0) :
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
		
		difference_type operator-(const const_iterator &other) const{
			return static_cast<difference_type>(pIndex - other.pIndex);
		}
		
		reference operator[](difference_type n) const{
			return pOwner->GetAt(pIndex + static_cast<int>(n));
		}
		
		bool operator==(const const_iterator &o) const{
			return pOwner == o.pOwner && pIndex == o.pIndex;
		}
		
		bool operator!=(const const_iterator &o) const{
			return !(*this == o);
		}
		
		bool operator<(const const_iterator &o) const{
			return pIndex < o.pIndex;
		}
		
		bool operator<=(const const_iterator &o) const{
			return pIndex <= o.pIndex;
		}
		
		bool operator>(const const_iterator &o) const{
			return pIndex > o.pIndex;
		}
		
		bool operator>=(const const_iterator &o) const{
			return pIndex >= o.pIndex;
		}
		
	private:
		const decTList* pOwner;
		int pIndex;
	};
	
	class iterator{
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using reference = T&;
		
		explicit iterator(decTList* owner = nullptr, int index = 0) :
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
		
		difference_type operator-(const iterator &other) const{
			return static_cast<difference_type>(pIndex - other.pIndex);
		}
		
		reference operator[](difference_type n) const{
			return pOwner->GetAt(pIndex + static_cast<int>(n));
		}
		
		bool operator==(const iterator &o) const{
			return pOwner == o.pOwner && pIndex == o.pIndex;
		}
		
		bool operator!=(const iterator &o) const{
			return !(*this == o);
		}
		
		bool operator<(const iterator &o) const{
			return pIndex < o.pIndex;
		}
		
		bool operator<=(const iterator &o) const{
			return pIndex <= o.pIndex;
		}
		
		bool operator>(const iterator &o) const{
			return pIndex > o.pIndex;
		}
		
		bool operator>=(const iterator &o) const{
			return pIndex >= o.pIndex;
		}
		
	private:
		decTList* pOwner;
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
	const_iterator cbegin() const {
		return const_iterator(this, 0);
	}
	
	/** Standard library iterator pointing at one past last element in list */
	iterator end(){
		return iterator(this, pCount);
	}
	const_iterator end() const {
		return cend();
	}
	const_iterator cend() const {
		return const_iterator(this, pCount);
	}
	
	/** Standard library reverse iterator pointing at one past last element in list */
	reverse_iterator rbegin(){
		return reverse_iterator(end());
	}
	const_reverse_iterator rbegin() const{
		return const_reverse_iterator(end());
	}
	const_reverse_iterator crbegin() const {
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
	void pSort(Comparator &comparator, int left, int right){
		T pivot = std::move(pElements[left]);
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



/**
 * \brief Object list template class.
 * 
 * All objects including nullptr are allowed.
 * 
 * This template uses deTObjectReference.
 */
template<typename T>
using decTObjectList = decTList<deTObjectReference<T>, T*>;

/**
 * \brief Thread safe object list template class.
 * 
 * All objects including nullptr are allowed.
 * 
 * This template uses deTThreadSafeObjectReference to get thread safe assignment of
 * object references. This does not make the set itself thread safe though.
 */
template<typename T>
using decTThreadSafeObjectList = decTList<deTThreadSafeObjectReference<T>, T*>;

#endif
