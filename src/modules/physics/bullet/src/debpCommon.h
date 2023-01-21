/* 
 * Drag[en]gine Bullet Physics Module
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
