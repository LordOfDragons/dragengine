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

#ifndef _DEOVRDEVICEAXIS_H_
#define _DEOVRDEVICEAXIS_H_

#include "deovrDeviceComponent.h"

#include <openvr/openvr.h>

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputDeviceAxis.h>
#include <dragengine/resources/image/deImage.h>

class deovrDevice;


/**
 * VR input device axis.
 */
class deovrDeviceAxis : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deovrDeviceAxis> Ref;
	
	
	
private:
	deovrDevice &pDevice;
	
	int pIndex;
	
	vr::VRActionHandle_t pActionAnalogHandle;
	int pComponent;
	int pFinger;
	
	decString pID;
	decString pName;
	deInputDeviceAxis::eAxisTypes pType;
	deovrDeviceComponent::Ref pInputDeviceComponent;
	
	deImage::Ref pDisplayImage;
	decObjectOrderedSet pDisplayIcons;
	decString pDisplayText;
	
	float pMinimum;
	float pMaximum;
	float pCenter;
	float pDeadZone;
	float pResolution;
	
	float pValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device axis. */
	deovrDeviceAxis( deovrDevice &device );
	
protected:
	/** Clean up device axis. */
	virtual ~deovrDeviceAxis();
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
	
	
	
	/** Analog action handle. */
	inline vr::VRActionHandle_t GetActionAnalogHandle() const{ return pActionAnalogHandle; }
	
	/** Set analog action handle. */
	void SetActionAnalogHandle( vr::VRActionHandle_t handle );
	
	/** Component to use. */
	inline int GetComponent() const{ return pComponent; }
	
	/** Set component to use. */
	void SetComponent( int component );
	
	/** Finger. */
	inline int GetFinger() const{ return pFinger; }
	
	/** Set finger. */
	void SetFinger( int finger );
	
	
	
	/** Identifier. */
	inline const decString &GetID() const{ return pID; }
	
	/** Set identifier. */
	void SetID( const char *id );
	
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Set name. */
	void SetName( const char *name );
	
	/** Type. */
	inline deInputDeviceAxis::eAxisTypes GetType() const{ return pType; }
	
	/** Set type. */
	void SetType( deInputDeviceAxis::eAxisTypes type );
	
	/** Input device component. */
	inline deovrDeviceComponent *GetInputDeviceComponent() const{ return pInputDeviceComponent; }
	
	/** Set input device component. */
	void SetInputDeviceComponent( deovrDeviceComponent *component );
	
	
	
	/** Display image. */
	inline deImage *GetDisplayImage() const{ return pDisplayImage; }
	
	/** Display icons (deImage*). */
	inline const decObjectOrderedSet &GetDisplayIcons() const{ return pDisplayIcons; }
	
	/** Set display image and icons. */
	void SetDisplayImages( const char *name );
	
	/** Display text. */
	inline const decString &GetDisplayText() const{ return pDisplayText; }
	
	/** Set display text. */
	void SetDisplayText( const char *text );
	
	
	
	/** Minimum. */
	inline float GetMinimum() const{ return pMinimum; }
	
	/** Maximum. */
	inline float GetMaximum() const{ return pMaximum; }
	
	/** Set range. */
	void SetRange( float minimum, float maximum );
	
	/** Center value. */
	inline float GetCenter() const{ return pCenter; }
	
	/** Set center value. */
	void SetCenter( float center );
	
	/** Dead zone value. */
	inline float GetDeadZone() const{ return pDeadZone; }
	
	/** Set dead zone value. */
	void SetDeadZone( float deadZone );
	
	/** Resolution. */
	inline float GetResolution() const{ return pResolution; }
	
	/** Set resolution value. */
	void SetResolution( float resolution );
	
	
	
	/** Value. */
	inline float GetValue() const{ return pValue; }
	
	/** Set value. */
	void SetValue( float value );
	
	/** Update value if changed. */
	void UpdateValue( float value );
	
	/** Track state. */
	void TrackState();
	
	/** Reset state. */
	void ResetState();
	
	
	
	/** Update engine input device information axis. */
	void GetInfo( deInputDeviceAxis &info ) const;
	/*@}*/
	
	
	
private:
};

#endif
