/* 
 * Drag[en]gine OpenXR VR Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOXRPASTHROUGH_H_
#define _DEOXRPASTHROUGH_H_

#include "deoxrBasics.h"

#include <dragengine/deObject.h>

class deoxrSession;


/**
 * VR Passthrough.
 */
class deoxrPassthrough : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoxrPassthrough> Ref;
	
	
	
private:
	deoxrSession &pSession;
	
	XrPassthroughFB pPassthrough;
	XrPassthroughLayerFB pLayer;
	
	bool pEnabled;
	float pTransparency;
	
	XrPassthroughStyleFB pLayerStyle;
	XrCompositionLayerPassthroughFB pCompositeLayer;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create passthrough. */
	deoxrPassthrough( deoxrSession &session );
	
protected:
	/** Clean up passthrough. */
	virtual ~deoxrPassthrough();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** Session. */
	inline deoxrSession &GetSession() const{ return pSession; }
	
	/** Passthrough. */
	inline XrPassthroughFB GetPassthrough() const{ return pPassthrough; }
	
	/** Passthrough layer. */
	inline XrPassthroughLayerFB GetLayer() const{ return pLayer; }
	
	/** Enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** Set enabled. */
	void SetEnabled( bool enabled );
	
	/** Transparency. */
	inline float GetTransparency() const{ return pTransparency; }
	
	/** Set transparency. */
	void SetTransparency( float transparency );
	
	/** Composite layer. */
	inline const XrCompositionLayerPassthroughFB &GetCompositeLayer() const{ return pCompositeLayer; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pUpdateLayerStyle();
};

#endif
