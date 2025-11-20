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

#ifndef _MECLHITLIST_H_
#define _MECLHITLIST_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>

class meCLHitListEntry;


/**
 * \brief List of elements hit during a test.
 */
class meCLHitList{
public:
	class Comparator{
	public:
		Comparator() = default;
		
		virtual int operator() (meCLHitListEntry &a, meCLHitListEntry &b) = 0;
	};
	
	class Visitor{
	public:
		Visitor() = default;
		
		virtual void operator() (meCLHitListEntry &entry) = 0;
	};
	
	class Evaluator{
	public:
		Evaluator() = default;
		
		virtual bool operator() (meCLHitListEntry &entry) = 0;
	};
	
	
private:
	decObjectOrderedSet pEntries;
	
	
public:
	/** \name Constructors, Destructors */
	/*@{*/
	/** \brief Create hit list. */
	meCLHitList() = default;
	
	/** \brief Clean up hit list. */
	~meCLHitList() = default;
	/*@}*/
	
	
	/** \name Entries */
	/*@{*/
	/** \brief Number of entries. */
	int GetEntryCount() const;
	
	/** \brief Entry at index. */
	meCLHitListEntry *GetEntryAt(int index) const;
	
	/** \brief Index of entry or -1 if absent. */
	int IndexOfEntry(meCLHitListEntry *entry) const;
	
	/** \brief Entry is present. */
	bool HasEntry(meCLHitListEntry *entry) const;
	
	/** \brief Add entry. */
	void AddEntry(meCLHitListEntry *entry);
	
	/** \brief Remove entry. */
	void RemoveEntry(meCLHitListEntry *entry);
	
	/** \brief Remove all entries. */
	void RemoveAllEntries();
	
	
	/** \brief Visit objects. */
	void Visit(Visitor &visitor, int from, int to = -1, int step = 1) const;
	
	inline void Visit(Visitor &visitor) const{ Visit(visitor, 0, pEntries.GetCount()); }
	
	/** \brief Find object. */
	bool Find(Evaluator &evaluator, deObject *&found, int from, int to = -1, int step = 1) const;
	
	inline bool Find(Evaluator &evaluator, deObject *&found) const{
		return Find(evaluator, found, 0, pEntries.GetCount());
	}
	
	/** \brief Find hit element matching same object. */
	meCLHitListEntry *FindSame(const meCLHitListEntry &entry) const;
	
	/** \brief Collect object into a new list. */
	meCLHitList Collect(Evaluator &evaluator, int from, int to = -1, int step = 1) const;
	
	inline meCLHitList Collect(Evaluator &evaluator) const{
		return Collect(evaluator, 0, pEntries.GetCount());
	}
	
	/** \brief Remove objects matching condition. */
	void RemoveIf(Evaluator &evaluator, int from, int to = -1, int step = 1);
	
	inline void RemoveIf(Evaluator &evaluator){
		RemoveIf(evaluator, 0, pEntries.GetCount());
	}
	
	/** \brief Remove duplicates. */
	void RemoveDuplicates();
	
	/** \brief Sort objects in place. */
	void Sort(Comparator &comparator);
	
	/** \brief Sort entries by distance. */
	void SortByDistance();
	/*@}*/
};

#endif
