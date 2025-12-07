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

// include only once
#ifndef _RERIGPUSHLIST_H_
#define _RERIGPUSHLIST_H_

// predefinitions
class reRigPush;



/**
 * @brief Rig Push List.
 * List of pushes.
 */
class reRigPushList{
private:
	reRigPush **pPushes;
	int pPushCount;
	int pPushSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rig push list. */
	reRigPushList();
	/** Cleans up the rig push list. */
	~reRigPushList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of pushs. */
	inline int GetPushCount() const{return pPushCount;}
	/** Retrieves the push at the given index. */
	reRigPush *GetPushAt(int index) const;
	/** Retrieves the index of the push or -1 if not found. */
	int IndexOfPush(reRigPush *push) const;
	/** Determines if the push exists. */
	bool HasPush(reRigPush *push) const;
	/** Adds a new push. */
	void AddPush(reRigPush *push);
	/** Removes the given push. */
	void RemovePush(reRigPush *push);
	/** Removes all pushs. */
	void RemoveAllPushes();
	/*@}*/
};

// end of include only once
#endif
