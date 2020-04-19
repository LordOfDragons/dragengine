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

#ifndef _DECLASSSOUNDLEVELMETER_H_
#define _DECLASSSOUNDLEVELMETER_H_

#include <libdscript/libdscript.h>

class deEngine;
class deScriptingDragonScript;
class deSoundLevelMeter;


/**
 * \brief Sound level meter script class.
 */
class deClassSoundLevelMeter : public dsClass{
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsSoundLevelMeterType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassSoundLevelMeter( deScriptingDragonScript &ds );
	
	/** \brief Clean up script class. */
	virtual ~deClassSoundLevelMeter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create script class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Sound level meter from real object or NULL if real object is NULL. */
	deSoundLevelMeter *GetSoundLevelMeter( dsRealObject *myself ) const;
	
	/** \brief Push sound level meter or NULL onto the stack. */
	void PushSoundLevelMeter( dsRunTime *rt, deSoundLevelMeter *soundLevelMeter );
	
	inline dsClass *GetClassSoundLevelMeterType() const{ return pClsSoundLevelMeterType; }
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsSoundLevelMeter;
		
		dsClass *clsVoid;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsString;
		dsClass *clsObject;
		dsClass *clsBool;
		
		dsClass *clsQuaternion;
		dsClass *clsDVector;
		dsClass *clsLayerMask;
		dsClass *clsSoundLevelMeterSpeaker;
		dsClass *clsSoundLevelMeterListener;
		dsClass *clsSoundLevelMeterType;
	};
	
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetType );
	DEF_NATFUNC( nfSetType );
	DEF_NATFUNC( nfGetPosition );
	DEF_NATFUNC( nfSetPosition );
	DEF_NATFUNC( nfGetOrientation );
	DEF_NATFUNC( nfSetOrientation );
	DEF_NATFUNC( nfGetConeAngle );
	DEF_NATFUNC( nfSetConeAngle );
	DEF_NATFUNC( nfGetAudibleDistance );
	DEF_NATFUNC( nfSetAudibleDistance );
	DEF_NATFUNC( nfGetLayerMask );
	DEF_NATFUNC( nfSetLayerMask );
	DEF_NATFUNC( nfGetEnabled );
	DEF_NATFUNC( nfSetEnabled );
	
	DEF_NATFUNC( nfGetAudibleSpeakerCount );
	DEF_NATFUNC( nfGetAudibleSpeakerAt );
	
	DEF_NATFUNC( nfGetListener );
	DEF_NATFUNC( nfSetListener );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
