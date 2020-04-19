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

#ifndef _DECLASSSOUND_H_
#define _DECLASSSOUND_H_

#include <libdscript/libdscript.h>

class deSound;
class deScriptingDragonScript;



/**
 * \brief Sound script class.
 */
class deClassSound : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassSound( deScriptingDragonScript &ds );
	
	/** \brief Clean up script class. */
	virtual ~deClassSound();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Get sound from object or \em NULL if \em myself is \em NULL. */
	deSound *GetSound( dsRealObject *myself ) const;
	
	/** \brief Push sound to stack or \em NULL if \em sound is \em NULL. */
	void PushSound( dsRunTime *rt, deSound *sound );
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsSound;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsString;
		dsClass *clsObject;
		dsClass *clsInteger;
		dsClass *clsFloat;
		
		dsClass *clsResNot;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfLoadAsynchron );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetFilename );
	DEF_NATFUNC( nfGetBytesPerSample );
	DEF_NATFUNC( nfGetSampleRate );
	DEF_NATFUNC( nfGetChannelCount );
	DEF_NATFUNC( nfGetSampleCount );
	DEF_NATFUNC( nfGetPlayTime );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
