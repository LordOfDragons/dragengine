/* 
 * Drag[en]gine DragonScript Script Module
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

#ifndef _DEDSCLASSPARTICLEEMITTERINSTANCELISTENER_H_
#define _DEDSCLASSPARTICLEEMITTERINSTANCELISTENER_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;
class deEngine;



/**
 * @brief Particle Emitter Instance Listener Script Class.
 */
class deClassParticleEmitterInstanceListener : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new class. */
	deClassParticleEmitterInstanceListener( deScriptingDragonScript *ds );
	/** Cleans up the class. */
	virtual ~deClassParticleEmitterInstanceListener();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	/** Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Function index for lastParticleDied(). */
	inline int GetFuncIndexLastParticleDied() const{ return pFuncIndexLastParticleDied; }
	
	/** \brief Function index for collisionResponse(). */
	inline int GetFuncIndexCollisionResponse() const{ return pFuncIndexCollisionResponse; }
	/*@}*/
	
private:
	int pFuncIndexLastParticleDied;
	int pFuncIndexCollisionResponse;
	
	struct sInitData{
		dsClass *clsPEIL, *clsVoid, *clsFlt, *clsPEI, *clsCI, *clsInt;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfLastParticleDied );
	DEF_NATFUNC( nfCollisionResponse );
#undef DEF_NATFUNC
};

#endif
