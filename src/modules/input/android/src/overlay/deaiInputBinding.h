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
