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

#ifndef _DECLASSSOUNDLEVELMETERSPEAKER_H_
#define _DECLASSSOUNDLEVELMETERSPEAKER_H_

#include <dragengine/resources/sensor/deSoundLevelMeter.h>

#include <libdscript/libdscript.h>

class deEngine;
class deScriptingDragonScript;


/**
 * \brief Sound level meter speaker script class.
 */
class deClassSoundLevelMeterSpeaker : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates script class. */
	deClassSoundLevelMeterSpeaker( deScriptingDragonScript &ds );
	
	/** \brief Clean up script class. */
	virtual ~deClassSoundLevelMeterSpeaker();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create script class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Push sound level meter or NULL onto the stack. */
	void PushSoundLevelMeterSpeaker( dsRunTime *rt, const deSoundLevelMeter::cAudibleSpeaker &speaker );
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsSoundLevelMeterSpeaker;
		
		dsClass *clsVoid;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsString;
		dsClass *clsObject;
		dsClass *clsBool;
		
		dsClass *clsSpeaker;
	};
	
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetSpeaker );
	DEF_NATFUNC( nfGetVolume );
#undef DEF_NATFUNC
};

#endif
