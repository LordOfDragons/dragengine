/* 
 * Drag[en]gine OpenVR VR Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOVRDEVICEBUTTON_H_
#define _DEOVRDEVICEBUTTON_H_

#include <stdint.h>

#include <openvr/openvr.h>

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/resources/image/deImage.h>

class deInputDeviceButton;
class deovrDevice;


/**
 * VR input device button.
 */
class deovrDeviceButton : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deovrDeviceButton> Ref;
	
	
	
private:
	deovrDevice &pDevice;
	
	int pIndex;
	
	const vr::EVRButtonId pButtonType;
	const uint64_t pButtonMask;
	
	decString pID;
	decString pName;
	bool pPressed;
	
	deImage::Ref pDisplayImage;
	decObjectOrderedSet pDisplayIcons;
	decString pDisplayText;
	
	bool pDirtyValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device button. */
	deovrDeviceButton( deovrDevice &device, vr::EVRButtonId buttonType );
	
protected:
	/** Clean up device button. */
	virtual ~deovrDeviceButton();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** Device. */
	inline deovrDevice &GetDevice() const{ return pDevice; }
	
	/** Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** Set index. */
	void SetIndex( int index );
	
	
	
	/** Button type. */
	inline vr::EVRButtonId GetButtonType() const{ return pButtonType; }
	
	/** Button mask. */
	inline uint64_t GetButtonMask() const{ return pButtonMask; }
	
	
	
	/** Identifier. */
	inline const decString &GetID() const{ return pID; }
	
	/** Set identifier. */
	void SetID( const char *id );
	
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Set name. */
	void SetName( const char *name );
	
	/** Button is pressed. */
	inline bool GetPressed() const{ return pPressed; }
	
	/** Set if button is presssed. */
	void SetPressed( bool pressed );
	
	
	
	/** Display image. */
	inline deImage *GetDisplayImage() const{ return pDisplayImage; }
	
	/** Display icons (deImage*). */
	inline const decObjectOrderedSet &GetDisplayIcons() const{ return pDisplayIcons; }
	
	/** Set display image and icons. */
	void SetDisplayImages( const char *name );
	
	/** Set display image and icons. */
	void SetDisplayImages( const deovrDeviceButton &button );
	
	/** Display text. */
	inline const decString &GetDisplayText() const{ return pDisplayText; }
	
	/** Set display text. */
	void SetDisplayText( const char *text );
	
	
	
	/** Update engine input device information button. */
	void GetInfo( deInputDeviceButton &info ) const;
	/*@}*/
};

#endif
