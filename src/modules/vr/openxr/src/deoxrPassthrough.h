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
	void pEnsureCreated();
};

#endif
