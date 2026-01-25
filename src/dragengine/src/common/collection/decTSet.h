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

#include <algorithm>
#include <iterator>
#include <cstddef>
#include <utility>
#include <type_traits>

#include "decCollectionInterfaces.h"
#include "../exceptions_reduced.h"
#include "../../deTObjectReference.h"
#include "../../deTUniqueReference.h"
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
		
		std::copy_n(set.pElements, set.pCount, pElements);
		pCount = set.pCount;
	}
	
	/** \briev Move set. */
	decTSet(decTSet<T,TP> &&set) : pElements(set.pElements), pCount(set.pCount), pSize(set.pSize){
		set.pElements = nullptr;
		set.pCount = 0;
		set.pSize = 0;
	}
	
	/**
	 * \brief Create set with content provided by iterators.
	 * 
	 * Duplicate elements are ignored.
	 */
	template <std::input_iterator InputIt>
	decTSet(InputIt first, InputIt last) : pElements(nullptr), pCount(0), pSize(0){
		for(; first != last; ++first){
			Add(*first);
		}
	}
	
	/**
	 * \brief Create set with content from another collection.
	 * 
	 * Duplicate elements are ignored.
	 */
	template<typename C>
	explicit decTSet(const C &collection,
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
	
	/** \brief Create set with content from variable count of arguments. */
	template<typename... A>
	explicit decTSet(de_variadic_constructor_tag_t, A&&... args) : pElements(nullptr), pCount(0), pSize(0){
		(Add(std::forward<A>(args)), ...);
	}
	
	/** \brief Clean up the set. */
	~decTSet(){
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
	
	/** \brief Set is empty. */
	inline bool IsEmpty() const{
		return pCount == 0;
	}
	
	/** \brief Set is not empty. */
	inline bool IsNotEmpty() const{
		return pCount > 0;
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
	
	/** \brief Determine if element exists in the set. */
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
	typename std::enable_if<!std::is_same<U, TP>::value, bool>::type
	Has(const T &element) const{
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
	bool HasMatching(Evaluator &&evaluator) const{
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
	
	/** \brief Index of the first occurance of an element or -1 if not found. */
	int IndexOf(const TP &element) const{
		int p;
		
		for(p=0; p<pCount; p++){
			if(pElements[p] == element){
				return p;
			}
		}
		
		return -1;
	}
	
	template<typename U = T>
	typename std::enable_if<!std::is_same<U, TP>::value, int>::type
	IndexOf(const T &element) const{
		int p;
		
		for(p=0; p<pCount; p++){
			if(pElements[p] == element){
				return p;
			}
		}
		
		return -1;
	}
	
	/**
	 * \brief Add element if absent from the set.
	 * \returns true if added or false if already present.
	 */
	template<typename U = T>
	typename std::enable_if<std::is_assignable<U&, const TP&>::value, bool>::type
	Add(const TP &element){
		if(Has(element)){
			return false;
		}
		
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
		return true;
	}
	
	template<typename U = T>
	typename std::enable_if<!std::is_same<U, TP>::value, bool>::type
	Add(const T &element){
		if(Has(element)){
			return false;
		}
		
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
		return true;
	}
	
	/**
	 * \brief Add element or throw if already present.
	 * \throws deeInvalidParam \em element is present in the set.
	 */
	void AddOrThrow(const TP &element){
		DEASSERT_TRUE(Add(element))
	}
	
	template<typename U = T>
	typename std::enable_if<!std::is_same<U, TP>::value, void>::type
	AddOrThrow(const T &element){
		DEASSERT_TRUE(Add(element))
	}
	
	/** \brief Add all elements from another collection not present in set. */
	template <std::input_iterator InputIt>
	void AddAll(InputIt first, InputIt last){
		for(; first != last; ++first){
			Add(*first);
		}
	}
	
	/** \brief Add all elements from another collection or throw if any element is present. */
	template <std::input_iterator InputIt>
	void AddAllOrThrow(InputIt first, InputIt last){
		for(; first != last; ++first){
			AddOrThrow(*first);
		}
	}
	
	/** \brief Add all elements from another collection not present in set. */
	template<typename C>
	inline void AddAll(const C &collection){
		AddAll(collection.cbegin(), collection.cend());
	}
	
	/** \brief Add all elements from another collection or throw if any element is present. */
	template<typename C>
	inline void AddAllOrThrow(const C &collection){
		AddAllOrThrow(collection.cbegin(), collection.cend());
	}
	
	/**
	 * \brief Remove element if present in the set.
	 * \returns true removed or false if absent.
	 */
	bool Remove(const TP &element){
		const int position = IndexOf(element);
		if(position == -1){
			return false;
		}
		
		if(position < pCount - 1){
			pElements[position] = std::move(pElements[pCount - 1]);
		}
		pElements[--pCount] = T();
		return true;
	}
	
	template<typename U = T>
	typename std::enable_if<!std::is_same<U, TP>::value, bool>::type
	Remove(const T &element){
		const int position = IndexOf(element);
		if(position == -1){
			return false;
		}
		
		if(position < pCount - 1){
			pElements[position] = std::move(pElements[pCount - 1]);
		}
		pElements[--pCount] = T();
		return true;
	}
	
	/**
	 * \brief Remove element or throw if absent from the set.
	 * \throws deeInvalidParam \em element is not present in the set.
	 */
	inline void RemoveOrThrow(const TP &element){
		DEASSERT_TRUE(Remove(element))
	}
	
	template<typename U = T>
	typename std::enable_if<!std::is_same<U, TP>::value, void>::type
	RemoveOrThrow(const T &element){
		DEASSERT_TRUE(Remove(element))
	}
	
	/** \brief Remove all elements. */
	void RemoveAll(){
		while(pCount > 0){
			pElements[--pCount] = T();
		}
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
	 */
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
	 * \brief Visit elements with index.
	 * \param[in] visitor Visitor callable invoked as visitor(int, T).
	 */
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
	 * \brief Visit elements while evaluator returns true.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T).
	 */
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
	 * \brief Visit elements with index while evaluator returns true.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(int, T).
	 */
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
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T).
	 * \param[out] found Found element if true is returned.
	 */
	template<typename Evaluator>
	inline bool Find(const T* &found, Evaluator &evaluator) const{
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
	inline bool Find(const T* &found, Evaluator &&evaluator) const{
		return Find<Evaluator>(found, evaluator);
	}
	
	/**
	 * \brief Find element with default value.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T).
	 * \return Found element or default value if not found.
	 */
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
	 * \brief Collect element into a new list.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T).
	 */
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
	 * \brief Remove elements matching condition.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T).
	 */
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
		std::copy_n(pElements, pCount, nset.pElements);
		nset.pCount = pCount;
		
		int i;
		for(i=0; i<set.pCount; i++){
			nset.Add(set.pElements[i]);
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
		
		std::copy_n(set.pElements, set.pCount, pElements);
		pCount = set.pCount;
		
		return *this;
	}
	
	/** \brief Move set. */
	decTSet<T,TP> &operator=(decTSet<T,TP> &&set){
		if(&set == this){
			return *this;
		}
		
		RemoveAll();
		
		if(pElements){
			delete [] pElements;
		}
		
		pElements = set.pElements;
		pCount = set.pCount;
		pSize = set.pSize;
		
		set.pElements = nullptr;
		set.pCount = 0;
		set.pSize = 0;
		
		return *this;
	}
	
	/** \brief Append elements of set to this set. */
	decTSet<T,TP> &operator+=(const decTSet<T,TP> &set){
		if(set.pCount > 0){
			int count = pCount + set.pCount;
			
			if(count > pSize){
				T * const newArray = new T[count];
				if(pElements){
					std::move(pElements, pElements + pSize, newArray);
					delete [] pElements;
				}
				pElements = newArray;
				pSize = count;
			}
			
			int i;
			for(i=0; i<set.pCount; i++){
				Add(set.pElements[i]);
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
 * All objects including nullptr are allowed. Objects can be included only once in the set.
 * 
 * This template uses deTObjectReference.
 */
template<typename T>
using decTObjectSet = decTSet<deTObjectReference<T>, T*>;

/**
 * \brief Thread safe object set template class.
 * 
 * All objects including nullptr are allowed. Objects can be included only once in the set.
 * 
 * This template uses deTThreadSafeObjectReference to get thread safe assignment of
 * object references. This does not make the set itself thread safe though.
 */
template<typename T>
using decTThreadSafeObjectSet = decTSet<deTThreadSafeObjectReference<T>, T*>;

#endif
