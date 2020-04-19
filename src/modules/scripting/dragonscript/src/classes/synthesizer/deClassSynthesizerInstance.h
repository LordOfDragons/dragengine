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

#ifndef _DECLASSSYNTHESIZERINSTANCE_H_
#define _DECLASSSYNTHESIZERINSTANCE_H_

#include <libdscript/libdscript.h>

class deEngine;
class deSynthesizerInstance;
class deScriptingDragonScript;



/**
 * \brief Synthesizer instance script class.
 */
class deClassSynthesizerInstance : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassSynthesizerInstance( deScriptingDragonScript &ds );
	
	/** \brief Clean up script class. */
	virtual ~deClassSynthesizerInstance();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Scripting module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Synthesizer instance for object or \em NULL if object is \em NULL. */
	deSynthesizerInstance *GetSynthesizerInstance( dsRealObject *object ) const;
	
	/** \brief Push animator instance or NULL on the stack. */
	void PushSynthesizerInstance( dsRunTime *rt, deSynthesizerInstance *instance );
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsSynI;
		dsClass *clsSyn;
		dsClass *clsSynController;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsString;
		dsClass *clsInt;
		dsClass *clsFloat;
		dsClass *clsObject;
	};
	#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetSynthesizer );
	DEF_NATFUNC( nfSetSynthesizer );
	DEF_NATFUNC( nfGetSampleCount );
	DEF_NATFUNC( nfSetSampleCount );
	
	DEF_NATFUNC( nfGetControllerCount );
	DEF_NATFUNC( nfSetControllerCount );
	DEF_NATFUNC( nfGetControllerAt );
	DEF_NATFUNC( nfGetControllerNamed );
	DEF_NATFUNC( nfIndexOfControllerNamed );
	DEF_NATFUNC( nfCopyControllerStates );
	DEF_NATFUNC( nfCopyControllerStates2 );
	DEF_NATFUNC( nfCopyNamedControllerStates );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
