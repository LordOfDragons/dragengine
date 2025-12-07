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

// include only once
#ifndef _DEBPSKIN_H_
#define _DEBPSKIN_H_

// includes
#include "dragengine/systems/modules/physics/deBasePhysicsSkin.h"
#include "dragengine/common/math/decMath.h"

// predefintions
class deSkin;



/**
 * @brief Bullet Physics Skin Peer
 * The peer for skin resources in the ODE Physics Module. The main
 * purpose of this class is to provide an octree of faces for quick
 * collision detection if the skin is a simple skin. Complex skins
 * have to be stored inside the Component Peer. Simple skins are
 * much quicker as they do not change over time. A skin is considered
 * simple if it has no weights that could influence the vertices.
 */
class debpSkin : public deBasePhysicsSkin{
private:
	deSkin *pSkin;
	
public:
	// constructor, destructor
	debpSkin(deSkin *skin);
	~debpSkin();
	// management
	inline deSkin *GetSkin() const{return pSkin;}
	
private:
	void pCleanUp();
};

// end of include only once
#endif
