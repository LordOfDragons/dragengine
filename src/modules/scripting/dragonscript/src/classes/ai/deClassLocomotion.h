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

#ifndef _DEDSCLASSLOCOMOTION_H_
#define _DEDSCLASSLOCOMOTION_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;

class dedsLocomotion;



/**
 * \brief Locomotion script class.
 */
class deClassLocomotion : public dsClass{
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsLocomotionAttribute;
	dsClass *pClsLocomotionTilt;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new class. */
	deClassLocomotion( deScriptingDragonScript &ds );
	
	/** \brief Clean up the class. */
	virtual ~deClassLocomotion();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS(){ return pDS; }
	inline const deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Locomotion from script object or NULL if myself is NULL. */
	dedsLocomotion *GetLocomotion( dsRealObject *myself ) const;
	
	/** \brief Push locomotion which can be NULL. */
	void PushLocomotion( dsRunTime *rt, dedsLocomotion *locomotion );
	
	inline dsClass *GetClassLocomotionAttribute() const{ return pClsLocomotionAttribute; }
	inline dsClass *GetClassLocomotionTilt() const{ return pClsLocomotionTilt; }
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsLoco, *clsVoid, *clsBool, *clsInteger, *clsFloat, *clsString, *clsObject;
		dsClass *clsVector, *clsQuaternion, *clsAInst, *clsCollider, *clsCCT;
		dsClass *clsFileReader;
		dsClass *clsFileWriter;
		dsClass *clsAnimatorCtrl;
		dsClass *clsLocomotionAttribute;
		dsClass *clsLocomotionTilt;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetCanTurn );
	DEF_NATFUNC( nfSetCanTurn );
	
	DEF_NATFUNC( nfGetLimitLookUp );
	DEF_NATFUNC( nfSetLimitLookUp );
	DEF_NATFUNC( nfGetLimitLookDown );
	DEF_NATFUNC( nfSetLimitLookDown );
	DEF_NATFUNC( nfGetAdjustTimeVertical );
	DEF_NATFUNC( nfSetAdjustTimeVertical );
	DEF_NATFUNC( nfGetAdjustRangeVertical );
	DEF_NATFUNC( nfSetAdjustRangeVertical );
	DEF_NATFUNC( nfGetLookVerticalGoal );
	DEF_NATFUNC( nfSetLookVerticalGoal );
	DEF_NATFUNC( nfGetLookVertical );
	DEF_NATFUNC( nfSetLookVertical );
	
	DEF_NATFUNC( nfGetLimitLookLeft );
	DEF_NATFUNC( nfSetLimitLookLeft );
	DEF_NATFUNC( nfGetLimitLookRight );
	DEF_NATFUNC( nfSetLimitLookRight );
	DEF_NATFUNC( nfGetAdjustTimeHorizontal );
	DEF_NATFUNC( nfSetAdjustTimeHorizontal );
	DEF_NATFUNC( nfGetAdjustRangeHorizontal );
	DEF_NATFUNC( nfSetAdjustRangeHorizontal );
	DEF_NATFUNC( nfGetLookHorizontalGoal );
	DEF_NATFUNC( nfSetLookHorizontalGoal );
	DEF_NATFUNC( nfGetLookHorizontal );
	DEF_NATFUNC( nfSetLookHorizontal );
	DEF_NATFUNC( nfGetTurnAdjustLookHorizontal );
	DEF_NATFUNC( nfSetTurnAdjustLookHorizontal );
	
	DEF_NATFUNC( nfGetAdjustTimeAnalogMovingHorizontal );
	DEF_NATFUNC( nfSetAdjustTimeAnalogMovingHorizontal );
	DEF_NATFUNC( nfGetAdjustRangeAnalogMovingHorizontal );
	DEF_NATFUNC( nfSetAdjustRangeAnalogMovingHorizontal );
	DEF_NATFUNC( nfGetAnalogMovingHorizontalGoal );
	DEF_NATFUNC( nfSetAnalogMovingHorizontalGoal );
	DEF_NATFUNC( nfGetAnalogMovingHorizontal );
	DEF_NATFUNC( nfSetAnalogMovingHorizontal );
	DEF_NATFUNC( nfGetAnalogMovingVertical );
	DEF_NATFUNC( nfSetAnalogMovingVertical );
	DEF_NATFUNC( nfGetTurnHorizontal );
	DEF_NATFUNC( nfSetTurnHorizontal );
	
	DEF_NATFUNC( nfGetAnalogMovingSpeed );
	DEF_NATFUNC( nfSetAnalogMovingSpeed );
	
	DEF_NATFUNC( nfGetAdjustTimeOrientation );
	DEF_NATFUNC( nfSetAdjustTimeOrientation );
	DEF_NATFUNC( nfGetOrientation );
	DEF_NATFUNC( nfSetOrientation );
	DEF_NATFUNC( nfGetOrientationQuaternion );
	DEF_NATFUNC( nfSetOrientationQuaternion );
	DEF_NATFUNC( nfGetTurningSpeed );
	DEF_NATFUNC( nfSetTurningSpeed );
	DEF_NATFUNC( nfGetAngularVelocity );
	DEF_NATFUNC( nfSetAngularVelocity );
	
	DEF_NATFUNC( nfGetIsMoving );
	DEF_NATFUNC( nfSetIsMoving );
	DEF_NATFUNC( nfGetMovingSpeed );
	DEF_NATFUNC( nfGetAbsMovingSpeed );
	DEF_NATFUNC( nfSetMovingSpeed );
	DEF_NATFUNC( nfGetMovingOrientation );
	DEF_NATFUNC( nfSetMovingOrientation );
	DEF_NATFUNC( nfGetMovingDirection );
	DEF_NATFUNC( nfGetMovingDirectionVector );
	DEF_NATFUNC( nfSetMovingDirection );
	
	DEF_NATFUNC( nfGetLinearVelocity );
	DEF_NATFUNC( nfSetLinearVelocity );
	DEF_NATFUNC( nfGetAdjustTimeLinearVelocity );
	DEF_NATFUNC( nfSetAdjustTimeLinearVelocity );
	DEF_NATFUNC( nfGetAdjustRangeLinearVelocity );
	DEF_NATFUNC( nfSetAdjustRangeLinearVelocity );
	
	DEF_NATFUNC( nfGetAdjustTimeStance );
	DEF_NATFUNC( nfSetAdjustTimeStance );
	DEF_NATFUNC( nfGetAdjustRangeStance );
	DEF_NATFUNC( nfSetAdjustRangeStance );
	DEF_NATFUNC( nfGetStance );
	DEF_NATFUNC( nfSetStance );
	DEF_NATFUNC( nfGetStanceGoal );
	DEF_NATFUNC( nfSetStanceGoal );
	
	DEF_NATFUNC( nfGetIsTurningIP );
	DEF_NATFUNC( nfSetIsTurningIP );
	DEF_NATFUNC( nfGetAdjustTimeTurnIP );
	DEF_NATFUNC( nfSetAdjustTimeTurnIP );
	DEF_NATFUNC( nfGetTurnIP );
	DEF_NATFUNC( nfSetTurnIP );
	DEF_NATFUNC( nfGetResetTimeTurnIP );
	DEF_NATFUNC( nfSetResetTimeTurnIP );
	DEF_NATFUNC( nfGetReverseTimeTurnIP );
	DEF_NATFUNC( nfSetReverseTimeTurnIP );
	
	DEF_NATFUNC( nfGetCanTurnInPlace );
	DEF_NATFUNC( nfSetCanTurnInPlace );
	DEF_NATFUNC( nfGetLimitTurnInPlaceLeft );
	DEF_NATFUNC( nfSetLimitTurnInPlaceLeft );
	DEF_NATFUNC( nfGetLimitTurnInPlaceRight );
	DEF_NATFUNC( nfSetLimitTurnInPlaceRight );
	
	DEF_NATFUNC( nfGetResetTimeWalk );
	DEF_NATFUNC( nfSetResetTimeWalk );
	
	DEF_NATFUNC( nfGetCanTilt );
	DEF_NATFUNC( nfSetCanTilt );
	DEF_NATFUNC( nfGetTiltMode );
	DEF_NATFUNC( nfSetTiltMode );
	DEF_NATFUNC( nfSetCCTTiltSingle );
	DEF_NATFUNC( nfSetCCTTiltFrontLeft );
	DEF_NATFUNC( nfSetCCTTiltFrontRight );
	DEF_NATFUNC( nfSetCCTTiltBackLeft );
	DEF_NATFUNC( nfSetCCTTiltBackRight );
	
	DEF_NATFUNC( nfGetLimitTiltUp );
	DEF_NATFUNC( nfSetLimitTiltUp );
	DEF_NATFUNC( nfGetLimitTiltDown );
	DEF_NATFUNC( nfSetLimitTiltDown );
	DEF_NATFUNC( nfGetTiltVertical );
	DEF_NATFUNC( nfSetTiltVertical );
	DEF_NATFUNC( nfGetTiltVerticalGoal );
	DEF_NATFUNC( nfSetTiltVerticalGoal );
	DEF_NATFUNC( nfGetAdjustTimeTiltVertical );
	DEF_NATFUNC( nfSetAdjustTimeTiltVertical );
	DEF_NATFUNC( nfGetAdjustRangeTiltVertical );
	DEF_NATFUNC( nfSetAdjustRangeTiltVertical );
	
	DEF_NATFUNC( nfGetLimitTiltLeft );
	DEF_NATFUNC( nfSetLimitTiltLeft );
	DEF_NATFUNC( nfGetLimitTiltRight );
	DEF_NATFUNC( nfSetLimitTiltRight );
	DEF_NATFUNC( nfGetTiltHorizontal );
	DEF_NATFUNC( nfSetTiltHorizontal );
	DEF_NATFUNC( nfGetTiltHorizontalGoal );
	DEF_NATFUNC( nfSetTiltHorizontalGoal );
	DEF_NATFUNC( nfGetAdjustTimeTiltHorizontal );
	DEF_NATFUNC( nfSetAdjustTimeTiltHorizontal );
	DEF_NATFUNC( nfGetAdjustRangeTiltHorizontal );
	DEF_NATFUNC( nfSetAdjustRangeTiltHorizontal );
	
	DEF_NATFUNC( nfGetTiltOffset );
	DEF_NATFUNC( nfSetTiltOffset );
	
	DEF_NATFUNC( nfGetAnimatorInstance );
	DEF_NATFUNC( nfSetAnimatorInstance );
	DEF_NATFUNC( nfAddControllerMapping );
	DEF_NATFUNC( nfRemoveControllerMapping );
	DEF_NATFUNC( nfRemoveAllControllerMappings );
	
	DEF_NATFUNC( nfGetAICollider );
	DEF_NATFUNC( nfSetAICollider );
	DEF_NATFUNC( nfGetUpdateAIColliderAngularVelocity );
	DEF_NATFUNC( nfSetUpdateAIColliderAngularVelocity );
	
	DEF_NATFUNC( nfCancelMovement );
	DEF_NATFUNC( nfCancelInput );
	DEF_NATFUNC( nfCancelMotion );
	DEF_NATFUNC( nfCancelTurnInPlace );
	DEF_NATFUNC( nfForceBodyAdjustment );
	
	DEF_NATFUNC( nfUpdateLooking );
	DEF_NATFUNC( nfApplyStates );
	DEF_NATFUNC( nfUpdateLocomotion );
	DEF_NATFUNC( nfUpdateIsMoving );
	DEF_NATFUNC( nfUpdateOrientation );
	DEF_NATFUNC( nfUpdateLinearVelocity );
	DEF_NATFUNC( nfUpdateStance );
	DEF_NATFUNC( nfUpdatePostLocomotion );
	DEF_NATFUNC( nfUpdateTilt );
	DEF_NATFUNC( nfUpdateAnimatorInstance );
	DEF_NATFUNC( nfUpdateAICollider );
	DEF_NATFUNC( nfAdjustOrientation );
	
	DEF_NATFUNC( nfReadFromFile );
	DEF_NATFUNC( nfWriteToFile );
	
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfEquals );
#undef DEF_NATFUNC
};

#endif
