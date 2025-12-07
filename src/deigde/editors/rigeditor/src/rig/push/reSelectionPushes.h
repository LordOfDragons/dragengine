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
#ifndef _RESELECTIONPUSHES_H_
#define _RESELECTIONPUSHES_H_

// predefinitions
class reRig;
class reRigPush;
class reRigPushList;
class deColliderVolume;



/**
 * @brief Rig Push Selection.
 * Stores a list of selected rig pushes as well as the active rig push.
 */
class reSelectionPushes{
private:
	reRig *pRig;
	
	reRigPush **pPushes;
	int pPushCount, pPushSize;
	reRigPush *pActivePush;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rig push selection. */
	reSelectionPushes(reRig *rig);
	/** Cleans up the rig push selection. */
	~reSelectionPushes();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the parent rig. */
	inline reRig *GetParentRig() const{return pRig;}
	
	/** Retrieves the number of selected pushs. */
	inline int GetPushCount() const{return pPushCount;}
	/** Retrieves the push at the given index. */
	reRigPush *GetPushAt(int index) const;
	/** Determines if the given push exists. */
	bool HasPush(reRigPush *push) const;
	/** Retrieves the index of the given push or -1 if not found. */
	int IndexOfPush(reRigPush *push) const;
	/** Retrieves the index of the push with the given collider or -1 if not found. */
	int IndexOfPushWith(deColliderVolume *collider) const;
	/** Adds a push if not existing already. */
	void AddPush(reRigPush *push);
	/** Removes a push if existing. */
	void RemovePush(reRigPush *push);
	/** Removes all pushs. */
	void RemoveAllPushes();
	
	/** Retrieves the active push or NULL. */
	inline reRigPush *GetActivePush() const{return pActivePush;}
	/** Determines if an active push exists. */
	bool HasActivePush() const;
	/** Sets the acitve push or NULL. */
	void SetActivePush(reRigPush *push);
	/** Activates the next push in the list. */
	//void ActivateNextPush();
	
	/** Removes all pushs and sets the active push to NULL. */
	void Reset();
	
	/** Adds all selected and visible pushs to the given list. */
	void AddVisiblePushesTo(reRigPushList &list) const;
	/*@}*/
};

// end of include only once
#endif
