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

#ifndef _DECLASSNETWORKMESSAGE_H_
#define _DECLASSNETWORKMESSAGE_H_

#include <libdscript/libdscript.h>

class deNetworkMessage;
class deScriptingDragonScript;



/**
 * \brief Network message script class.
 */
class deClassNetworkMessage : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassNetworkMessage( deScriptingDragonScript &ds );
	
	/** \brief Clean up script class. */
	virtual ~deClassNetworkMessage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module owning the script class. */
	inline deScriptingDragonScript &GetDS(){ return pDS; }
	inline const deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create script class members. */
	virtual void CreateClassMembers( dsEngine *engine );
	
	
	/**
	 * \brief Network message from script object.
	 * \details Returns NULL if \em myself is NULL.
	 */
	deNetworkMessage *GetNetworkMessage( dsRealObject *myself ) const;
	
	/**
	 * \brief Push network message onto stack.
	 * \details Pushes a \em null object to the stack if \em collider is NULL.
	 */
	void PushNetworkMessage( dsRunTime *rt, deNetworkMessage *message );
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsNetworkMessage;
		dsClass *clsVoid;
		dsClass *clsInteger;
		dsClass *clsBool;
		dsClass *clsString;
		dsClass *clsObject;
		dsClass *clsFileReader;
		dsClass *clsFileWrite;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetDataLength );
	DEF_NATFUNC( nfClear );
	
	DEF_NATFUNC( nfGetReader );
	DEF_NATFUNC( nfGetWriter );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
