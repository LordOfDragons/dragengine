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

#ifndef _DETTOBJECTORDEREDSET_H_
#define _DETTOBJECTORDEREDSET_H_

#include "decCollectionInterfaces.h"
#include "../exceptions.h"
#include "../../deObject.h"


/**
 * \brief Ordered object set template class.
 * 
 * All objects including nullptrptr are allowed. Objects can be included only once in the set.
 */
template<typename T>
class decTObjectOrderedSet{
private:
	T **pObjects;
	int pObjectCount, pObjectSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new set. */
	decTObjectOrderedSet(){
		pObjects = nullptr;
		pObjectCount = 0;
		pObjectSize = 0;
	}
	
	/**
	 * \brief Create a new set with initial capacity.
	 * \throws deeInvalidParam \em capacity is less than 0.
	 */
	explicit decTObjectOrderedSet(int capacity){
		DEASSERT_TRUE(capacity >= 0)
		
		pObjects = nullptr;
		pObjectCount = 0;
		pObjectSize = 0;
		
		if(capacity > 0){
			pObjects = new T*[capacity];
			pObjectSize = capacity;
		}
	}
	
	/** \brief Create copy of a set. */
	decTObjectOrderedSet(const decTObjectOrderedSet<T> &set){
		int count = set.GetCount();
		
		pObjects = nullptr;
		pObjectCount = 0;
		pObjectSize = 0;
		
		if(count > 0){
			T *object;
			
			pObjects = new T*[count];
			pObjectSize = count;
			
			for(pObjectCount=0; pObjectCount<count; pObjectCount++){
				object = set.pObjects[pObjectCount];
				pObjects[pObjectCount] = object;
				if(object){
					object->AddReference();
				}
			}
		}
	}
	
	/** \brief Clean up the set. */
	~decTObjectOrderedSet(){
		RemoveAll();
		
		if(pObjects){
			delete [] pObjects;
		}
	}
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of objects. */
	inline int GetCount() const{ return pObjectCount; }
	
	/** \brief List is empty. */
	inline bool IsEmpty() const{ return pObjectCount == 0; }
	
	/** \brief List is not empty. */
	inline bool IsNotEmpty() const{ return pObjectCount > 0; }
	
	/**
	 * \brief Object at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	T *GetAt(int index) const{
		DEASSERT_TRUE(index >= 0)
		DEASSERT_TRUE(index < pObjectCount)
		
		return pObjects[index];
	}
	
	/**
	 * \brief First object.
	 * \throws deeInvalidParam if list is empty.
	 */
	T *First() const{
		DEASSERT_TRUE(pObjectCount > 0)
		return pObjects[0];
	}
	
	/**
	 * \brief Last object.
	 * \throws deeInvalidParam if list is empty.
	 */
	T *Last() const{
		DEASSERT_TRUE(pObjectCount > 0)
		return pObjects[pObjectCount - 1];
	}
	
	/** \brief Index of the first occurance of an object or -1 if not found. */
	int IndexOf(const T *object) const{
		int p;
		
		for(p=0; p<pObjectCount; p++){
			if(object == pObjects[p]){
				return p;
			}
		}
		
		return -1;
	}
	
	/** \brief Determine if object exists in the list. */
	bool Has(const T *object) const{
		int p;
		
		for(p=0; p<pObjectCount; p++){
			if(object == pObjects[p]){
				return true;
			}
		}
		
		return false;
	}
	
	/**
	 * \brief Set object at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void SetAt(int index, T *object){
		DEASSERT_TRUE(index >= 0)
		DEASSERT_TRUE(index < pObjectCount)
		
		if(object == pObjects[index]){
			return;
		}
		
		DEASSERT_FALSE(Has(object))
		
		if(pObjects[index]){
			pObjects[index]->FreeReference();
		}
		
		pObjects[index] = object;
		
		if(object){
			object->AddReference();
		}
	}
	
	/**
	 * \brief Add object.
	 * \throws deeInvalidParam \em object is present in the set.
	 */
	void Add(T *object){
		DEASSERT_FALSE(Has(object))
		
		if(pObjectCount == pObjectSize){
			int newSize = pObjectSize * 3 / 2 + 1;
			T **newArray = new T*[newSize];
			if(pObjects){
				memcpy(newArray, pObjects, sizeof(T*) * pObjectSize);
				delete [] pObjects;
			}
			pObjects = newArray;
			pObjectSize = newSize;
		}
		
		pObjects[pObjectCount] = object;
		if(object){
			object->AddReference();
		}
		pObjectCount++;
	}
	
	/** \brief Add object if absent from the set. */
	void AddIfAbsent(T *object){
		if(Has(object)){
			return;
		}
		
		if(pObjectCount == pObjectSize){
			int newSize = pObjectSize * 3 / 2 + 1;
			T **newArray = new T*[newSize];
			if(pObjects){
				memcpy(newArray, pObjects, sizeof(T*) * pObjectSize);
				delete [] pObjects;
			}
			pObjects = newArray;
			pObjectSize = newSize;
		}
		
		pObjects[pObjectCount] = object;
		if(object){
			object->AddReference();
		}
		pObjectCount++;
	}
	
	/**
	 * \brief Insert object.
	 * \throws deeInvalidParam \em object is present in the set.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void Insert(T *object, int index){
		DEASSERT_FALSE(Has(object))
		DEASSERT_TRUE(index >= 0)
		DEASSERT_TRUE(index <= pObjectCount)
		
		int i;
		
		if(pObjectCount == pObjectSize){
			int newSize = pObjectSize * 3 / 2 + 1;
			T **newArray = new T*[newSize];
			if(pObjects){
				memcpy(newArray, pObjects, sizeof(T*) * pObjectSize);
				delete [] pObjects;
			}
			pObjects = newArray;
			pObjectSize = newSize;
		}
		
		for(i=pObjectCount; i>index; i--){
			pObjects[i] = pObjects[i - 1];
		}
		pObjects[index] = object;
		if(object){
			object->AddReference();
		}
		pObjectCount++;
	}
	
	/**
	 * \brief Move object.
	 * \throws deeInvalidParam \em to is less than 0 or larger than GetCount().
	 */
	void Move(const T *object, int to){
		const int from = IndexOf(object);
		DEASSERT_TRUE(from != -1)
		DEASSERT_TRUE(to >= 0)
		DEASSERT_TRUE(to < pObjectCount)
		
		T * const tempObject = pObjects[from];
		int i;
		
		if(to < from){
			for(i=from; i>to; i--){
				pObjects[i] = pObjects[i - 1];
			}
			
		}else if(to > from){
			for(i=from; i<to; i++){
				pObjects[i] = pObjects[i + 1];
			}
		}
		
		pObjects[to] = tempObject;
	}
	
	/**
	 * \brief Remove object.
	 * \throws deeInvalidParam \em object is is absent from the set.
	 */
	void Remove(const T *object){
		int p, position = IndexOf(object);
		
		DEASSERT_TRUE(position != -1)
		
		if(pObjects[position]){
			pObjects[position]->FreeReference();
		}
		for(p=position+1; p<pObjectCount; p++){
			pObjects[p - 1] = pObjects[p];
		}
		pObjectCount--;
	}
	
	/** \brief Remove object if present in the set. */
	void RemoveIfPresent(T *object){
		int p, position = IndexOf(object);
		
		if(position == -1){
			return;
		}
		
		if(pObjects[position]){
			pObjects[position]->FreeReference();
		}
		for(p=position+1; p<pObjectCount; p++){
			pObjects[p - 1] = pObjects[p];
		}
		pObjectCount--;
	}
	
	/**
	 * \brief Remove object from index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void RemoveFrom(int index){
		DEASSERT_TRUE(index >= 0)
		DEASSERT_TRUE(index < pObjectCount)
		
		if(pObjects[index]){
			pObjects[index]->FreeReference();
		}
		
		int i;
		for(i=index+1; i<pObjectCount; i++){
			pObjects[i - 1] = pObjects[i];
		}
		pObjectCount--;
	}
	
	/** \brief Remove all objects. */
	void RemoveAll(){
		while(pObjectCount > 0){
			pObjectCount--;
			if(pObjects[pObjectCount]){
				pObjects[pObjectCount]->FreeReference();
			}
		}
	}
	
	/** \brief Determine if this set is equal to another set. */
	bool Equals(const decTObjectOrderedSet<T> &set) const{
		int p;
		
		if(set.pObjectCount != pObjectCount){
			return false;
		}
		
		for(p=0; p<pObjectCount; p++){
			if(!set.Has(pObjects[p])){
				return false;
			}
		}
		
		return true;
	}
	
	/**
	 * \brief New set with the objects from the beginning of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decTObjectOrderedSet<T> GetHead(int count) const{
		DEASSERT_TRUE(count >= 0)
		
		if(count > pObjectCount){
			count = pObjectCount;
		}
		if(count == 0){
			return decTObjectOrderedSet<T>();
		}
		
		decTObjectOrderedSet<T> set(count);
		T *object;
		
		for(set.pObjectCount=0; set.pObjectCount<count; set.pObjectCount++){
			object = pObjects[set.pObjectCount];
			set.pObjects[set.pObjectCount] = object;
			if(object){
				object->AddReference();
			}
		}
		
		return set;
	}
	
	/**
	 * \brief Set set to objects from the beginning of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetHead(decTObjectOrderedSet<T> &set, int count) const{
		DEASSERT_TRUE(count >= 0)
		
		if(count > pObjectCount){
			count = pObjectCount;
		}
		
		T *object;
		
		if(count > set.pObjectSize){
			T **newArray = new T*[count];
			if(set.pObjects){
				delete [] set.pObjects;
			}
			set.pObjects = newArray;
			set.pObjectSize = count;
		}
		
		for(set.pObjectCount=0; set.pObjectCount<count; set.pObjectCount++){
			object = pObjects[set.pObjectCount];
			set.pObjects[set.pObjectCount] = object;
			if(object){
				object->AddReference();
			}
		}
	}
	
	/**
	 * \brief New set with objects from the end of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	decTObjectOrderedSet<T> GetTail(int count) const{
		DEASSERT_TRUE(count >= 0)
		
		if(count > pObjectCount){
			count = pObjectCount;
		}
		if(count == 0){
			return decTObjectOrderedSet<T>();
		}
		
		decTObjectOrderedSet<T> set(count);
		int from = pObjectCount - count;
		T *object;
		
		for(set.pObjectCount=0; set.pObjectCount<count; set.pObjectCount++){
			object = pObjects[from + set.pObjectCount];
			set.pObjects[set.pObjectCount] = object;
			if(object){
				object->AddReference();
			}
		}
		
		return set;
	}
	
	/**
	 * \brief Set set to objects from the end of this set.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void GetTail(decTObjectOrderedSet<T> &set, int count) const{
		DEASSERT_TRUE(count >= 0)
		
		if(count > pObjectCount){
			count = pObjectCount;
		}
		
		if(count > set.pObjectSize){
			T **newArray = new T*[count];
			if(set.pObjects){
				delete [] set.pObjects;
			}
			set.pObjects = newArray;
			set.pObjectSize = count;
		}
		
		int from = pObjectCount - count;
		T *object;
		
		for(set.pObjectCount=0; set.pObjectCount<count; set.pObjectCount++){
			object = pObjects[from + set.pObjectCount];
			set.pObjects[set.pObjectCount] = object;
			if(object){
				object->AddReference();
			}
		}
	}
	
	/**
	 * \brief New set with objects from the middle of this set.
	 * 
	 * \em from and \em to are clamped to the last index in the set if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 */
	decTObjectOrderedSet<T> GetMiddle(int from, int to) const{
		DEASSERT_TRUE(from >= 0)
		DEASSERT_TRUE(to >= from)
		
		int count = to - from + 1;
		if(count > pObjectCount){
			count = pObjectCount - from;
		}
		if(count == 0){
			return decTObjectOrderedSet<T>();
		}
		
		decTObjectOrderedSet<T> set(count);
		T *object;
		
		for(set.pObjectCount=0; set.pObjectCount<count; set.pObjectCount++){
			object = pObjects[from + set.pObjectCount];
			set.pObjects[set.pObjectCount] = object;
			if(object){
				object->AddReference();
			}
		}
		
		return set;
		
	}
	
	/**
	 * \brief Set set to objects from the middle of this set.
	 * 
	 * \em from and \em to are clamped to the last index in the set if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 */
	void GetMiddle(decTObjectOrderedSet<T> &set, int from, int to) const{
		DEASSERT_TRUE(from >= 0)
		DEASSERT_TRUE(to >= from)
		
		int count = to - from + 1;
		if(count > pObjectCount){
			count = pObjectCount - from;
		}
		T *object;
		
		if(count > set.pObjectSize){
			T **newArray = new T*[count];
			if(set.pObjects){
				delete [] set.pObjects;
			}
			set.pObjects = newArray;
			set.pObjectSize = count;
		}
		
		for(set.pObjectCount=0; set.pObjectCount<count; set.pObjectCount++){
			object = pObjects[from + set.pObjectCount];
			set.pObjects[set.pObjectCount] = object;
			if(object){
				object->AddReference();
			}
		}
	}
	
	/**
	 * \brief New set with objects from the middle of this set using a step size.
	 * 
	 * \em from and \em to are clamped to the last index in the set if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 * \throws deeInvalidParam \em step is less than 1.
	 */
	decTObjectOrderedSet<T> GetSliced(int from, int to, int step) const{
		if(step == 1){
			return GetMiddle(from, to);
			
		}else{
			// TODO Implementation
			DETHROW_INFO(deeInvalidAction, "Not implemented yet");
		}
	}
	
	/**
	 * \brief Set set to objects from the middle of this set using a step size.
	 * 
	 * \em from and \em to are clamped to the last index in the set if larger.
	 * 
	 * \throws deeInvalidParam \em from is less than 0.
	 * \throws deeInvalidParam \em to is less than \em from.
	 * \throws deeInvalidParam \em step is less than 1.
	 */
	void GetSliced(decTObjectOrderedSet<T> &set, int from, int to, int step) const{
		if(step == 1){
			GetMiddle(set, from, to);
			
		}else{
			// TODO Implementation
			DETHROW_INFO(deeInvalidAction, "Not implemented yet");
		}
	}
	
	
	
	/**
	 * \brief Visit objects.
	 * \param[in] visitor Visitor callable invoked as visitor(T*).
	 * \param[in] from First index to visit. Negative counts from end of list.
	 * \param[in] to One past last index to visit. Negative counts from end of list.
	 * \param[in] step Step size. Can be negative but not 0.
	 */
	template<typename Visitor>
	void Visit(Visitor &visitor, int from, int to, int step = 1) const {
		DEASSERT_TRUE(step != 0)
		
		if(from < 0){
			from = pObjectCount + from;
		}
		DEASSERT_TRUE(from >= 0)
		DEASSERT_TRUE(from < pObjectCount)
		
		if(to < 0){
			to = pObjectCount + to;
		}
		DEASSERT_TRUE(to >= 0)
		
		int i;
		if(step > 0){
			DEASSERT_TRUE(to <= pObjectCount)
			
			for(i=from; i<to; i+=step){
				visitor(pObjects[i]);
			}
			
		}else{
			DEASSERT_TRUE(to < pObjectCount)
			
			for(i=from; i>=to; i+=step){
				visitor(pObjects[i]);
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
			from = pObjectCount + from;
		}
		DEASSERT_TRUE(from >= 0)
		DEASSERT_TRUE(from < pObjectCount)
		
		int i;
		for(i=from; i<pObjectCount; i++){
			visitor(pObjects[i]);
		}
	}
	
	template<typename Visitor>
	void Visit(Visitor &&visitor, int from) const{
		Visit<Visitor>(visitor, from);
	}
	
	template<typename Visitor>
	void Visit(Visitor &visitor) const{
		int i;
		for(i=0; i<pObjectCount; i++){
			visitor(pObjects[i]);
		}
	}
	
	template<typename Visitor>
	inline void Visit(Visitor &&visitor) const{ Visit<Visitor>(visitor); }
	
	
	/**
	 * \brief Find object.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T*).
	 * \param[out] found Found object if true is returned.
	 * \param[in] from First index to visit. Negative counts from end of list.
	 * \param[in] to One past last index to visit. Negative counts from end of list.
	 * \param[in] step Step size. Can be negative but not 0.
	 */
	template<typename Evaluator>
	bool Find(Evaluator &evaluator, T *&found, int from, int to, int step = 1) const{
		DEASSERT_TRUE(step != 0)
		
		if(from < 0){
			from = pObjectCount + from;
		}
		DEASSERT_TRUE(from >= 0)
		DEASSERT_TRUE(from < pObjectCount)
		
		if(to < 0){
			to = pObjectCount + to;
		}
		DEASSERT_TRUE(to >= 0)
		
		int i;
		if(step > 0){
			DEASSERT_TRUE(to <= pObjectCount)
			
			for(i=from; i<to; i+=step){
				if(evaluator(pObjects[i])){
					found = pObjects[i];
					return true;
				}
			}
			
		}else{
			DEASSERT_TRUE(to < pObjectCount)
			
			for(i=from; i>=to; i+=step){
				if(evaluator(pObjects[i])){
					found = pObjects[i];
					return true;
				}
			}
		}
		return false;
	}
	
	template<typename Evaluator>
	bool Find(Evaluator &&evaluator, T *&found, int from, int to, int step = 1) const{
		return Find<Evaluator>(evaluator, found, from, to, step);
	}
	
	template<typename Evaluator>
	bool Find(Evaluator &evaluator, T *&found, int from) const{
		if(from < 0){
			from = pObjectCount + from;
		}
		DEASSERT_TRUE(from >= 0)
		DEASSERT_TRUE(from < pObjectCount)
		
		int i;
		for(i=from; i<pObjectCount; i++){
			if(evaluator(pObjects[i])){
				found = pObjects[i];
				return true;
			}
		}
		return false;
	}
	
	template<typename Evaluator>
	bool Find(Evaluator &&evaluator, T *&found, int from) const{
		return Find<Evaluator>(evaluator, found, from);
	}
	
	template<typename Evaluator>
	inline bool Find(Evaluator &evaluator, T *&found) const{
		int i;
		for(i=0; i<pObjectCount; i++){
			if(evaluator(pObjects[i])){
				found = pObjects[i];
				return true;
			}
		}
		return false;
	}
	
	template<typename Evaluator>
	inline bool Find(Evaluator &&evaluator, T *&found) const{
		return Find<Evaluator>(evaluator, found);
	}
	
	/**
	 * \brief Find object.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T*).
	 * \param[in] from First index to visit. Negative counts from end of list.
	 * \param[in] to One past last index to visit. Negative counts from end of list.
	 * \param[in] step Step size. Can be negative but not 0.
	 * \return Found object or nullptr if not found.
	 */
	template<typename Evaluator>
	T *Find(Evaluator &evaluator, int from, int to, int step = 1) const{
		T *found = nullptr;
		return Find<Evaluator>(evaluator, found, from, to, step) ? found : nullptr;
	}
	
	template<typename Evaluator>
	T *Find(Evaluator &&evaluator, int from, int to, int step = 1) const{
		return Find<Evaluator>(evaluator, from, to, step);
	}
	
	template<typename Evaluator>
	T *Find(Evaluator &evaluator, int from) const{
		T *found = nullptr;
		return Find<Evaluator>(evaluator, found, from) ? found : nullptr;
	}
	
	template<typename Evaluator>
	T *Find(Evaluator &&evaluator, int from) const{
		return Find<Evaluator>(evaluator, from);
	}
	
	template<typename Evaluator>
	inline T *Find(Evaluator &evaluator) const{
		T *found = nullptr;
		return Find<Evaluator>(evaluator, found) ? found : nullptr;
	}
	
	template<typename Evaluator>
	inline T *Find(Evaluator &&evaluator) const{
		return Find<Evaluator>(evaluator);
	}
	
	
	/**
	 * \brief Collect object into a new list.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T*).
	 * \param[in] from First index to visit. Negative counts from end of list.
	 * \param[in] to One past last index to visit. Negative counts from end of list.
	 * \param[in] step Step size. Can be negative but not 0.
	 */
	template<typename Evaluator>
	decTObjectOrderedSet<T> Collect(Evaluator &evaluator, int from, int to, int step = 1) const{
		DEASSERT_TRUE(step != 0)
		
		if(from < 0){
			from = pObjectCount + from;
		}
		DEASSERT_TRUE(from >= 0)
		DEASSERT_TRUE(from < pObjectCount)
		
		if(to < 0){
			to = pObjectCount + to;
		}
		DEASSERT_TRUE(to >= 0)
		
		decTObjectOrderedSet<T> collected;
		int i;
		if(step > 0){
			DEASSERT_TRUE(to <= pObjectCount)
			
			for(i=from; i<to; i+=step){
				if(evaluator(pObjects[i])){
					collected.Add(pObjects[i]);
				}
			}
			
		}else{
			DEASSERT_TRUE(to < pObjectCount)
			
			for(i=from; i>=to; i+=step){
				if(evaluator(pObjects[i])){
					collected.Add(pObjects[i]);
				}
			}
		}
		return collected;
	}
	
	template<typename Evaluator>
	decTObjectOrderedSet<T> Collect(Evaluator &&evaluator, int from, int to, int step = 1) const{
		return Collect<Evaluator>(evaluator, from, to, step);
	}
	
	template<typename Evaluator>
	decTObjectOrderedSet<T> Collect(Evaluator &evaluator, int from) const{
		if(from < 0){
			from = pObjectCount + from;
		}
		DEASSERT_TRUE(from >= 0)
		DEASSERT_TRUE(from < pObjectCount)
		
		decTObjectOrderedSet<T> collected;
		int i;
		for(i=from; i<pObjectCount; i++){
			if(evaluator(pObjects[i])){
				collected.Add(pObjects[i]);
			}
		}
		return collected;
	}
	
	template<typename Evaluator>
	decTObjectOrderedSet<T> Collect(Evaluator &&evaluator, int from) const{
		return Collect<Evaluator>(evaluator, from);
	}
	
	template<typename Evaluator>
	inline decTObjectOrderedSet<T> Collect(Evaluator &evaluator) const{
		decTObjectOrderedSet<T> collected;
		int i;
		for(i=0; i<pObjectCount; i++){
			if(evaluator(pObjects[i])){
				collected.Add(pObjects[i]);
			}
		}
		return collected;
	}
	
	template<typename Evaluator>
	inline decTObjectOrderedSet<T> Collect(Evaluator &&evaluator) const{
		return Collect<Evaluator>(evaluator);
	}
	
	
	/**
	 * \brief Remove objects matching condition.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(T*).
	 * \param[in] from First index to visit. Negative counts from end of list.
	 * \param[in] to One past last index to visit. Negative counts from end of list.
	 * \param[in] step Step size. Can be negative but not 0.
	 */
	template<typename Evaluator>
	void RemoveIf(Evaluator &evaluator, int from, int to, int step = 1){
		DEASSERT_TRUE(step != 0)
		
		if(from < 0){
			from = pObjectCount + from;
		}
		DEASSERT_TRUE(from >= 0)
		DEASSERT_TRUE(from < pObjectCount)
		
		if(to < 0){
			to = pObjectCount + to;
		}
		DEASSERT_TRUE(to >= 0)
		
		int i;
		if(step > 0){
			DEASSERT_TRUE(to <= pObjectCount)
			
			for(i=from; i<to; i+=step){
				if(evaluator(pObjects[i])){
					RemoveFrom(i);
					i--;
					to--;
				}
			}
			
		}else{
			DEASSERT_TRUE(to < pObjectCount)
			
			for(i=from; i>=to; i+=step){
				if(evaluator(pObjects[i])){
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
			from = pObjectCount + from;
		}
		DEASSERT_TRUE(from >= 0)
		DEASSERT_TRUE(from < pObjectCount)
		
		int i;
		for(i=from; i<pObjectCount; i++){
			if(evaluator(pObjects[i])){
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
		int i;
		for(i=0; i<pObjectCount; i++){
			if(evaluator(pObjects[i])){
				RemoveFrom(i);
				i--;
			}
		}
	}
	
	template<typename Evaluator>
	inline void RemoveIf(Evaluator &&evaluator){ RemoveIf<Evaluator>(evaluator); }
	
	
	/** \brief Sort objects in place. */
	template<typename Comparator>
	void Sort(Comparator &comparator){
		if(pObjectCount > 1){
			pSort<Comparator>(comparator, 0, pObjectCount - 1);
		}
	}
	
	template<typename Comparator>
	void Sort(Comparator &&comparator){
		Sort<Comparator>(comparator);
	}
	
	/** \brief Sort objects as new list. */
	template<typename Comparator>
	decTObjectOrderedSet<T> GetSorted(Comparator &comparator) const{
		decTObjectOrderedSet<T> copy(*this);
		copy.Sort<Comparator>(comparator);
		return copy;
	}
	
	template<typename Comparator>
	decTObjectOrderedSet<T> GetSorted(Comparator &&comparator) const{
		return GetSorted<Comparator>(comparator);
	}
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Determine if this set is equal to another set. */
	bool operator==(const decTObjectOrderedSet<T> &set) const{
		return Equals(set);
	}
	
	/** \brief New set containing all objects of this set followed by all objects of another set. */
	decTObjectOrderedSet<T> operator+(const decTObjectOrderedSet<T> &set) const{
		decTObjectOrderedSet<T> nset(pObjectCount + set.pObjectCount);
		T *object;
		int i;
		
		for(i=0; i<pObjectCount; i++){
			object = pObjects[i];
			nset.pObjects[i] = object;
			if(object){
				object->AddReference();
			}
		}
		
		for(i=0; i<set.pObjectCount; i++){
			nset.AddIfAbsent(set.pObjects[i]);
		}
		
		return nset;
	}
	
	/**
	 * \brief Object at index.
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	T *operator[](int index) const{
		return GetAt(index);
	}
	
	/** \brief Copy set to this set. */
	decTObjectOrderedSet<T> &operator=(const decTObjectOrderedSet<T> &set){
		if(&set == this){
			return *this;
		}
		
		T *object;
		
		RemoveAll();
		
		if(set.pObjectCount > pObjectSize){
			T **newArray = new T*[set.pObjectCount];
			if(pObjects){
				delete [] pObjects;
			}
			pObjects = newArray;
			pObjectSize = set.pObjectCount;
		}
		
		for(pObjectCount=0; pObjectCount<set.pObjectCount; pObjectCount++){
			object = set.pObjects[pObjectCount];
			pObjects[pObjectCount] = object;
			if(object){
				object->AddReference();
			}
		}
		
		return *this;
	}
	
	/** \brief Append objects of set to this set. */
	decTObjectOrderedSet<T> &operator+=(const decTObjectOrderedSet<T> &set){
		if(set.pObjectCount > 0){
			int i, count = pObjectCount + set.pObjectCount;
			
			if(count > pObjectSize){
				T **newArray = new T*[count];
				if(pObjects){
					memcpy(newArray, pObjects, sizeof(T*) * pObjectSize);
					delete [] pObjects;
				}
				pObjects = newArray;
				pObjectSize = count;
			}
			
			for(i=0; i<set.pObjectCount; i++){
				AddIfAbsent(set.pObjects[i]);
			}
		}
		
		return *this;
	}
	/*@}*/
	
	
private:
	template<typename Comparator>
	void pSort(Comparator &comparator, int left, int right){
		T * const pivot = pObjects[left];
		const int r_hold = right;
		const int l_hold = left;
		
		while(left < right){
			while(left < right && comparator(pObjects[right], pivot) >= 0){
				right--;
			}
			if(left != right){
				pObjects[left] = pObjects[right];
				left++;
			}
			while(left < right && comparator(pObjects[left], pivot) <= 0){
				left++;
			}
			if(left != right){
				pObjects[right] = pObjects[left];
				right--;
			}
		}
		
		pObjects[left] = pivot;
		if(l_hold < left){
			pSort<Comparator>(comparator, l_hold, left - 1);
		}
		if(r_hold > left){
			pSort<Comparator>(comparator, left + 1, r_hold);
		}
	}
};

#endif
