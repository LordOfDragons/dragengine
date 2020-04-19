/* 
 * Drag[en]gine OpenGL Graphic Module
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
#ifndef _DEOGLCOMMANDEXECUTER_H_
#define _DEOGLCOMMANDEXECUTER_H_

// includes

// definitions

// predefintions
class deGraphicOpenGl;
class decUnicodeArgumentList;
class decUnicodeString;



// opengl command executer
class deoglCommandExecuter{
private:
	deGraphicOpenGl &pOgl;
public:
	// constructor, destructor
	deoglCommandExecuter( deGraphicOpenGl &ogl );
	~deoglCommandExecuter();
	// management
	void ExecuteCommand( const decUnicodeArgumentList &command, decUnicodeString &answer );
private:
	void pHelp( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pExtensions( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pRenderWindow( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pVisual( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pFBOInfos( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pQuickDebug( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pFixNaN( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	void pAnswerBoolValue( decUnicodeString &answer, const char *name, bool value );
	void pAnswerIntValue( decUnicodeString &answer, const char *name, int value );
	void pAnswerStringValue( decUnicodeString &answer, const char *name, const char *value );
	void pAnswerPointerValue( decUnicodeString &answer, const char *name, void *value );
};

// end of include only once
#endif
