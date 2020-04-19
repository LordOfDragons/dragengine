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

#ifndef _DEDSCLASSPARTICLEEMITTERCONTROLLER_H_
#define _DEDSCLASSPARTICLEEMITTERCONTROLLER_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;

class deParticleEmitter;
class deParticleEmitterInstance;



/**
 * \brief ParticleEmitter controller script class.
 */
class deClassParticleEmitterController : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassParticleEmitterController( deScriptingDragonScript &ds );
	
	/** \brief Clean up class. */
	virtual ~deClassParticleEmitterController();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Get parameters from object. */
	void GetController( dsRealObject *myself, deParticleEmitter *&emitter,
		deParticleEmitterInstance *&instance, int &index ) const;
	
	/** \brief Push controller. */
	void PushController( dsRunTime *rt, deParticleEmitter *emitter, int index );
	
	/** \brief Push controller. */
	void PushController( dsRunTime *rt, deParticleEmitterInstance *instance, int index );
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsParticleEmitterCtrl;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsInt;
		dsClass *clsFloat;
		dsClass *clsStr;
		dsClass *clsObj;
		
		dsClass *clsParticleEmitter;
		dsClass *clsParticleEmitterInst;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetParticleEmitter );
	DEF_NATFUNC( nfGetParticleEmitterInstance );
	DEF_NATFUNC( nfGetControllerIndex );
	
	DEF_NATFUNC( nfGetName );
	DEF_NATFUNC( nfSetName );
	DEF_NATFUNC( nfGetLower );
	DEF_NATFUNC( nfGetUpper );
	DEF_NATFUNC( nfSetRange );
	DEF_NATFUNC( nfGetValue );
	DEF_NATFUNC( nfSetValue );
	DEF_NATFUNC( nfGetRelativeValue );
	DEF_NATFUNC( nfSetRelativeValue );
	DEF_NATFUNC( nfGetFrozen );
	DEF_NATFUNC( nfSetFrozen );
	DEF_NATFUNC( nfGetClamp );
	DEF_NATFUNC( nfSetClamp );
	
	DEF_NATFUNC( nfIncrement );
	DEF_NATFUNC( nfSetToLower );
	DEF_NATFUNC( nfSetToUpper );
	DEF_NATFUNC( nfReverse );
	DEF_NATFUNC( nfSetValueFrom );
	DEF_NATFUNC( nfSetAllFrom );
	DEF_NATFUNC( nfIsAtLower );
	DEF_NATFUNC( nfIsAtUpper );
	
	DEF_NATFUNC( nfEquals );
#undef DEF_NATFUNC
};

#endif
