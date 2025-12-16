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

#ifndef _DECTSET_H_
#define _DECTSET_H_

#include <iterator>
#include <cstddef>

#include "decCollectionInterfaces.h"
#include "../exceptions.h"
#include "../../deTObjectReference.h"
#include "../../threading/deTThreadSafeObjectReference.h"


/**
 * \brief Set template class.
 * 
 * All elements including default constructed values are allowed and they can occure only once in the set.
 */
template<typename T, typename TP = T>
class decTSet{
private:
	T *pElements;
	int pCount, pSize;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new set. */
	decTSet() : pElements(nullptr), pCount(0), pSize(0){
	}
	
	/**
	 * \brief Create a new set with initial capacity.
	 * \throws deeInvalidParam \em capacity is less than 0.
	 */
	explicit decTSet(int capacity) : pElements(nullptr), pCount(0), pSize(0){
		DEASSERT_TRUE(capacity >= 0)
		
		if(capacity > 0){
			pElements = new T[capacity];
			pSize = capacity;
		}
	}
	
	/** \brief Create copy of a set. */
	decTSet(const decTSet<T,TP> &set) : pElements(nullptr), pCount(0), pSize(0){
		if(set.pCount == 0){
			return;
		}
		
		pElements = new T[set.pCount];
		pSize = set.pCount;
		
		for(pCount=0; pCount<set.pCount; pCount++){
			pElements[pCount] = set.pElements[pCount];
		}
	}
	
	/** \brief Clean up the set. */
	~decTSet(){
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
	
	/** \brief Set is empty. */
	inline bool IsEmpty() const{ return pCount == 0; }
	
	/** \brief Set is not empty. */
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
	 * \brief First element.
	 * \throws deeInvalidParam if set is empty.
	 */
	const T &First() const{
		DEASSERT_TRUE(pCount > 0)
		return pElements[0];
	}
	
	/**
	 * \brief Last element.
	 * \throws deeInvalidParam if set is empty.
	 */
	const T &Last() const{
		DEASSERT_TRUE(pCount > 0)
		return pElements[pCount - 1];
	}
	
	/** \brief Determine if element exists in the set. */
	bool Has(const TP &element) const{
		int p;
		
		for(p=0; p<pCount; p++){
			if(element == pElements[p]){
				return true;
			}
		}
		
		return false;
	}
	
	/** \brief Index of the first occurance of an element or -1 if not found. */
	int IndexOf(const TP &element) const{
		int p;
		
		for(p=0; p<pCount; p++){
			if(element == pElements[p]){
				return p;
			}
		}
		
		return -1;
	}
	
	/**
	 * \brief Add element.
	 * \throws deeInvalidParam \em element is present in the set.
	 */
	void Add(const TP &element){
		DEASSERT_TRUE(AddIfAbsent(element))
	}
	
	/** \brief Add element if absent from the set. */
	bool AddIfAbsent(const TP &element){
		if(Has(element)){
			return false;
		}
		
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
		return true;
	}
	
	/**
	 * \brief Remove element.
	 * \throws deeInvalidParam \em element is is absent from the set.
	 */
	void Remove(const TP &element){
		DEASSERT_TRUE(RemoveIfPresent(element))
	}
	
	/** \brief Remove element if present in the set. */
	bool RemoveIfPresent(const TP &element){
		const int position = IndexOf(element);
		if(position == -1){
			return false;
		}
		
		int p;
		for(p=position+1; p<pCount; p++){
			pElements[p - 1] = pElements[p];
		}
		pCount--;
		pElements[pCount] = T();
		return true;
	}
	
	/** \brief Remove all elements. */
	void RemoveAll(){
		int i;
		for(i=0; i<pCount; i++){
			pElements[i] = T();
		}
		pCount = 0;
	}
	
	/** \brief Determine if this set is equal to another set. */
	bool Equals(const decTSet<T,TP> &set) const{
		int p;
		
		if(set.pCount != pCount){
			return false;
		}
		
		for(p=0; p<pCount; p++){
			if(!set.Has(pElements[p])){
				return false;
			}
		}
		
		return true;
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
	
	/**
	 * \brief Find element with default value.
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
	inline T FindOrDefault(Evaluator &evaluator, const T &defaultValue) const{
		const T *found = nullptr;
		return Find<Evaluator>(evaluator, found) ? *found : defaultValue;
	}
	
	template<typename Evaluator>
	inline T FindOrDefault(Evaluator &&evaluator, const T &defaultValue) const{
		return FindOrDefault<Evaluator>(evaluator, defaultValue);
	}
	
	
	/**
	 * \brief Collect element into a new list.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T).
	 * \param[in] from First index to visit. Negative counts from end of list.
	 * \param[in] to One past last index to visit. Negative counts from end of list.
	 * \param[in] step Step size. Can be negative but not 0.
	 */
	template<typename Evaluator>
	decTSet<T,TP> Collect(Evaluator &evaluator, int from, int to, int step = 1) const{
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
		
		decTSet<T,TP> collected;
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
	decTSet<T,TP> Collect(Evaluator &&evaluator, int from, int to, int step = 1) const{
		return Collect<Evaluator>(evaluator, from, to, step);
	}
	
	template<typename Evaluator>
	decTSet<T,TP> Collect(Evaluator &evaluator, int from) const{
		if(from < 0){
			from = pCount + from;
		}
		DEASSERT_TRUE(from >= 0)
		DEASSERT_TRUE(from < pCount)
		
		decTSet<T,TP> collected;
		int i;
		for(i=from; i<pCount; i++){
			if(evaluator(pElements[i])){
				collected.Add(pElements[i]);
			}
		}
		return collected;
	}
	
	template<typename Evaluator>
	decTSet<T,TP> Collect(Evaluator &&evaluator, int from) const{
		return Collect<Evaluator>(evaluator, from);
	}
	
	template<typename Evaluator>
	inline decTSet<T,TP> Collect(Evaluator &evaluator) const{
		decTSet<T,TP> collected;
		int i;
		for(i=0; i<pCount; i++){
			if(evaluator(pElements[i])){
				collected.Add(pElements[i]);
			}
		}
		return collected;
	}
	
	template<typename Evaluator>
	inline decTSet<T,TP> Collect(Evaluator &&evaluator) const{
		return Collect<Evaluator>(evaluator);
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
					RemoveIfPresent(pElements[i]);
					i--;
					to--;
				}
			}
			
		}else{
			DEASSERT_TRUE(to < pCount)
			
			for(i=from; i>=to; i+=step){
				if(evaluator(pElements[i])){
					RemoveIfPresent(pElements[i]);
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
				RemoveIfPresent(pElements[i]);
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
		int i;
		for(i=0; i<pCount; i++){
			if(evaluator(pElements[i])){
				RemoveIfPresent(pElements[i]);
				i--;
			}
		}
	}
	
	template<typename Evaluator>
	inline void RemoveIf(Evaluator &&evaluator){ RemoveIf<Evaluator>(evaluator); }
	/*@}*/
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Determine if this set is equal to another set. */
	bool operator==(const decTSet<T,TP> &set) const{
		return Equals(set);
	}
	
	/** \brief Determine if this set is not equal to another set. */
	bool operator!=(const decTSet<T,TP> &set) const{
		return !Equals(set);
	}
	
	/** \brief New set containing all elements of this set followed by all elements of another set. */
	decTSet<T,TP> operator+(const decTSet<T,TP> &set) const{
		decTSet<T,TP> nset(pCount + set.pCount);
		int i;
		
		for(i=0; i<pCount; i++){
			nset.pElements[i] = pElements[i];
		}
		nset.pCount = pCount;
		
		for(i=0; i<set.pCount; i++){
			nset.AddIfAbsent(set.pElements[i]);
		}
		
		return nset;
	}
	
	/**
	 * \brief Element at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	const T &operator[](int index) const{
		return GetAt(index);
	}
	
	/** \brief Copy set to this set. */
	decTSet<T,TP> &operator=(const decTSet<T,TP> &set){
		if(&set == this){
			return *this;
		}
		
		RemoveAll();
		
		if(set.pCount > pSize){
			T * const newArray = new T[set.pCount];
			if(pElements){
				delete [] pElements;
			}
			pElements = newArray;
			pSize = set.pCount;
		}
		
		for(pCount=0; pCount<set.pCount; pCount++){
			pElements[pCount] = set.pElements[pCount];
		}
		
		return *this;
	}
	
	/** \brief Append elements of set to this set. */
	decTSet<T,TP> &operator+=(const decTSet<T,TP> &set){
		if(set.pCount > 0){
			int count = pCount + set.pCount;
			
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
			for(i=0; i<set.pCount; i++){
				AddIfAbsent(set.pElements[i]);
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
		
		explicit const_iterator(const decTSet<T,TP>* owner = nullptr, int index = 0) :
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
		const decTSet<T,TP>* pOwner;
		int pIndex;
	};
	
	
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	
	
	/** Standard library iterator pointing at first element in set */
	const_iterator begin() const{
		return cbegin();
	}
	const_iterator cbegin() const {
		return const_iterator(this, 0);
	}
	
	/** Standard library iterator pointing at one past last element in set */
	const_iterator end() const {
		return cend();
	}
	const_iterator cend() const {
		return const_iterator(this, pCount);
	}
	
	/** Standard library reverse iterator pointing at one past last element in set */
	const_reverse_iterator rbegin() const{
		return const_reverse_iterator(end());
	}
	const_reverse_iterator crbegin() const {
		return const_reverse_iterator(cend());
	}
	
	/** Standard library reverse iterator pointing at first element in set */
	const_reverse_iterator rend() const{
		return const_reverse_iterator(begin());
	}
	const_reverse_iterator crend() const{
		return const_reverse_iterator(cbegin());
	}
	/*@}*/
};



/**
 * \brief Object set template class.
 * 
 * All objects including nullptr are allowed. Objects can be included more than once in the set.
 * 
 * This template uses deTObjectReference.
 */
template<typename T>
using decTObjectSet = decTSet<deTObjectReference<T>, T*>;

/**
 * \brief Thread safe object set template class.
 * 
 * All objects including nullptr are allowed. Objects can be included more than once in the set.
 * 
 * This template uses deTThreadSafeObjectReference to get thread safe assignment of
 * object references. This does not make the set itself thread safe though.
 */
template<typename T>
using decTThreadSafeObjectSet = decTSet<deTThreadSafeObjectReference<T>, T*>;

#endif
