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

#ifndef _DEBPCOMMON_H_
#define _DEBPCOMMON_H_



#ifdef WITH_DEBUG
	#define BP_DEBUG_LOG(bullet,message) bullet.GetLogger().LogInfoFormat("%s:%d: %s", __FILE__, __LINE__, message);
	#define BP_DEBUG_LOG_F(bullet,fmt,...) bullet.GetLogger().LogInfoFormat("%s:%d: " fmt, __FILE__, __LINE__, __VA_ARGS__);
	#define BP_DEBUG_IF(cmd) cmd;
	
#else
	#define BP_DEBUG_LOG(bullet,message)
	#define BP_DEBUG_LOG_F(bullet,fmt,...)
	#define BP_DEBUG_IF(cmd)
#endif



#include "LinearMath/btScalar.h"


// HACK
// this is a temporary solution since bullet uses with btScalar double
// all over the place but inside meshes this is overkill since meshes
// have vertices grouped around their position in the world. Only the
// world position is requried to be double. Doing so also for the mesh
// vertices wastes precious memory. For the time being the btScalar is
// used which works but wastes memory.


// Vertex struct for triangle meshes
struct debpVertex{
	btScalar x;
	btScalar y;
	btScalar z;
};

// Face struct for triangle meshes
struct debpTriangle{
	int p1;
	int p2;
	int p3;
};

#endif
