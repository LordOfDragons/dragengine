/* 
 * Drag[en]gine Windows Input Module
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

#ifndef _DEWIDEVICEFEEDBACK_H_
#define _DEWIDEVICEFEEDBACK_H_

#include "dewiInclude.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputDeviceFeedback.h>
#include <dragengine/resources/image/deImageReference.h>

class deInputDeviceFeedback;
class deWindowsInput;


/**
 * \brief Windows input device feedback.
 */
class dewiDeviceFeedback : public deObject{
private:
	deWindowsInput &pModule;
	
	int pIndex;
	decString pID;
	decString pName;
	deInputDeviceFeedback::eFeedbackTypes pType;
	
	deImageReference pDisplayImage;
	decObjectOrderedSet pDisplayIcons;
	decString pDisplayText;
	
	int pMaximum;
	
	float pValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create device feedback. */
	dewiDeviceFeedback( deWindowsInput &module );
	
protected:
	/** \brief Clean up device feedback. */
	virtual ~dewiDeviceFeedback();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** \brief Input module. */
	inline deWindowsInput &GetModule() const{ return pModule; }
	
	/** \brief Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** \brief Set index. */
	void SetIndex( int index );
	
	/** \brief Identifier. */
	inline const decString &GetID() const{ return pID; }
	
	/** \brief Set identifier. */
	void SetID( const char *id );
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief Type. */
	inline deInputDeviceFeedback::eFeedbackTypes GetType() const{ return pType; }
	
	/** \brief Set type. */
	void SetType( deInputDeviceFeedback::eFeedbackTypes type );
	
	
	
	/** \brief Display image. */
	inline deImage *GetDisplayImage() const{ return pDisplayImage; }
	
	/** \brief Display icons (deImage*). */
	inline const decObjectOrderedSet &GetDisplayIcons() const{ return pDisplayIcons; }
	
	/** \brief Set display image and icons. */
	void SetDisplayImages( const char *name );
	
	/** \brief Display text. */
	inline const decString &GetDisplayText() const{ return pDisplayText; }
	
	/** \brief Set display text. */
	void SetDisplayText( const char *text );
	
	
	
	/** \brief Maximum value. */
	inline int GetMaximum() const{ return pMaximum; }
	
	/** \brief Set maximum value. */
	void SetMaximum( int maximum );
	
	
	
	/** \brief Current value. */
	inline float GetValue() const{ return pValue; }
	
	/** \brief Set current value. */
	void SetValue( float value );
	
	
	
	/** \brief Update engine input device information feedback. */
	void GetInfo( deInputDeviceFeedback &info ) const;
	/*@}*/
};

#endif
