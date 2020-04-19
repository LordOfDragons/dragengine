/* 
 * Drag[en]gine Android Input Module
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

#ifndef _DEAIINPUTBINDING_H_
#define _DEAIINPUTBINDING_H_

#include <dragengine/common/string/decString.h>

class deaiDeviceManager;
class deAndroidInput;



/**
 * \brief Input binding.
 */
class deaiInputBinding{
private:
	decString pDeviceID;
	decString pAxisID;
	decString pButtonID;
	bool pInvertAxis;
	int pDeviceIndex;
	int pAxisIndex;
	int pButtonIndex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create input binding. */
	deaiInputBinding();
	
	/** \brief Create button input binding. */
	deaiInputBinding( const char *deviceID, const char *buttonID );
	
	/** \brief Create axis input binding. */
	deaiInputBinding( const char *deviceID, const char *axisID, bool invertAxis );
	
	/** \brief Create copy of input binding. */
	deaiInputBinding( const deaiInputBinding &binding );
	
	/** \brief Clean up overlay. */
	~deaiInputBinding();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Device identifier. */
	inline const decString &GetDeviceID() const{ return pDeviceID; }
	
	/** \brief Set device identifier. */
	void SetDeviceID( const char *id );
	
	/** \brief Axis identifier or empty string. */
	inline const decString &GetAxisID() const{ return pAxisID; }
	
	/** \brief Set axis identifier or empty string. */
	void SetAxisID( const char *id );
	
	/** \brief Button identifier or empty string. */
	inline const decString &GetButtonID() const{ return pButtonID; }
	
	/** \brief Set button identifier or empty string. */
	void SetButtonID( const char *id );
	
	/** \brief Invert axis. */
	inline bool GetInvertAxis() const{ return pInvertAxis; }
	
	/** \brief Set invert axis. */
	void SetInvertAxis( bool invertAxis );
	
	/** \brief Device index or -1 if not matching. */
	inline int GetDeviceIndex() const{ return pDeviceIndex; }
	
	/** \brief Axis index or -1 if not matching. */
	inline int GetAxisIndex() const{ return pAxisIndex; }
	
	/** \brief Button index or -1 if not matching. */
	inline int GetButtonIndex() const{ return pButtonIndex; }
	
	
	
	/** \brief Update indices. */
	void UpdateIndices( const deaiDeviceManager &devices );
	
	/** \brief Change button state and send event if valid. */
	void ChangeButtonState( deAndroidInput &androidInput, bool pressed ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Bindings are equal. */
	bool operator==( const deaiInputBinding &binding ) const;
	
	/** \brief Assign binding. */
	deaiInputBinding &operator=( const deaiInputBinding &binding );
	/*@}*/
};

#endif
