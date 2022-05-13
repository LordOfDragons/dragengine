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

#ifndef _DEOVRDEVICECOMPONENT_H_
#define _DEOVRDEVICECOMPONENT_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputDeviceComponent.h>
#include <dragengine/resources/image/deImage.h>

class deovrDevice;


/**
 * VR input device component.
 */
class deovrDeviceComponent : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deovrDeviceComponent> Ref;
	
	
	
private:
	deovrDevice &pDevice;
	
	int pIndex;
	
	decString pID;
	decString pName;
	deInputDeviceComponent::eComponentTypes pType;
	
	deImage::Ref pDisplayImage;
	decObjectOrderedSet pDisplayIcons;
	decString pDisplayText;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device component. */
	deovrDeviceComponent( deovrDevice &device );
	
protected:
	/** Clean up device component. */
	virtual ~deovrDeviceComponent();
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
	
	
	
	/** Identifier. */
	inline const decString &GetID() const{ return pID; }
	
	/** Set identifier. */
	void SetID( const char *id );
	
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Set name. */
	void SetName( const char *name );
	
	/** Type. */
	inline deInputDeviceComponent::eComponentTypes GetType() const{ return pType; }
	
	/** Set type. */
	void SetType( deInputDeviceComponent::eComponentTypes type );
	
	
	
	/** Display image. */
	inline deImage *GetDisplayImage() const{ return pDisplayImage; }
	
	/** Display icons (deImage*). */
	inline const decObjectOrderedSet &GetDisplayIcons() const{ return pDisplayIcons; }
	
	/** Set display image and icons. */
	void SetDisplayImages( const char *name );
	
	/** Set display image and icons. */
	void SetDisplayImages( const deovrDeviceComponent &component );
	
	/** Display text. */
	inline const decString &GetDisplayText() const{ return pDisplayText; }
	
	/** Set display text. */
	void SetDisplayText( const char *text );
	
	
	
	/** Update engine input device information component. */
	void GetInfo( deInputDeviceComponent &info ) const;
	/*@}*/
};

#endif
