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
#ifndef _AESUBANIMATOR_H_
#define _AESUBANIMATOR_H_

// includes
#include <dragengine/common/math/decMath.h>

#include <dragengine/common/string/decString.h>

// predefinitions
class aeLoadSaveSystem;

class deEngine;
class deAnimator;
class deTouchSensor;
class deComponent;
class deAnimation;
class deAnimatorInstance;
class deCollider;
class decCurveBezier;



/**
 * Sub Animator.
 *
 * A sub animator to be used inside the actual animator to edit. Sub
 * animators are loaded from file or can be created manually.
 */
class aeSubAnimator{
private:
	deEngine *pEngine;
	
	deAnimator *pEngAnimator;
	deAnimatorInstance *pEngAnimatorInstance;
	
	decString pPathAnimator;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new sub animator. */
	aeSubAnimator( deEngine *engine );
	/** Cleans up the sub animator. */
	~aeSubAnimator();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the engine animator or NULL if not existing. */
	inline deAnimator *GetEngineAnimator() const{ return pEngAnimator; }
	
	/** Retrieves the path to the animator file. */
	inline const decString &GetPathAnimator() const{ return pPathAnimator; }
	/** Sets the path to the animator file. */
	void SetPathAnimator( const char *path );
	/** Loads the engine animator using the stored path. */
	void LoadAnimator( aeLoadSaveSystem &lssys );
	
	/** Clears the animator. */
	void ClearAnimator();
	/** Adds a controler. */
	void AddController( const char *name, float minimum, float maximum, bool clamp );
	/** Sets a controller value. */
	void SetControllerValue( int controller, float value );
	/** Adds a link. */
	void AddLink( int controller, const decCurveBezier &curve );
	/** Adds a state snapshot rule. */
	void AddRuleSS();
	/** Adds an inverse kinematic rule. */
	void AddRuleIK( const decVector &localPosition, const decVector &localOrientation, const char *solverBone,
		int linkSourceFactor );
	/** Adds a bone to a rule. */
	void AddBoneToRule( int rule, const char *bone );
	/** Enables or disabled a rule. */
	void EnableRule( int rule, bool enable );
	
	/** Set component and animation. */
	void SetComponent( deComponent *component );
	
	/** Set component and animation. */
	void SetComponentAndAnimation( deComponent *component, deAnimation *animation );
	
	/** Apply controllers from another animator instance. */
	void CopyControllers( deAnimatorInstance &instance );
	/** Apply. */
	void Apply();
	/*@}*/
	
private:
	void pCleanUp();
};

// end of include only once
#endif
