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

#ifndef _DECLASSSOUNDLEVELMETERLISTENER_H_
#define _DECLASSSOUNDLEVELMETERLISTENER_H_

#include <libdscript/libdscript.h>


class deEngine;
class deScriptingDragonScript;


/**
 * \brief Sound level meter listener script class.
 */
class deClassSoundLevelMeterListener : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	int pFuncIndexSpeakerAudible;
	int pFuncIndexSpeakerInaudible;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassSoundLevelMeterListener( deScriptingDragonScript &ds );
	
	/** \brief Clean up script class. */
	virtual ~deClassSoundLevelMeterListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create script class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Function index for speakerAudible(). */
	inline int GetFuncIndexSpeakerAudible() const{ return pFuncIndexSpeakerAudible; }
	
	/** \brief Function index for speakerInaudible(). */
	inline int GetFuncIndexSpeakerInaudible() const{ return pFuncIndexSpeakerInaudible; }
	
	
	
private:
	struct sInitData{
		dsClass *clsSoundLevelMeterListener;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsInteger;
		dsClass *clsSoundLevelMeterSpeaker;
		dsClass *clsSpeaker;
	};
	
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfSpeakerAudible );
	DEF_NATFUNC( nfSpeakerInaudible );
#undef DEF_NATFUNC
};

#endif
