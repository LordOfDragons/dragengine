/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DEOGLENVMAPSLOT_H_
#define _DEOGLENVMAPSLOT_H_

class deoglEnvironmentMap;



/**
 * @brief Environment Map Slot.
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
