/* 
 * Drag[en]gine IGDE Animator Editor
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
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new sub animator. */
	aeSubAnimator( deEngine *engine );
	/** Cleans up the sub animator. */
	~aeSubAnimator();
	/*@}*/
	
	/** @name Management */
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
