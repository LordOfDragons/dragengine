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

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputDeviceAxis.h>
#include <dragengine/resources/image/deImage.h>

class deovrDevice;
class deVROpenVR;



/**
 * VR input device axis.
 */
class deovrDeviceAxis : public deObject{
private:
	deVROpenVR &pOvr;
	
	int pIndex;
	decString pID;
	decString pName;
	deInputDeviceAxis::eAxisTypes pType;
	
	deImage::Ref pDisplayImage;
	decObjectOrderedSet pDisplayIcons;
	decString pDisplayText;
	
	int pMinimum;
	int pMaximum;
	
	float pValue;
	timeval pLastEventTime;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device axis. */
	deovrDeviceAxis( deVROpenVR &ovr );
	
protected:
	/** Clean up device axis. */
	virtual ~deovrDeviceAxis();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** OpenVR ovr. */
	inline deVROpenVR &GetOvr() const{ return pOvr; }
	
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
	inline deInputDeviceAxis::eAxisTypes GetType() const{ return pType; }
	
	/** Set type. */
	void SetType( deInputDeviceAxis::eAxisTypes type );
	
	
	
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
	
	
	
	/** Minimum value. */
	inline int GetMinimum() const{ return pMinimum; }
	
	/** Set minimum value. */
	void SetMinimum( int minimum );
	
	/** Maximum value. */
	inline int GetMaximum() const{ return pMaximum; }
	
	/** Set maximum value. */
	void SetMaximum( int maximum );
	
	/** Value. */
	inline float GetValue() const{ return pValue; }
	
	/** Set value. */
	void SetValue( float value );
	
	
	
	/** Update engine input device information axis. */
	void GetInfo( deInputDeviceAxis &info ) const;
	/*@}*/
	
	
	
private:
	void pUpdateDeadZone();
};

#endif
