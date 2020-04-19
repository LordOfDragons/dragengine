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

#ifndef _DEDSCLASSPARTICLEEMITTER_H_
#define _DEDSCLASSPARTICLEEMITTER_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;

class deParticleEmitter;



/**
 * @brief Particle Emitter Script Class.
 */
class deClassParticleEmitter : public dsClass{
private:
	deScriptingDragonScript *pDS;
	dsClass *pClsParticleEmitterParameter;
	dsClass *pClsParticleEmitterEmitController;
	dsClass *pClsParticleEmitterCastFrom;
	dsClass *pClsParticleCollisionResponse;
	dsClass *pClsParticleEmitterSimulation;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new class. */
	deClassParticleEmitter( deScriptingDragonScript *ds );
	/** Cleans up the class. */
	virtual ~deClassParticleEmitter();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	/** Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** Retrieves the prop field or NULL if myself is NULL. */
	deParticleEmitter *GetParticleEmitter( dsRealObject *myself ) const;
	/** Pushes the prop field which can be NULL. */
	void PushParticleEmitter( dsRunTime *rt, deParticleEmitter *emitter );
	
	inline dsClass *GetClassParticleEmitterParameter() const{ return pClsParticleEmitterParameter; }
	inline dsClass *GetClassParticleEmitterEmitController() const{ return pClsParticleEmitterEmitController; }
	inline dsClass *GetClassParticleEmitterCastFrom() const{ return pClsParticleEmitterCastFrom; }
	inline dsClass *GetClassParticleCollisionResponse() const{ return pClsParticleCollisionResponse; }
	inline dsClass *GetClassParticleEmitterSimulation() const{ return pClsParticleEmitterSimulation; }
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsPE, *clsVoid, *clsBool, *clsInt, *clsFloat, *clsStr, *clsObj;
		dsClass *clsCurveBezier, *clsModel, *clsSkin;
		dsClass *clsPEController;
		dsClass *clsParticleEmitterParameter;
		dsClass *clsParticleEmitterEmitController;
		dsClass *clsParticleEmitterCastFrom;
		dsClass *clsParticleCollisionResponse;
		dsClass *clsParticleEmitterSimulation;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetControllerCount );
	DEF_NATFUNC( nfSetControllerCount );
	DEF_NATFUNC( nfGetControllerAt );
	DEF_NATFUNC( nfGetControllerNamed );
	DEF_NATFUNC( nfIndexOfControllerNamed );
	
	DEF_NATFUNC( nfGetBurstLifetime );
	DEF_NATFUNC( nfSetBurstLifetime );
	DEF_NATFUNC( nfGetEmitBurst );
	DEF_NATFUNC( nfSetEmitBurst );
	
	DEF_NATFUNC( nfGetTypeCount );
	DEF_NATFUNC( nfSetTypeCount );
	DEF_NATFUNC( nfGetTypeSkin );
	DEF_NATFUNC( nfSetTypeSkin );
	DEF_NATFUNC( nfGetTypeModel );
	DEF_NATFUNC( nfSetTypeModel );
	DEF_NATFUNC( nfGetTypeModelSkin );
	DEF_NATFUNC( nfSetTypeModelSkin );
	DEF_NATFUNC( nfGetTypeCastFrom );
	DEF_NATFUNC( nfSetTypeCastFrom );
	DEF_NATFUNC( nfGetTypeSimulationType );
	DEF_NATFUNC( nfSetTypeSimulationType );
	DEF_NATFUNC( nfGetTypeIntervalAsDistance );
	DEF_NATFUNC( nfSetTypeIntervalAsDistance );
	DEF_NATFUNC( nfGetTypeTrailEmitter );
	DEF_NATFUNC( nfSetTypeTrailEmitter );
	DEF_NATFUNC( nfGetTypeTrailController );
	DEF_NATFUNC( nfSetTypeTrailController );
	DEF_NATFUNC( nfGetTypePhysicsSize );
	DEF_NATFUNC( nfSetTypePhysicsSize );
	DEF_NATFUNC( nfGetTypeCollisionResponse );
	DEF_NATFUNC( nfSetTypeCollisionResponse );
	DEF_NATFUNC( nfGetTypeCollisionEmitter );
	DEF_NATFUNC( nfSetTypeCollisionEmitter );
	DEF_NATFUNC( nfGetTypeEmitMinImpulse );
	DEF_NATFUNC( nfSetTypeEmitMinImpulse );
	DEF_NATFUNC( nfGetTypeEmitController );
	DEF_NATFUNC( nfSetTypeEmitController );
	
	DEF_NATFUNC( nfSetTypeParameterValue );
	DEF_NATFUNC( nfSetTypeParameterSpread );
	DEF_NATFUNC( nfSetTypeParameterControllerValue );
	DEF_NATFUNC( nfSetTypeParameterCurveValue );
	DEF_NATFUNC( nfSetTypeParameterControllerSpread );
	DEF_NATFUNC( nfSetTypeParameterCurveSpread );
	DEF_NATFUNC( nfSetTypeParameterCurveProgress );
	DEF_NATFUNC( nfSetTypeParameterCurveBeam );
	
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfEquals );
#undef DEF_NATFUNC
};

#endif
