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
