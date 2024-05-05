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

#ifndef _DEOGLENVMAPSLOT_H_
#define _DEOGLENVMAPSLOT_H_

class deoglEnvironmentMap;



/**
 * Environment Map Slot.
 */
class deoglEnvMapSlot{
private:
	deoglEnvironmentMap *pEnvMap; ///< weak reference
	bool pInUse;
	int pLastUsed;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new environment map slot. */
	deoglEnvMapSlot();
	/** Cleans up the environment map slot. */
	~deoglEnvMapSlot();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Clears the slot. */
	void Clear();
	
	/** Determines if the slot is in use. */
	inline bool GetInUse() const{ return pInUse; }
	/** Sets if the slot is in use. */
	void SetInUse( bool inUse );
	
	/** Retrieves the environment map or NULL if the slot is empty. */
	inline deoglEnvironmentMap *GetEnvMap() const{ return pEnvMap; }
	/** Sets the environment map or NULL if the slot is empty. */
	void SetEnvMap( deoglEnvironmentMap *envmap );
	
	/** Retrieves the last used. */
	inline int GetLastUsed() const{ return pLastUsed; }
	/** Resets the last used to 0. */
	void ResetLastUsed();
	/** Increments the last used by 1. */
	void IncrementLastUsed();
	/*@}*/
};

#endif
