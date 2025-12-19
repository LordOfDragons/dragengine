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

#include <iterator>
#include <cstddef>

#include "decCollectionInterfaces.h"
#include "../math/decMath.h"
#include "../exceptions_reduced.h"
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
	explicit decTList(int capacity) : pElements(nullptr), pCount(0), pSize(0){
		DEASSERT_TRUE(capacity >= 0)
		
		if(capacity > 0){
			pElements = new T[capacity];
			pSize = capacity;
		}
	}
	
	/** \brief Create a copy of a list. */
	decTList(const decTList<T,TP> &list) : pElements(nullptr), pCount(0), pSize(0){
		if(list.pCount == 0){
			return;
		}
		
		pElements = new T[list.pCount];
		pSize = list.pCount;
		
		for(pCount=0; pCount<list.pCount; pCount++){
			pElements[pCount] = list.pElements[pCount];
		}
	}
	
	/** \brief Move list. */
	decTList(decTList<T,TP> &&list) : pElements(list.pElements), pCount(list.pCount), pSize(list.pSize){
		list.pElements = nullptr;
		list.pCount = 0;
		list.pSize = 0;
	}
	
	/** \brief Clean up the list. */
	~decTList(){
		RemoveAll();
		
		if(pElements){
			delete [] pElements;
		}
	}
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of elements. */
	inline int GetCount() const{ return pCount; }
	
	/** \brief List is empty. */
	inline bool IsEmpty() const{ return pCount == 0; }
	
	/** \brief List is not empty. */
	inline bool IsNotEmpty() const{ return pCount > 0; }
	
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
	const T &First() const{
		DEASSERT_TRUE(pCount > 0)
		return pElements[0];
	}
	
	/**
	 * \brief Last element.
	 * \throws deeInvalidParam if list is empty.
	 */
	const T &Last() const{
		DEASSERT_TRUE(pCount > 0)
		return pElements[pCount - 1];
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
	
	/** \brief Index of the first occurance of an element or -1 if not found. */
	int IndexOf(const TP &element) const{
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
			if(element == pElements[i]){
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
		return IndexOfMatching<Evaluator>(evaluator, 0);
	}
	
	template<typename Evaluator>
	int IndexOfMatching(Evaluator &&evaluator) const{
		return IndexOfMatching<Evaluator>(evaluator);
	}
	
	/**
	 * \brief Index of first element match condition or -1 if not found.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T).
	 * \throws deeInvalidParam \em start is less than 0 or larger than GetCount().
	 */
	template<typename Evaluator>
	int IndexOfMatching(Evaluator &evaluator, int start) const{
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
	int IndexOfMatching(Evaluator &&evaluator, int start) const{
		return IndexOfMatching<Evaluator>(evaluator, start);
	}
	
	/** \brief Determine if element exists in the list. */
	bool Has(const TP &element) const{
		int p;
		
		for(p=0; p<pCount; p++){
			if(element == pElements[p]){
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
		return Find<Evaluator>(evaluator, f);
	}
	
	template<typename Evaluator>
	bool HasMatching(Evaluator &&evaluator) const{
		return HasMatching<Evaluator>(evaluator);
	}
	
	/** \brief Number of times element exists in the list. */
	int CountOccurance(const TP &element) const{
		int count = 0;
		int p;
		
		for(p=0; p<pCount; p++){
			if(element == pElements[p]){
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
				int i;
				for(i=0; i<pSize; i++){
					newArray[i] = pElements[i];
				}
				delete [] pElements;
			}
			pElements = newArray;
			pSize = newSize;
		}
		
		pElements[pCount++] = element;
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
				int i;
				for(i=0; i<pSize; i++){
					newArray[i] = pElements[i];
				}
				delete [] pElements;
			}
			pElements = newArray;
			pSize = newSize;
		}
		
		int i;
		for(i=pCount; i>index; i--){
			pElements[i] = pElements[i - 1];
		}
		pElements[index] = element;
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
		
		const T tempElement(pElements[from]);
		int i;
		
		if(to < from){
			for(i=from; i>to; i--){
				pElements[i] = pElements[i - 1];
			}
			pElements[to] = tempElement;
			
		}else{
			for(i=from; i<to-1; i++){
				pElements[i] = pElements[i + 1];
			}
			pElements[to - 1] = tempElement;
		}
	}
	
	/**
	 * \brief Remove element from index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void RemoveFrom(int index){
		DEASSERT_TRUE(index >= 0)
		DEASSERT_TRUE(index < pCount)
		
		int i;
		for(i=index+1; i<pCount; i++){
			pElements[i - 1] = pElements[i];
		}
		pElements[--pCount] = T();
	}
	
	/** \brief Remove all elements. */
	void RemoveAll(){
		while(pCount > 0){
			pElements[--pCount] = T();
		}
	}
	
	/** \brief Determine if this list is equal to another list. */
	bool Equals(const decTList<T,TP> &list) const{
		int p;
		
		if(list.pCount != pCount){
			return false;
		}
		
		for(p=0; p<pCount; p++){
			if(!(pElements[p] == list.pElements[p])){
				return false;
			}
		}
		
		return true;
	}
	
	/**
	 * \brief New list with the values from the beginning of this list.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decTList<T,TP> GetHead(int count) const{
		DEASSERT_TRUE(count >= 0)
		
		if(count > pCount){
			count = pCount;
		}
		if(count == 0){
			return decTList<T,TP>();
		}
		
		decTList<T,TP> list(count);
		for(list.pCount=0; list.pCount<count; list.pCount++){
			list.pElements[list.pCount] = pElements[list.pCount];
		}
		
		return list;
	}
	
	/**
	 * \brief Set list to values from the beginning of this list.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetHead(decTList<T,TP> &list, int count) const{
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
		
		for(list.pCount=0; list.pCount<count; list.pCount++){
			list.pElements[list.pCount] = pElements[list.pCount];
		}
	}
	
	/**
	 * \brief New list with values from the end of this list.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decTList<T,TP> GetTail(int count) const{
		DEASSERT_TRUE(count >= 0)
		
		if(count > pCount){
			count = pCount;
		}
		if(count == 0){
			return decTList<T,TP>();
		}
		
		decTList<T,TP> list(count);
		int from = pCount - count;
		
		for(list.pCount=0; list.pCount<count; list.pCount++){
			list.pElements[list.pCount] = pElements[from + list.pCount];
		}
		
		return list;
	}
	
	/**
	 * \brief Set list to values from the end of this list.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetTail(decTList<T,TP> &list, int count) const{
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
		
		int from = pCount - count;
		
		for(list.pCount=0; list.pCount<count; list.pCount++){
			list.pElements[list.pCount] = pElements[from + list.pCount];
		}
	}
	
	/**
	 * \brief New list with values from the middle of this list.
	 * 
	 * \em from and \em to are clamped to the last index in the list if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 */
	decTList<T,TP> GetMiddle(int from, int to) const{
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
			return decTList<T,TP>();
		}
		
		decTList<T,TP> list(count);
		
		for(list.pCount=0; list.pCount<count; list.pCount++){
			list.pElements[list.pCount] = pElements[from + list.pCount];
		}
		
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
	void GetMiddle(decTList<T,TP> &list, int from, int to) const{
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
		
		for(list.pCount=0; list.pCount<count; list.pCount++){
			list.pElements[list.pCount] = pElements[from + list.pCount];
		}
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
	decTList<T,TP> GetSliced(int from, int to, int step) const{
		if(step == 1){
			return GetMiddle(from, to);
			
		}else{
			decTList<T,TP> list;
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
	void GetSliced(decTList<T,TP> &list, int from, int to, int step) const{
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
	 * \param[in] visitor Visitor callable invoked as visitor(T).
	 * \param[in] from First index to visit. Negative counts from end of list.
	 * \param[in] to One past last index to visit. Negative counts from end of list.
	 * \param[in] step Step size. Can be negative but not 0.
	 */
	template<typename Visitor>
	void Visit(Visitor &visitor, int from, int to, int step = 1) const {
		DEASSERT_TRUE(step != 0)
		
		if(from < 0){
			from = pCount + from;
		}
		DEASSERT_TRUE(from >= 0)
		DEASSERT_TRUE(from < pCount)
		
		if(to < 0){
			to = pCount + to;
		}
		DEASSERT_TRUE(to >= 0)
		
		int i;
		if(step > 0){
			DEASSERT_TRUE(to <= pCount)
			
			for(i=from; i<to; i+=step){
				visitor(pElements[i]);
			}
			
		}else{
			DEASSERT_TRUE(to < pCount)
			
			for(i=from; i>=to; i+=step){
				visitor(pElements[i]);
			}
		}
	}
	
	template<typename Visitor>
	void Visit(Visitor &&visitor, int from, int to, int step = 1) const{
		Visit<Visitor>(visitor, from, to, step);
	}
	
	template<typename Visitor>
	void Visit(Visitor &visitor, int from) const {
		if(from < 0){
			from = pCount + from;
		}
		DEASSERT_TRUE(from >= 0)
		DEASSERT_TRUE(from < pCount)
		
		int i;
		for(i=from; i<pCount; i++){
			visitor(pElements[i]);
		}
	}
	
	template<typename Visitor>
	void Visit(Visitor &&visitor, int from) const{
		Visit<Visitor>(visitor, from);
	}
	
	template<typename Visitor>
	void Visit(Visitor &visitor) const{
		int i;
		for(i=0; i<pCount; i++){
			visitor(pElements[i]);
		}
	}
	
	template<typename Visitor>
	inline void Visit(Visitor &&visitor) const{ Visit<Visitor>(visitor); }
	
	template<typename Visitor>
	void VisitReverse(Visitor &visitor) const{
		int i;
		for(i=pCount-1; i>=0; i--){
			visitor(pElements[i]);
		}
	}
	
	template<typename Visitor>
	inline void VisitReverse(Visitor &&visitor) const{ VisitReverse<Visitor>(visitor); }
	
	
	/**
	 * \brief Find element.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T).
	 * \param[out] found Found element if true is returned.
	 * \param[in] from First index to visit. Negative counts from end of list.
	 * \param[in] to One past last index to visit. Negative counts from end of list.
	 * \param[in] step Step size. Can be negative but not 0.
	 */
	template<typename Evaluator>
	bool Find(Evaluator &evaluator, const T* &found, int from, int to, int step = 1) const{
		DEASSERT_TRUE(step != 0)
		
		if(from < 0){
			from = pCount + from;
		}
		DEASSERT_TRUE(from >= 0)
		DEASSERT_TRUE(from < pCount)
		
		if(to < 0){
			to = pCount + to;
		}
		DEASSERT_TRUE(to >= 0)
		
		int i;
		if(step > 0){
			DEASSERT_TRUE(to <= pCount)
			
			for(i=from; i<to; i+=step){
				if(evaluator(pElements[i])){
					found = &pElements[i];
					return true;
				}
			}
			
		}else{
			DEASSERT_TRUE(to < pCount)
			
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
	bool Find(Evaluator &&evaluator, const T* &found, int from, int to, int step = 1) const{
		return Find<Evaluator>(evaluator, found, from, to, step);
	}
	
	template<typename Evaluator>
	bool Find(Evaluator &evaluator, const T* &found, int from) const{
		if(from < 0){
			from = pCount + from;
		}
		DEASSERT_TRUE(from >= 0)
		DEASSERT_TRUE(from < pCount)
		
		int i;
		for(i=from; i<pCount; i++){
			if(evaluator(pElements[i])){
				found = &pElements[i];
				return true;
			}
		}
		return false;
	}
	
	template<typename Evaluator>
	bool Find(Evaluator &&evaluator, const T* &found, int from) const{
		return Find<Evaluator>(evaluator, found, from);
	}
	
	template<typename Evaluator>
	inline bool Find(Evaluator &evaluator, const T* &found) const{
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
	inline bool Find(Evaluator &&evaluator, const T* &found) const{
		return Find<Evaluator>(evaluator, found);
	}
	
	template<typename Evaluator>
	inline bool FindReverse(Evaluator &evaluator, const T* &found) const{
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
	inline bool FindReverse(Evaluator &&evaluator, const T* &found) const{
		return FindReverse<Evaluator>(evaluator, found);
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
	T FindOrDefault(Evaluator &evaluator, const T &defaultValue, int from, int to, int step = 1) const{
		const T *found = nullptr;
		return Find<Evaluator>(evaluator, found, from, to, step) ? *found : defaultValue;
	}
	
	template<typename Evaluator>
	T FindOrDefault(Evaluator &&evaluator, const T &defaultValue, int from, int to, int step = 1) const{
		return FindOrDefault<Evaluator>(evaluator, defaultValue, from, to, step);
	}
	
	template<typename Evaluator>
	T FindOrDefault(Evaluator &evaluator, const T &defaultValue, int from) const{
		const T *found = nullptr;
		return Find<Evaluator>(evaluator, found, from) ? *found : defaultValue;
	}
	
	template<typename Evaluator>
	T FindOrDefault(Evaluator &&evaluator, const T &defaultValue, int from) const{
		return FindOrDefault<Evaluator>(evaluator, defaultValue, from);
	}
	
	template<typename Evaluator>
	inline T FindOrDefault(Evaluator &evaluator, const T &defaultValue = T()) const{
		const T *found = nullptr;
		return Find<Evaluator>(evaluator, found) ? *found : defaultValue;
	}
	
	template<typename Evaluator>
	inline T FindOrDefault(Evaluator &&evaluator, const T &defaultValue = T()) const{
		return FindOrDefault<Evaluator>(evaluator, defaultValue);
	}
	
	template<typename Evaluator>
	inline T FindReverseOrDefault(Evaluator &evaluator, const T &defaultValue = T()) const{
		const T *found = nullptr;
		return FindReverse<Evaluator>(evaluator, found) ? *found : defaultValue;
	}
	
	template<typename Evaluator>
	inline T FindReverseOrDefault(Evaluator &&evaluator, const T &defaultValue = T()) const{
		return FindReverseOrDefault<Evaluator>(evaluator, defaultValue);
	}
	
	
	/**
	 * \brief Collect element into a new list.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T).
	 * \param[in] from First index to visit. Negative counts from end of list.
	 * \param[in] to One past last index to visit. Negative counts from end of list.
	 * \param[in] step Step size. Can be negative but not 0.
	 */
	template<typename Evaluator>
	decTList<T,TP> Collect(Evaluator &evaluator, int from, int to, int step = 1) const{
		DEASSERT_TRUE(step != 0)
		
		if(from < 0){
			from = pCount + from;
		}
		DEASSERT_TRUE(from >= 0)
		DEASSERT_TRUE(from < pCount)
		
		if(to < 0){
			to = pCount + to;
		}
		DEASSERT_TRUE(to >= 0)
		
		decTList<T,TP> collected;
		int i;
		if(step > 0){
			DEASSERT_TRUE(to <= pCount)
			
			for(i=from; i<to; i+=step){
				if(evaluator(pElements[i])){
					collected.Add(pElements[i]);
				}
			}
			
		}else{
			DEASSERT_TRUE(to < pCount)
			
			for(i=from; i>=to; i+=step){
				if(evaluator(pElements[i])){
					collected.Add(pElements[i]);
				}
			}
		}
		return collected;
	}
	
	template<typename Evaluator>
	decTList<T,TP> Collect(Evaluator &&evaluator, int from, int to, int step = 1) const{
		return Collect<Evaluator>(evaluator, from, to, step);
	}
	
	template<typename Evaluator>
	decTList<T,TP> Collect(Evaluator &evaluator, int from) const{
		if(from < 0){
			from = pCount + from;
		}
		DEASSERT_TRUE(from >= 0)
		DEASSERT_TRUE(from < pCount)
		
		decTList<T,TP> collected;
		int i;
		for(i=from; i<pCount; i++){
			if(evaluator(pElements[i])){
				collected.Add(pElements[i]);
			}
		}
		return collected;
	}
	
	template<typename Evaluator>
	decTList<T,TP> Collect(Evaluator &&evaluator, int from) const{
		return Collect<Evaluator>(evaluator, from);
	}
	
	template<typename Evaluator>
	inline decTList<T,TP> Collect(Evaluator &evaluator) const{
		decTList<T,TP> collected;
		int i;
		for(i=0; i<pCount; i++){
			if(evaluator(pElements[i])){
				collected.Add(pElements[i]);
			}
		}
		return collected;
	}
	
	template<typename Evaluator>
	inline decTList<T,TP> Collect(Evaluator &&evaluator) const{
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
	T Fold(Combiner &combiner, int from, int to, int step = 1) const{
		DEASSERT_TRUE(step != 0)
		
		if(from < 0){
			from = pCount + from;
		}
		DEASSERT_TRUE(from >= 0)
		DEASSERT_TRUE(from < pCount)
		
		if(to < 0){
			to = pCount + to;
		}
		DEASSERT_TRUE(to >= 0)
		
		int i;
		if(step > 0){
			DEASSERT_TRUE(to <= pCount)
			DEASSERT_TRUE(from < to)
			T acc = pElements[from];
			
			for(i=from+step; i<to; i+=step){
				acc = combiner(acc, pElements[i]);
			}
			return acc;
			
		}else{
			DEASSERT_TRUE(to < pCount)
			DEASSERT_TRUE(from > to)
			T acc = pElements[from];
			
			for(i=from+step; i>=to; i+=step){
				acc = combiner(acc, pElements[i]);
			}
			return acc;
		}
	}
	
	template<typename Combiner>
	T Fold(Combiner &&combiner, int from, int to, int step = 1) const{
		return Fold<Combiner>(combiner, from, to, step);
	}
	
	/**
	 * \brief Fold starting at index from to end.
	 * \param[in] combiner Combiner callable invoked as combiner(accumulator, element) -> accumulator.
	 * \throws deeInvalidParam \em from is less than 0 or larger than GetCount()-1.
	 * \return Accumulated value or default constructed T() if no elements in range.
	 */
	template<typename Combiner>
	T Fold(Combiner &combiner, int from) const{
		return Fold<Combiner>(combiner, from, pCount);
	}
	
	template<typename Combiner>
	T Fold(Combiner &&combiner, int from) const{
		return Fold<Combiner>(combiner, from);
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
	T Fold(Combiner &&combiner) const{
		return Fold<Combiner>(combiner);
	}
	
	
	/**
	 * \brief Inject (reduce) elements using a combiner starting with initial value.
	 * \param[in] value Initial value.
	 * \param[in] combiner Combiner callable invoked as combiner(accumulator, element) -> accumulator.
	 */
	template<typename R, typename Combiner>
	R Inject(const R &value, Combiner &combiner, int from, int to, int step = 1) const{
		DEASSERT_TRUE(step != 0)
		
		if(from < 0){
			from = pCount + from;
		}
		DEASSERT_TRUE(from >= 0)
		DEASSERT_TRUE(from < pCount)
		
		if(to < 0){
			to = pCount + to;
		}
		DEASSERT_TRUE(to >= 0)
		
		R acc = value;
		int i;
		if(step > 0){
			DEASSERT_TRUE(to <= pCount)
			for(i=from; i<to; i+=step){
				acc = combiner(acc, pElements[i]);
			}
			
		}else{
			DEASSERT_TRUE(to < pCount)
			for(i=from; i>=to; i+=step){
				acc = combiner(acc, pElements[i]);
			}
		}
		return acc;
	}
	
	template<typename R, typename Combiner>
	R Inject(const R &value, Combiner &&combiner, int from, int to, int step = 1) const{
		return Inject<R,Combiner>(value, combiner, from, to, step);
	}
	
	template<typename R, typename Combiner>
	R Inject(const R &value, Combiner &combiner, int from) const{
		return Inject<R,Combiner>(value, combiner, from, pCount);
	}
	
	template<typename R, typename Combiner>
	R Inject(const R &value, Combiner &&combiner, int from) const{
		return Inject<R,Combiner>(value, combiner, from);
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
	R Inject(const R &value, Combiner &&combiner) const{
		return Inject<R,Combiner>(value, combiner);
	}
	
	
	/**
	 * \brief Reverse order of elements.
	 */
	void Reverse(){
		int i;
		for(i=0; i<pCount / 2; i++){
			const T temp = pElements[i];
			pElements[i] = pElements[pCount - 1 - i];
			pElements[pCount - 1 - i] = temp;
		}
	}
	
	/**
	 * \brief Set with elements in reverse order.
	 */
	decTList<T,TP> GetReversed() const{
		decTList<T,TP> reversed(pCount);
		int i;
		for(i=0; i<pCount; i++){
			reversed.pElements[i] = pElements[pCount - 1 - i];
		}
		reversed.pCount = pCount;
		return reversed;
	}
	
	
	/**
	 * \brief Remove elements matching condition.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T).
	 * \param[in] from First index to visit. Negative counts from end of list.
	 * \param[in] to One past last index to visit. Negative counts from end of list.
	 * \param[in] step Step size. Can be negative but not 0.
	 */
	template<typename Evaluator>
	void RemoveIf(Evaluator &evaluator, int from, int to, int step = 1){
		DEASSERT_TRUE(step != 0)
		
		if(from < 0){
			from = pCount + from;
		}
		DEASSERT_TRUE(from >= 0)
		DEASSERT_TRUE(from < pCount)
		
		if(to < 0){
			to = pCount + to;
		}
		DEASSERT_TRUE(to >= 0)
		
		int i;
		if(step > 0){
			DEASSERT_TRUE(to <= pCount)
			
			for(i=from; i<to; i+=step){
				if(evaluator(pElements[i])){
					RemoveFrom(i);
					i--;
					to--;
				}
			}
			
		}else{
			DEASSERT_TRUE(to < pCount)
			
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
	void RemoveIf(Evaluator &&evaluator, int from, int to, int step = 1){
		RemoveIf<Evaluator>(evaluator, from, to, step);
	}
	
	template<typename Evaluator>
	void RemoveIf(Evaluator &evaluator, int from){
		if(from < 0){
			from = pCount + from;
		}
		DEASSERT_TRUE(from >= 0)
		DEASSERT_TRUE(from < pCount)
		
		int i;
		for(i=from; i<pCount; i++){
			if(evaluator(pElements[i])){
				RemoveFrom(i);
				i--;
			}
		}
	}
	
	template<typename Evaluator>
	void RemoveIf(Evaluator &&evaluator, int from){
		RemoveIf<Evaluator>(evaluator, from);
	}
	
	template<typename Evaluator>
	void RemoveIf(Evaluator &evaluator){
		int i, last = 0;
		for(i=0; i<pCount; i++){
			if(evaluator(pElements[i])){
				continue;
			}
			
			if(last < i){
				pElements[last] = pElements[i];
			}
			last++;
		}
		while(pCount > last){
			pElements[--pCount] = T();
		}
	}
	
	template<typename Evaluator>
	inline void RemoveIf(Evaluator &&evaluator){ RemoveIf<Evaluator>(evaluator); }
	
	
	/** \brief Sort elements in place. */
	template<typename Comparator>
	void Sort(Comparator &comparator){
		if(pCount > 1){
			pSort<Comparator>(comparator, 0, pCount - 1);
		}
	}
	
	template<typename Comparator>
	void Sort(Comparator &&comparator){
		Sort<Comparator>(comparator);
	}
	
	/** \brief Sort using decAscendingComparator. */
	void SortAscending(){
		Sort<decAscendingComparator<T>>(decAscendingComparator<T>());
	}
	
	/** \brief Sort using decDescendingComparator. */
	void SortDescending(){
		Sort<decDesendingComparator<T>>(decDesendingComparator<T>());
	}
	
	/** \brief Sort elements as new list. */
	template<typename Comparator>
	decTList<T,TP> GetSorted(Comparator &comparator) const{
		decTList<T,TP> copy(*this);
		copy.Sort<Comparator>(comparator);
		return copy;
	}
	
	template<typename Comparator>
	decTList<T,TP> GetSorted(Comparator &&comparator) const{
		return GetSorted<Comparator>(comparator);
	}
	
	/** \brief Sort as new list using decAscendingComparator. */
	decTList<T,TP> GetSortedAscending() const{
		return GetSorted<decAscendingComparator<T>>(decAscendingComparator<T>());
	}
	
	/** \brief Sort as new list using decDescendingComparator. */
	decTList<T,TP> GetSortedDescending() const{
		return GetSorted<decDesendingComparator<T>>(decDesendingComparator<T>());
	}
	/*@}*/
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Determine if this list is equal to another list. */
	bool operator==(const decTList<T,TP> &list) const{
		return Equals(list);
	}
	
	/** \brief Determine if this list is not equal to another list. */
	bool operator!=(const decTList<T,TP> &list) const{
		return !Equals(list);
	}
	
	/** \brief New list containing all values of this list followed by all values of another list. */
	decTList<T,TP> operator+(const decTList<T,TP> &list) const{
		decTList<T,TP> nlist(pCount + list.pCount);
		int i;
		
		for(i=0; i<pCount; i++){
			nlist.pElements[i] = pElements[i];
		}
		nlist.pCount = pCount;
		
		for(i=0; i<list.pCount; i++){
			nlist.pElements[pCount + i] = list.pElements[i];
		}
		nlist.pCount += list.pCount;
		
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
	decTList<T,TP> &operator=(const decTList<T,TP> &list){
		if(&list == this){
			return *this;
		}
		
		RemoveAll();
		
		if(list.pCount > pSize){
			T * const newArray = new T[list.pCount];
			if(pElements){
				delete [] pElements;
			}
			pElements = newArray;
			pSize = list.pCount;
		}
		
		for(pCount=0; pCount<list.pCount; pCount++){
			pElements[pCount] = list.pElements[pCount];
		}
		
		return *this;
	}
	
	/** \brief Move list. */
	decTList<T,TP> &operator=(decTList<T,TP> &&list) noexcept{
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
	decTList<T,TP> &operator+=(const decTList<T,TP> &list){
		if(list.pCount > 0){
			int count = pCount + list.pCount;
			
			if(count > pSize){
				T * const newArray = new T[count];
				if(pElements){
					int i;
					for(i=0; i<pSize; i++){
						newArray[i] = pElements[i];
					}
					delete [] pElements;
				}
				pElements = newArray;
				pSize = count;
			}
			
			int i;
			for(i=0; i<list.pCount; i++){
				pElements[pCount++] = list.pElements[i];
			}
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
		
		explicit const_iterator(const decTList<T,TP>* owner = nullptr, int index = 0) :
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
		const decTList<T,TP>* pOwner;
		int pIndex;
	};
	
	class iterator{
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using reference = T&;
		
		explicit iterator(decTList<T,TP>* owner = nullptr, int index = 0) :
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
		decTList<T,TP>* pOwner;
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
		const T pivot = pElements[left];
		const int r_hold = right;
		const int l_hold = left;
		
		while(left < right){
			while(left < right && comparator(pElements[right], pivot) >= 0){
				right--;
			}
			if(left != right){
				pElements[left] = pElements[right];
				left++;
			}
			while(left < right && comparator(pElements[left], pivot) <= 0){
				left++;
			}
			if(left != right){
				pElements[right] = pElements[left];
				right--;
			}
		}
		
		pElements[left] = pivot;
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
