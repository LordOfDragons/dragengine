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

#ifndef _DEDSCLASSPARTICLEEMITTERINSTANCE_H_
#define _DEDSCLASSPARTICLEEMITTERINSTANCE_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;

class deParticleEmitterInstance;



/**
 * @brief Particle Emitter Instance Script Class.
 */
class deClassParticleEmitterInstance : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new class. */
	deClassParticleEmitterInstance(deScriptingDragonScript *ds);
	/** Cleans up the class. */
	virtual ~deClassParticleEmitterInstance();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline deScriptingDragonScript *GetDS() const{return pDS;}
	/** Creates class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** Retrieves the instance or NULL if myself is NULL. */
	deParticleEmitterInstance *GetInstance(dsRealObject *myself) const;
	/** Pushes the instance which can be NULL. */
	void PushInstance(dsRunTime *rt, deParticleEmitterInstance *instance);
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsPEI, *clsVoid, *clsBool, *clsInt, *clsFloat, *clsStr, *clsObj;
		dsClass *clsDVec, *clsVec, *clsComp, *clsMdl, *clsSkin, *clsCF, *clsPEIL;
		dsClass *clsPE, *clsCurveBezier, *clsQuat, *clsDynSkin;
		dsClass *clsPEController;
		dsClass *clsLayerMask;
		dsClass *clsCollider;
		dsClass *clsWorld;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetEmitter);
	DEF_NATFUNC(nfSetEmitter);
	DEF_NATFUNC(nfGetPosition);
	DEF_NATFUNC(nfSetPosition);
	DEF_NATFUNC(nfGetOrientation);
	DEF_NATFUNC(nfSetOrientation);
	DEF_NATFUNC(nfGetEnableCasting);
	DEF_NATFUNC(nfSetEnableCasting);
	DEF_NATFUNC(nfGetRemoveAfterLastParticleDied);
	DEF_NATFUNC(nfSetRemoveAfterLastParticleDied);
	DEF_NATFUNC(nfGetTimeScale);
	DEF_NATFUNC(nfSetTimeScale);
	DEF_NATFUNC(nfGetWarmUpTime);
	DEF_NATFUNC(nfSetWarmUpTime);
	DEF_NATFUNC(nfGetBurstTime);
	DEF_NATFUNC(nfSetBurstTime);
	DEF_NATFUNC(nfGetParentWorld);
	
	DEF_NATFUNC(nfGetControllerCount);
	DEF_NATFUNC(nfGetControllerAt);
	DEF_NATFUNC(nfGetControllerNamed);
	DEF_NATFUNC(nfIndexOfControllerNamed);
	DEF_NATFUNC(nfCopyControllerStates);
	DEF_NATFUNC(nfCopyControllerStates2);
	DEF_NATFUNC(nfCopyNamedControllerStates);
	
	DEF_NATFUNC(nfGetLayerMask);
	DEF_NATFUNC(nfSetLayerMask);
	DEF_NATFUNC(nfGetCollisionFilter);
	DEF_NATFUNC(nfSetCollisionFilter);
	
	DEF_NATFUNC(nfGetTypeComponent);
	DEF_NATFUNC(nfSetTypeComponent);
	DEF_NATFUNC(nfGetTypeDynamicSkin);
	DEF_NATFUNC(nfSetTypeDynamicSkin);
	
	DEF_NATFUNC(nfResetBurst);
	DEF_NATFUNC(nfKillAllParticles);
	
	DEF_NATFUNC(nfGetListener);
	DEF_NATFUNC(nfSetListener);
	
	DEF_NATFUNC(nfGetIgnoreColliderCount);
	DEF_NATFUNC(nfGetIgnoreColliderAt);
	DEF_NATFUNC(nfHasIgnoreCollider);
	DEF_NATFUNC(nfAddIgnoreCollider);
	DEF_NATFUNC(nfRemoveIgnoreCollider);
	DEF_NATFUNC(nfRemoveAllIgnoreColliders);
	
	DEF_NATFUNC(nfHashCode);
	DEF_NATFUNC(nfEquals);
#undef DEF_NATFUNC
};

#endif
