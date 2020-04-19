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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "deoglDebugFont.h"
#include "configuration/deoglConfiguration.h"
#include "renderthread/deoglRenderThread.h"
#include "texture/texture2d/deoglTexture.h"
#include "texture/pixelbuffer/deoglPixelBuffer.h"
#include "../font/debugfont3.c"

#include <dragengine/common/exceptions.h>



// Class deoglDebugFont
/////////////////////////

// Constructor, Destructor
////////////////////////////

deoglDebugFont::deoglDebugFont( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ){
	pGlyphs = NULL;
	pTexture = NULL;
	pFontHeight = 1;
	
	try{
		pBuildTexture();
		pBuildGlyphs();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglDebugFont::~deoglDebugFont(){
	pCleanUp();
}



// Private Functions
//////////////////////

void deoglDebugFont::pCleanUp(){
	if( pTexture ){
		delete pTexture;
	}
	
	if( pGlyphs ){
		delete [] pGlyphs;
	}
}

struct sTempGlyph{
	int u, v, width;
};

void deoglDebugFont::pBuildGlyphs(){
	const deoglConfiguration &config = pRenderThread.GetConfiguration();
	float factorU, factorV, offsetU, offsetV;
	int i;
	
	// reg-exp replace .font.xml :
	// search: <glyph code='([0-9]+)' u='([0-9]+)' v='([0-9]+)' width='([0-9]+)' bearing='([0-9]+)' advance='([0-9]+)'/>
	// replace: { \2, \3, \6 }, /* glyph \1 */
#if 0
	sTempGlyph tempGlyphs[ 256 ] = {
		{ 0, 0, 5 }, /* glyph 0 */
		{ 0, 0, 5 }, /* glyph 1 */
		{ 0, 0, 5 }, /* glyph 2 */
		{ 0, 0, 5 }, /* glyph 3 */
		{ 0, 0, 5 }, /* glyph 4 */
		{ 0, 0, 5 }, /* glyph 5 */
		{ 0, 0, 5 }, /* glyph 6 */
		{ 0, 0, 5 }, /* glyph 7 */
		{ 0, 0, 5 }, /* glyph 8 */
		{ 0, 0, 5 }, /* glyph 9 */
		{ 0, 0, 5 }, /* glyph 10 */
		{ 0, 0, 5 }, /* glyph 11 */
		{ 0, 0, 5 }, /* glyph 12 */
		{ 0, 0, 5 }, /* glyph 13 */
		{ 0, 0, 5 }, /* glyph 14 */
		{ 0, 0, 5 }, /* glyph 15 */
		{ 0, 0, 5 }, /* glyph 16 */
		{ 0, 0, 5 }, /* glyph 17 */
		{ 0, 0, 5 }, /* glyph 18 */
		{ 0, 0, 5 }, /* glyph 19 */
		{ 0, 0, 5 }, /* glyph 20 */
		{ 0, 0, 5 }, /* glyph 21 */
		{ 0, 0, 5 }, /* glyph 22 */
		{ 0, 0, 5 }, /* glyph 23 */
		{ 0, 0, 5 }, /* glyph 24 */
		{ 0, 0, 5 }, /* glyph 25 */
		{ 0, 0, 5 }, /* glyph 26 */
		{ 0, 0, 5 }, /* glyph 27 */
		{ 0, 0, 5 }, /* glyph 28 */
		{ 0, 0, 5 }, /* glyph 29 */
		{ 0, 0, 5 }, /* glyph 30 */
		{ 0, 0, 5 }, /* glyph 31 */
		
		{ 1, 1, 10 }, /* glyph 32 */
		{ 12, 1, 10 }, /* glyph 33 */
		{ 23, 1, 10 }, /* glyph 34 */
		{ 34, 1, 10 }, /* glyph 35 */
		{ 45, 1, 10 }, /* glyph 36 */
		{ 56, 1, 10 }, /* glyph 37 */
		{ 67, 1, 10 }, /* glyph 38 */
		{ 78, 1, 10 }, /* glyph 39 */
		{ 89, 1, 10 }, /* glyph 40 */
		{ 100, 1, 10 }, /* glyph 41 */
		{ 111, 1, 10 }, /* glyph 42 */
		{ 122, 1, 10 }, /* glyph 43 */
		{ 133, 1, 10 }, /* glyph 44 */
		{ 144, 1, 10 }, /* glyph 45 */
		{ 155, 1, 10 }, /* glyph 46 */
		{ 166, 1, 10 }, /* glyph 47 */
		{ 177, 1, 10 }, /* glyph 48 */
		{ 188, 1, 10 }, /* glyph 49 */
		{ 199, 1, 10 }, /* glyph 50 */
		{ 210, 1, 10 }, /* glyph 51 */
		{ 221, 1, 10 }, /* glyph 52 */
		{ 232, 1, 10 }, /* glyph 53 */
		{ 243, 1, 10 }, /* glyph 54 */
		{ 1, 17, 10 }, /* glyph 55 */
		{ 12, 17, 10 }, /* glyph 56 */
		{ 23, 17, 10 }, /* glyph 57 */
		{ 34, 17, 10 }, /* glyph 58 */
		{ 45, 17, 10 }, /* glyph 59 */
		{ 56, 17, 10 }, /* glyph 60 */
		{ 67, 17, 10 }, /* glyph 61 */
		{ 78, 17, 10 }, /* glyph 62 */
		{ 89, 17, 10 }, /* glyph 63 */
		{ 100, 17, 10 }, /* glyph 64 */
		{ 111, 17, 10 }, /* glyph 65 */
		{ 122, 17, 10 }, /* glyph 66 */
		{ 133, 17, 10 }, /* glyph 67 */
		{ 144, 17, 10 }, /* glyph 68 */
		{ 155, 17, 10 }, /* glyph 69 */
		{ 166, 17, 10 }, /* glyph 70 */
		{ 177, 17, 10 }, /* glyph 71 */
		{ 188, 17, 10 }, /* glyph 72 */
		{ 199, 17, 10 }, /* glyph 73 */
		{ 210, 17, 10 }, /* glyph 74 */
		{ 221, 17, 10 }, /* glyph 75 */
		{ 232, 17, 10 }, /* glyph 76 */
		{ 243, 17, 10 }, /* glyph 77 */
		{ 1, 33, 10 }, /* glyph 78 */
		{ 12, 33, 10 }, /* glyph 79 */
		{ 23, 33, 10 }, /* glyph 80 */
		{ 34, 33, 10 }, /* glyph 81 */
		{ 45, 33, 10 }, /* glyph 82 */
		{ 56, 33, 10 }, /* glyph 83 */
		{ 67, 33, 10 }, /* glyph 84 */
		{ 78, 33, 10 }, /* glyph 85 */
		{ 89, 33, 10 }, /* glyph 86 */
		{ 100, 33, 10 }, /* glyph 87 */
		{ 111, 33, 10 }, /* glyph 88 */
		{ 122, 33, 10 }, /* glyph 89 */
		{ 133, 33, 10 }, /* glyph 90 */
		{ 144, 33, 10 }, /* glyph 91 */
		{ 155, 33, 10 }, /* glyph 92 */
		{ 166, 33, 10 }, /* glyph 93 */
		{ 177, 33, 10 }, /* glyph 94 */
		{ 188, 33, 10 }, /* glyph 95 */
		{ 199, 33, 10 }, /* glyph 96 */
		{ 210, 33, 10 }, /* glyph 97 */
		{ 221, 33, 10 }, /* glyph 98 */
		{ 232, 33, 10 }, /* glyph 99 */
		{ 243, 33, 10 }, /* glyph 100 */
		{ 1, 49, 10 }, /* glyph 101 */
		{ 12, 49, 10 }, /* glyph 102 */
		{ 23, 49, 10 }, /* glyph 103 */
		{ 34, 49, 10 }, /* glyph 104 */
		{ 45, 49, 10 }, /* glyph 105 */
		{ 56, 49, 10 }, /* glyph 106 */
		{ 67, 49, 10 }, /* glyph 107 */
		{ 78, 49, 10 }, /* glyph 108 */
		{ 89, 49, 10 }, /* glyph 109 */
		{ 100, 49, 10 }, /* glyph 110 */
		{ 111, 49, 10 }, /* glyph 111 */
		{ 122, 49, 10 }, /* glyph 112 */
		{ 133, 49, 10 }, /* glyph 113 */
		{ 144, 49, 10 }, /* glyph 114 */
		{ 155, 49, 10 }, /* glyph 115 */
		{ 166, 49, 10 }, /* glyph 116 */
		{ 177, 49, 10 }, /* glyph 117 */
		{ 188, 49, 10 }, /* glyph 118 */
		{ 199, 49, 10 }, /* glyph 119 */
		{ 210, 49, 10 }, /* glyph 120 */
		{ 221, 49, 10 }, /* glyph 121 */
		{ 232, 49, 10 }, /* glyph 122 */
		{ 243, 49, 10 }, /* glyph 123 */
		{ 1, 65, 10 }, /* glyph 124 */
		{ 12, 65, 10 }, /* glyph 125 */
		{ 23, 65, 10 }, /* glyph 126 */
		{ 34, 65, 10 }, /* glyph 160 */
		{ 45, 65, 10 }, /* glyph 161 */
		{ 56, 65, 10 }, /* glyph 162 */
		{ 67, 65, 10 }, /* glyph 163 */
		{ 78, 65, 10 }, /* glyph 164 */
		{ 89, 65, 10 }, /* glyph 165 */
		{ 100, 65, 10 }, /* glyph 166 */
		{ 111, 65, 10 }, /* glyph 167 */
		{ 122, 65, 10 }, /* glyph 168 */
		{ 133, 65, 10 }, /* glyph 169 */
		{ 144, 65, 10 }, /* glyph 170 */
		{ 155, 65, 10 }, /* glyph 171 */
		{ 166, 65, 10 }, /* glyph 172 */
		{ 177, 65, 10 }, /* glyph 173 */
		{ 188, 65, 10 }, /* glyph 174 */
		{ 199, 65, 10 }, /* glyph 175 */
		{ 210, 65, 10 }, /* glyph 176 */
		{ 221, 65, 10 }, /* glyph 177 */
		{ 232, 65, 10 }, /* glyph 178 */
		{ 243, 65, 10 }, /* glyph 179 */
		{ 1, 81, 10 }, /* glyph 180 */
		{ 12, 81, 10 }, /* glyph 181 */
		{ 23, 81, 10 }, /* glyph 182 */
		{ 34, 81, 10 }, /* glyph 183 */
		{ 45, 81, 10 }, /* glyph 184 */
		{ 56, 81, 10 }, /* glyph 185 */
		{ 67, 81, 10 }, /* glyph 186 */
		{ 78, 81, 10 }, /* glyph 187 */
		{ 89, 81, 10 }, /* glyph 188 */
		{ 100, 81, 10 }, /* glyph 189 */
		{ 111, 81, 10 }, /* glyph 190 */
		{ 122, 81, 10 }, /* glyph 191 */
		{ 133, 81, 10 }, /* glyph 192 */
		{ 144, 81, 10 }, /* glyph 193 */
		{ 155, 81, 10 }, /* glyph 194 */
		{ 166, 81, 10 }, /* glyph 195 */
		{ 177, 81, 10 }, /* glyph 196 */
		{ 188, 81, 10 }, /* glyph 197 */
		{ 199, 81, 10 }, /* glyph 198 */
		{ 210, 81, 10 }, /* glyph 199 */
		{ 221, 81, 10 }, /* glyph 200 */
		{ 232, 81, 10 }, /* glyph 201 */
		{ 243, 81, 10 }, /* glyph 202 */
		{ 1, 97, 10 }, /* glyph 203 */
		{ 12, 97, 10 }, /* glyph 204 */
		{ 23, 97, 10 }, /* glyph 205 */
		{ 34, 97, 10 }, /* glyph 206 */
		{ 45, 97, 10 }, /* glyph 207 */
		{ 56, 97, 10 }, /* glyph 208 */
		{ 67, 97, 10 }, /* glyph 209 */
		{ 78, 97, 10 }, /* glyph 210 */
		{ 89, 97, 10 }, /* glyph 211 */
		{ 100, 97, 10 }, /* glyph 212 */
		{ 111, 97, 10 }, /* glyph 213 */
		{ 122, 97, 10 }, /* glyph 214 */
		{ 133, 97, 10 }, /* glyph 215 */
		{ 144, 97, 10 }, /* glyph 216 */
		{ 155, 97, 10 }, /* glyph 217 */
		{ 166, 97, 10 }, /* glyph 218 */
		{ 177, 97, 10 }, /* glyph 219 */
		{ 188, 97, 10 }, /* glyph 220 */
		{ 199, 97, 10 }, /* glyph 221 */
		{ 210, 97, 10 }, /* glyph 222 */
		{ 221, 97, 10 }, /* glyph 223 */
		{ 232, 97, 10 }, /* glyph 224 */
		{ 243, 97, 10 }, /* glyph 225 */
		{ 1, 113, 10 }, /* glyph 226 */
		{ 12, 113, 10 }, /* glyph 227 */
		{ 23, 113, 10 }, /* glyph 228 */
		{ 34, 113, 10 }, /* glyph 229 */
		{ 45, 113, 10 }, /* glyph 230 */
		{ 56, 113, 10 }, /* glyph 231 */
		{ 67, 113, 10 }, /* glyph 232 */
		{ 78, 113, 10 }, /* glyph 233 */
		{ 89, 113, 10 }, /* glyph 234 */
		{ 100, 113, 10 }, /* glyph 235 */
		{ 111, 113, 10 }, /* glyph 236 */
		{ 122, 113, 10 }, /* glyph 237 */
		{ 133, 113, 10 }, /* glyph 238 */
		{ 144, 113, 10 }, /* glyph 239 */
		{ 155, 113, 10 }, /* glyph 240 */
		{ 166, 113, 10 }, /* glyph 241 */
		{ 177, 113, 10 }, /* glyph 242 */
		{ 188, 113, 10 }, /* glyph 243 */
		{ 199, 113, 10 }, /* glyph 244 */
		{ 210, 113, 10 }, /* glyph 245 */
		{ 221, 113, 10 }, /* glyph 246 */
		{ 232, 113, 10 }, /* glyph 247 */
		{ 243, 113, 10 }, /* glyph 248 */
		{ 1, 129, 10 }, /* glyph 249 */
		{ 12, 129, 10 }, /* glyph 250 */
		{ 23, 129, 10 }, /* glyph 251 */
		{ 34, 129, 10 }, /* glyph 252 */
		{ 45, 129, 10 }, /* glyph 253 */
		{ 56, 129, 10 }, /* glyph 254 */
		{ 67, 129, 10 }, /* glyph 255 */
	};
#endif
	
#if 0
	sTempGlyph tempGlyphs[ 256 ] = {
		{ 0, 0, 5 }, /* glyph 0 */
		{ 0, 0, 5 }, /* glyph 1 */
		{ 0, 0, 5 }, /* glyph 2 */
		{ 0, 0, 5 }, /* glyph 3 */
		{ 0, 0, 5 }, /* glyph 4 */
		{ 0, 0, 5 }, /* glyph 5 */
		{ 0, 0, 5 }, /* glyph 6 */
		{ 0, 0, 5 }, /* glyph 7 */
		{ 0, 0, 5 }, /* glyph 8 */
		{ 0, 0, 5 }, /* glyph 9 */
		{ 0, 0, 5 }, /* glyph 10 */
		{ 0, 0, 5 }, /* glyph 11 */
		{ 0, 0, 5 }, /* glyph 12 */
		{ 0, 0, 5 }, /* glyph 13 */
		{ 0, 0, 5 }, /* glyph 14 */
		{ 0, 0, 5 }, /* glyph 15 */
		{ 0, 0, 5 }, /* glyph 16 */
		{ 0, 0, 5 }, /* glyph 17 */
		{ 0, 0, 5 }, /* glyph 18 */
		{ 0, 0, 5 }, /* glyph 19 */
		{ 0, 0, 5 }, /* glyph 20 */
		{ 0, 0, 5 }, /* glyph 21 */
		{ 0, 0, 5 }, /* glyph 22 */
		{ 0, 0, 5 }, /* glyph 23 */
		{ 0, 0, 5 }, /* glyph 24 */
		{ 0, 0, 5 }, /* glyph 25 */
		{ 0, 0, 5 }, /* glyph 26 */
		{ 0, 0, 5 }, /* glyph 27 */
		{ 0, 0, 5 }, /* glyph 28 */
		{ 0, 0, 5 }, /* glyph 29 */
		{ 0, 0, 5 }, /* glyph 30 */
		{ 0, 0, 5 }, /* glyph 31 */
		
		{ 1, 1, 7 }, /* glyph 32 */
		{ 9, 1, 7 }, /* glyph 33 */
		{ 17, 1, 7 }, /* glyph 34 */
		{ 25, 1, 7 }, /* glyph 35 */
		{ 33, 1, 7 }, /* glyph 36 */
		{ 41, 1, 7 }, /* glyph 37 */
		{ 49, 1, 7 }, /* glyph 38 */
		{ 57, 1, 7 }, /* glyph 39 */
		{ 65, 1, 7 }, /* glyph 40 */
		{ 73, 1, 7 }, /* glyph 41 */
		{ 81, 1, 7 }, /* glyph 42 */
		{ 89, 1, 7 }, /* glyph 43 */
		{ 97, 1, 7 }, /* glyph 44 */
		{ 105, 1, 7 }, /* glyph 45 */
		{ 113, 1, 7 }, /* glyph 46 */
		{ 121, 1, 7 }, /* glyph 47 */
		{ 1, 12, 7 }, /* glyph 48 */
		{ 9, 12, 7 }, /* glyph 49 */
		{ 17, 12, 7 }, /* glyph 50 */
		{ 25, 12, 7 }, /* glyph 51 */
		{ 33, 12, 7 }, /* glyph 52 */
		{ 41, 12, 7 }, /* glyph 53 */
		{ 49, 12, 7 }, /* glyph 54 */
		{ 57, 12, 7 }, /* glyph 55 */
		{ 65, 12, 7 }, /* glyph 56 */
		{ 73, 12, 7 }, /* glyph 57 */
		{ 81, 12, 7 }, /* glyph 58 */
		{ 89, 12, 7 }, /* glyph 59 */
		{ 97, 12, 7 }, /* glyph 60 */
		{ 105, 12, 7 }, /* glyph 61 */
		{ 113, 12, 7 }, /* glyph 62 */
		{ 121, 12, 7 }, /* glyph 63 */
		{ 1, 23, 7 }, /* glyph 64 */
		{ 9, 23, 7 }, /* glyph 65 */
		{ 17, 23, 7 }, /* glyph 66 */
		{ 25, 23, 7 }, /* glyph 67 */
		{ 33, 23, 7 }, /* glyph 68 */
		{ 41, 23, 7 }, /* glyph 69 */
		{ 49, 23, 7 }, /* glyph 70 */
		{ 57, 23, 7 }, /* glyph 71 */
		{ 65, 23, 7 }, /* glyph 72 */
		{ 73, 23, 7 }, /* glyph 73 */
		{ 81, 23, 7 }, /* glyph 74 */
		{ 89, 23, 7 }, /* glyph 75 */
		{ 97, 23, 7 }, /* glyph 76 */
		{ 105, 23, 7 }, /* glyph 77 */
		{ 113, 23, 7 }, /* glyph 78 */
		{ 121, 23, 7 }, /* glyph 79 */
		{ 1, 34, 7 }, /* glyph 80 */
		{ 9, 34, 7 }, /* glyph 81 */
		{ 17, 34, 7 }, /* glyph 82 */
		{ 25, 34, 7 }, /* glyph 83 */
		{ 33, 34, 7 }, /* glyph 84 */
		{ 41, 34, 7 }, /* glyph 85 */
		{ 49, 34, 7 }, /* glyph 86 */
		{ 57, 34, 7 }, /* glyph 87 */
		{ 65, 34, 7 }, /* glyph 88 */
		{ 73, 34, 7 }, /* glyph 89 */
		{ 81, 34, 7 }, /* glyph 90 */
		{ 89, 34, 7 }, /* glyph 91 */
		{ 97, 34, 7 }, /* glyph 92 */
		{ 105, 34, 7 }, /* glyph 93 */
		{ 113, 34, 7 }, /* glyph 94 */
		{ 121, 34, 7 }, /* glyph 95 */
		{ 1, 45, 7 }, /* glyph 96 */
		{ 9, 45, 7 }, /* glyph 97 */
		{ 17, 45, 7 }, /* glyph 98 */
		{ 25, 45, 7 }, /* glyph 99 */
		{ 33, 45, 7 }, /* glyph 100 */
		{ 41, 45, 7 }, /* glyph 101 */
		{ 49, 45, 7 }, /* glyph 102 */
		{ 57, 45, 7 }, /* glyph 103 */
		{ 65, 45, 7 }, /* glyph 104 */
		{ 73, 45, 7 }, /* glyph 105 */
		{ 81, 45, 7 }, /* glyph 106 */
		{ 89, 45, 7 }, /* glyph 107 */
		{ 97, 45, 7 }, /* glyph 108 */
		{ 105, 45, 7 }, /* glyph 109 */
		{ 113, 45, 7 }, /* glyph 110 */
		{ 121, 45, 7 }, /* glyph 111 */
		{ 1, 56, 7 }, /* glyph 112 */
		{ 9, 56, 7 }, /* glyph 113 */
		{ 17, 56, 7 }, /* glyph 114 */
		{ 25, 56, 7 }, /* glyph 115 */
		{ 33, 56, 7 }, /* glyph 116 */
		{ 41, 56, 7 }, /* glyph 117 */
		{ 49, 56, 7 }, /* glyph 118 */
		{ 57, 56, 7 }, /* glyph 119 */
		{ 65, 56, 7 }, /* glyph 120 */
		{ 73, 56, 7 }, /* glyph 121 */
		{ 81, 56, 7 }, /* glyph 122 */
		{ 89, 56, 7 }, /* glyph 123 */
		{ 97, 56, 7 }, /* glyph 124 */
		{ 105, 56, 7 }, /* glyph 125 */
		{ 113, 56, 7 }, /* glyph 126 */
		{ 121, 56, 7 }, /* glyph 160 */
		{ 1, 67, 7 }, /* glyph 161 */
		{ 9, 67, 7 }, /* glyph 162 */
		{ 17, 67, 7 }, /* glyph 163 */
		{ 25, 67, 7 }, /* glyph 164 */
		{ 33, 67, 7 }, /* glyph 165 */
		{ 41, 67, 7 }, /* glyph 166 */
		{ 49, 67, 7 }, /* glyph 167 */
		{ 57, 67, 7 }, /* glyph 168 */
		{ 65, 67, 7 }, /* glyph 169 */
		{ 73, 67, 7 }, /* glyph 170 */
		{ 81, 67, 7 }, /* glyph 171 */
		{ 89, 67, 7 }, /* glyph 172 */
		{ 97, 67, 7 }, /* glyph 173 */
		{ 105, 67, 7 }, /* glyph 174 */
		{ 113, 67, 7 }, /* glyph 175 */
		{ 121, 67, 7 }, /* glyph 176 */
		{ 1, 78, 7 }, /* glyph 177 */
		{ 9, 78, 7 }, /* glyph 178 */
		{ 17, 78, 7 }, /* glyph 179 */
		{ 25, 78, 7 }, /* glyph 180 */
		{ 33, 78, 7 }, /* glyph 181 */
		{ 41, 78, 7 }, /* glyph 182 */
		{ 49, 78, 7 }, /* glyph 183 */
		{ 57, 78, 7 }, /* glyph 184 */
		{ 65, 78, 7 }, /* glyph 185 */
		{ 73, 78, 7 }, /* glyph 186 */
		{ 81, 78, 7 }, /* glyph 187 */
		{ 89, 78, 7 }, /* glyph 188 */
		{ 97, 78, 7 }, /* glyph 189 */
		{ 105, 78, 7 }, /* glyph 190 */
		{ 113, 78, 7 }, /* glyph 191 */
		{ 121, 78, 7 }, /* glyph 192 */
		{ 1, 89, 7 }, /* glyph 193 */
		{ 9, 89, 7 }, /* glyph 194 */
		{ 17, 89, 7 }, /* glyph 195 */
		{ 25, 89, 7 }, /* glyph 196 */
		{ 33, 89, 7 }, /* glyph 197 */
		{ 41, 89, 7 }, /* glyph 198 */
		{ 49, 89, 7 }, /* glyph 199 */
		{ 57, 89, 7 }, /* glyph 200 */
		{ 65, 89, 7 }, /* glyph 201 */
		{ 73, 89, 7 }, /* glyph 202 */
		{ 81, 89, 7 }, /* glyph 203 */
		{ 89, 89, 7 }, /* glyph 204 */
		{ 97, 89, 7 }, /* glyph 205 */
		{ 105, 89, 7 }, /* glyph 206 */
		{ 113, 89, 7 }, /* glyph 207 */
		{ 121, 89, 7 }, /* glyph 208 */
		{ 1, 100, 7 }, /* glyph 209 */
		{ 9, 100, 7 }, /* glyph 210 */
		{ 17, 100, 7 }, /* glyph 211 */
		{ 25, 100, 7 }, /* glyph 212 */
		{ 33, 100, 7 }, /* glyph 213 */
		{ 41, 100, 7 }, /* glyph 214 */
		{ 49, 100, 7 }, /* glyph 215 */
		{ 57, 100, 7 }, /* glyph 216 */
		{ 65, 100, 7 }, /* glyph 217 */
		{ 73, 100, 7 }, /* glyph 218 */
		{ 81, 100, 7 }, /* glyph 219 */
		{ 89, 100, 7 }, /* glyph 220 */
		{ 97, 100, 7 }, /* glyph 221 */
		{ 105, 100, 7 }, /* glyph 222 */
		{ 113, 100, 7 }, /* glyph 223 */
		{ 121, 100, 7 }, /* glyph 224 */
		{ 1, 111, 7 }, /* glyph 225 */
		{ 9, 111, 7 }, /* glyph 226 */
		{ 17, 111, 7 }, /* glyph 227 */
		{ 25, 111, 7 }, /* glyph 228 */
		{ 33, 111, 7 }, /* glyph 229 */
		{ 41, 111, 7 }, /* glyph 230 */
		{ 49, 111, 7 }, /* glyph 231 */
		{ 57, 111, 7 }, /* glyph 232 */
		{ 65, 111, 7 }, /* glyph 233 */
		{ 73, 111, 7 }, /* glyph 234 */
		{ 81, 111, 7 }, /* glyph 235 */
		{ 89, 111, 7 }, /* glyph 236 */
		{ 97, 111, 7 }, /* glyph 237 */
		{ 105, 111, 7 }, /* glyph 238 */
		{ 113, 111, 7 }, /* glyph 239 */
		{ 121, 111, 7 }, /* glyph 240 */
		{ 1, 122, 7 }, /* glyph 241 */
		{ 9, 122, 7 }, /* glyph 242 */
		{ 17, 122, 7 }, /* glyph 243 */
		{ 25, 122, 7 }, /* glyph 244 */
		{ 33, 122, 7 }, /* glyph 245 */
		{ 41, 122, 7 }, /* glyph 246 */
		{ 49, 122, 7 }, /* glyph 247 */
		{ 57, 122, 7 }, /* glyph 248 */
		{ 65, 122, 7 }, /* glyph 249 */
		{ 73, 122, 7 }, /* glyph 250 */
		{ 81, 122, 7 }, /* glyph 251 */
		{ 89, 122, 7 }, /* glyph 252 */
		{ 97, 122, 7 }, /* glyph 253 */
		{ 105, 122, 7 }, /* glyph 254 */
		{ 113, 122, 7 }, /* glyph 255 */
	};
#endif
	
	sTempGlyph tempGlyphs[ 256 ] = {
		{ 0, 0, 5 }, /* glyph 0 */
		{ 0, 0, 5 }, /* glyph 1 */
		{ 0, 0, 5 }, /* glyph 2 */
		{ 0, 0, 5 }, /* glyph 3 */
		{ 0, 0, 5 }, /* glyph 4 */
		{ 0, 0, 5 }, /* glyph 5 */
		{ 0, 0, 5 }, /* glyph 6 */
		{ 0, 0, 5 }, /* glyph 7 */
		{ 0, 0, 5 }, /* glyph 8 */
		{ 0, 0, 5 }, /* glyph 9 */
		{ 0, 0, 5 }, /* glyph 10 */
		{ 0, 0, 5 }, /* glyph 11 */
		{ 0, 0, 5 }, /* glyph 12 */
		{ 0, 0, 5 }, /* glyph 13 */
		{ 0, 0, 5 }, /* glyph 14 */
		{ 0, 0, 5 }, /* glyph 15 */
		{ 0, 0, 5 }, /* glyph 16 */
		{ 0, 0, 5 }, /* glyph 17 */
		{ 0, 0, 5 }, /* glyph 18 */
		{ 0, 0, 5 }, /* glyph 19 */
		{ 0, 0, 5 }, /* glyph 20 */
		{ 0, 0, 5 }, /* glyph 21 */
		{ 0, 0, 5 }, /* glyph 22 */
		{ 0, 0, 5 }, /* glyph 23 */
		{ 0, 0, 5 }, /* glyph 24 */
		{ 0, 0, 5 }, /* glyph 25 */
		{ 0, 0, 5 }, /* glyph 26 */
		{ 0, 0, 5 }, /* glyph 27 */
		{ 0, 0, 5 }, /* glyph 28 */
		{ 0, 0, 5 }, /* glyph 29 */
		{ 0, 0, 5 }, /* glyph 30 */
		{ 0, 0, 5 }, /* glyph 31 */
		
		{ 1, 1, 6 }, /* glyph 32 */
		{ 8, 1, 6 }, /* glyph 33 */
		{ 15, 1, 6 }, /* glyph 34 */
		{ 22, 1, 6 }, /* glyph 35 */
		{ 29, 1, 6 }, /* glyph 36 */
		{ 36, 1, 6 }, /* glyph 37 */
		{ 43, 1, 6 }, /* glyph 38 */
		{ 50, 1, 6 }, /* glyph 39 */
		{ 57, 1, 6 }, /* glyph 40 */
		{ 64, 1, 6 }, /* glyph 41 */
		{ 71, 1, 6 }, /* glyph 42 */
		{ 78, 1, 6 }, /* glyph 43 */
		{ 85, 1, 6 }, /* glyph 44 */
		{ 92, 1, 6 }, /* glyph 45 */
		{ 99, 1, 6 }, /* glyph 46 */
		{ 106, 1, 6 }, /* glyph 47 */
		{ 113, 1, 6 }, /* glyph 48 */
		{ 120, 1, 6 }, /* glyph 49 */
		{ 1, 12, 6 }, /* glyph 50 */
		{ 8, 12, 6 }, /* glyph 51 */
		{ 15, 12, 6 }, /* glyph 52 */
		{ 22, 12, 6 }, /* glyph 53 */
		{ 29, 12, 6 }, /* glyph 54 */
		{ 36, 12, 6 }, /* glyph 55 */
		{ 43, 12, 6 }, /* glyph 56 */
		{ 50, 12, 6 }, /* glyph 57 */
		{ 57, 12, 6 }, /* glyph 58 */
		{ 64, 12, 6 }, /* glyph 59 */
		{ 71, 12, 6 }, /* glyph 60 */
		{ 78, 12, 6 }, /* glyph 61 */
		{ 85, 12, 6 }, /* glyph 62 */
		{ 92, 12, 6 }, /* glyph 63 */
		{ 99, 12, 6 }, /* glyph 64 */
		{ 106, 12, 6 }, /* glyph 65 */
		{ 113, 12, 6 }, /* glyph 66 */
		{ 120, 12, 6 }, /* glyph 67 */
		{ 1, 23, 6 }, /* glyph 68 */
		{ 8, 23, 6 }, /* glyph 69 */
		{ 15, 23, 6 }, /* glyph 70 */
		{ 22, 23, 6 }, /* glyph 71 */
		{ 29, 23, 6 }, /* glyph 72 */
		{ 36, 23, 6 }, /* glyph 73 */
		{ 43, 23, 6 }, /* glyph 74 */
		{ 50, 23, 6 }, /* glyph 75 */
		{ 57, 23, 6 }, /* glyph 76 */
		{ 64, 23, 6 }, /* glyph 77 */
		{ 71, 23, 6 }, /* glyph 78 */
		{ 78, 23, 6 }, /* glyph 79 */
		{ 85, 23, 6 }, /* glyph 80 */
		{ 92, 23, 6 }, /* glyph 81 */
		{ 99, 23, 6 }, /* glyph 82 */
		{ 106, 23, 6 }, /* glyph 83 */
		{ 113, 23, 6 }, /* glyph 84 */
		{ 120, 23, 6 }, /* glyph 85 */
		{ 1, 34, 6 }, /* glyph 86 */
		{ 8, 34, 6 }, /* glyph 87 */
		{ 15, 34, 6 }, /* glyph 88 */
		{ 22, 34, 6 }, /* glyph 89 */
		{ 29, 34, 6 }, /* glyph 90 */
		{ 36, 34, 6 }, /* glyph 91 */
		{ 43, 34, 6 }, /* glyph 92 */
		{ 50, 34, 6 }, /* glyph 93 */
		{ 57, 34, 6 }, /* glyph 94 */
		{ 64, 34, 6 }, /* glyph 95 */
		{ 71, 34, 6 }, /* glyph 96 */
		{ 78, 34, 6 }, /* glyph 97 */
		{ 85, 34, 6 }, /* glyph 98 */
		{ 92, 34, 6 }, /* glyph 99 */
		{ 99, 34, 6 }, /* glyph 100 */
		{ 106, 34, 6 }, /* glyph 101 */
		{ 113, 34, 6 }, /* glyph 102 */
		{ 120, 34, 6 }, /* glyph 103 */
		{ 1, 45, 6 }, /* glyph 104 */
		{ 8, 45, 6 }, /* glyph 105 */
		{ 15, 45, 6 }, /* glyph 106 */
		{ 22, 45, 6 }, /* glyph 107 */
		{ 29, 45, 6 }, /* glyph 108 */
		{ 36, 45, 6 }, /* glyph 109 */
		{ 43, 45, 6 }, /* glyph 110 */
		{ 50, 45, 6 }, /* glyph 111 */
		{ 57, 45, 6 }, /* glyph 112 */
		{ 64, 45, 6 }, /* glyph 113 */
		{ 71, 45, 6 }, /* glyph 114 */
		{ 78, 45, 6 }, /* glyph 115 */
		{ 85, 45, 6 }, /* glyph 116 */
		{ 92, 45, 6 }, /* glyph 117 */
		{ 99, 45, 6 }, /* glyph 118 */
		{ 106, 45, 6 }, /* glyph 119 */
		{ 113, 45, 6 }, /* glyph 120 */
		{ 120, 45, 6 }, /* glyph 121 */
		{ 1, 56, 6 }, /* glyph 122 */
		{ 8, 56, 6 }, /* glyph 123 */
		{ 15, 56, 6 }, /* glyph 124 */
		{ 22, 56, 6 }, /* glyph 125 */
		{ 29, 56, 6 }, /* glyph 126 */
		{ 36, 56, 6 }, /* glyph 160 */
		{ 43, 56, 6 }, /* glyph 161 */
		{ 50, 56, 6 }, /* glyph 162 */
		{ 57, 56, 6 }, /* glyph 163 */
		{ 64, 56, 6 }, /* glyph 164 */
		{ 71, 56, 6 }, /* glyph 165 */
		{ 78, 56, 6 }, /* glyph 166 */
		{ 85, 56, 6 }, /* glyph 167 */
		{ 92, 56, 6 }, /* glyph 168 */
		{ 99, 56, 6 }, /* glyph 169 */
		{ 106, 56, 6 }, /* glyph 170 */
		{ 113, 56, 6 }, /* glyph 171 */
		{ 120, 56, 6 }, /* glyph 172 */
		{ 1, 67, 6 }, /* glyph 173 */
		{ 8, 67, 6 }, /* glyph 174 */
		{ 15, 67, 6 }, /* glyph 175 */
		{ 22, 67, 6 }, /* glyph 176 */
		{ 29, 67, 6 }, /* glyph 177 */
		{ 36, 67, 6 }, /* glyph 178 */
		{ 43, 67, 6 }, /* glyph 179 */
		{ 50, 67, 6 }, /* glyph 180 */
		{ 57, 67, 6 }, /* glyph 181 */
		{ 64, 67, 6 }, /* glyph 182 */
		{ 71, 67, 6 }, /* glyph 183 */
		{ 78, 67, 6 }, /* glyph 184 */
		{ 85, 67, 6 }, /* glyph 185 */
		{ 92, 67, 6 }, /* glyph 186 */
		{ 99, 67, 6 }, /* glyph 187 */
		{ 106, 67, 6 }, /* glyph 188 */
		{ 113, 67, 6 }, /* glyph 189 */
		{ 120, 67, 6 }, /* glyph 190 */
		{ 1, 78, 6 }, /* glyph 191 */
		{ 8, 78, 6 }, /* glyph 192 */
		{ 15, 78, 6 }, /* glyph 193 */
		{ 22, 78, 6 }, /* glyph 194 */
		{ 29, 78, 6 }, /* glyph 195 */
		{ 36, 78, 6 }, /* glyph 196 */
		{ 43, 78, 6 }, /* glyph 197 */
		{ 50, 78, 6 }, /* glyph 198 */
		{ 57, 78, 6 }, /* glyph 199 */
		{ 64, 78, 6 }, /* glyph 200 */
		{ 71, 78, 6 }, /* glyph 201 */
		{ 78, 78, 6 }, /* glyph 202 */
		{ 85, 78, 6 }, /* glyph 203 */
		{ 92, 78, 6 }, /* glyph 204 */
		{ 99, 78, 6 }, /* glyph 205 */
		{ 106, 78, 6 }, /* glyph 206 */
		{ 113, 78, 6 }, /* glyph 207 */
		{ 120, 78, 6 }, /* glyph 208 */
		{ 1, 89, 6 }, /* glyph 209 */
		{ 8, 89, 6 }, /* glyph 210 */
		{ 15, 89, 6 }, /* glyph 211 */
		{ 22, 89, 6 }, /* glyph 212 */
		{ 29, 89, 6 }, /* glyph 213 */
		{ 36, 89, 6 }, /* glyph 214 */
		{ 43, 89, 6 }, /* glyph 215 */
		{ 50, 89, 6 }, /* glyph 216 */
		{ 57, 89, 6 }, /* glyph 217 */
		{ 64, 89, 6 }, /* glyph 218 */
		{ 71, 89, 6 }, /* glyph 219 */
		{ 78, 89, 6 }, /* glyph 220 */
		{ 85, 89, 6 }, /* glyph 221 */
		{ 92, 89, 6 }, /* glyph 222 */
		{ 99, 89, 6 }, /* glyph 223 */
		{ 106, 89, 6 }, /* glyph 224 */
		{ 113, 89, 6 }, /* glyph 225 */
		{ 120, 89, 6 }, /* glyph 226 */
		{ 1, 100, 6 }, /* glyph 227 */
		{ 8, 100, 6 }, /* glyph 228 */
		{ 15, 100, 6 }, /* glyph 229 */
		{ 22, 100, 6 }, /* glyph 230 */
		{ 29, 100, 6 }, /* glyph 231 */
		{ 36, 100, 6 }, /* glyph 232 */
		{ 43, 100, 6 }, /* glyph 233 */
		{ 50, 100, 6 }, /* glyph 234 */
		{ 57, 100, 6 }, /* glyph 235 */
		{ 64, 100, 6 }, /* glyph 236 */
		{ 71, 100, 6 }, /* glyph 237 */
		{ 78, 100, 6 }, /* glyph 238 */
		{ 85, 100, 6 }, /* glyph 239 */
		{ 92, 100, 6 }, /* glyph 240 */
		{ 99, 100, 6 }, /* glyph 241 */
		{ 106, 100, 6 }, /* glyph 242 */
		{ 113, 100, 6 }, /* glyph 243 */
		{ 120, 100, 6 }, /* glyph 244 */
		{ 1, 111, 6 }, /* glyph 245 */
		{ 8, 111, 6 }, /* glyph 246 */
		{ 15, 111, 6 }, /* glyph 247 */
		{ 22, 111, 6 }, /* glyph 248 */
		{ 29, 111, 6 }, /* glyph 249 */
		{ 36, 111, 6 }, /* glyph 250 */
		{ 43, 111, 6 }, /* glyph 251 */
		{ 50, 111, 6 }, /* glyph 252 */
		{ 57, 111, 6 }, /* glyph 253 */
		{ 64, 111, 6 }, /* glyph 254 */
		{ 71, 111, 6 }, /* glyph 255 */
	};
	
	const int glyphOffset = -1;
	const int glyphEnlarge = 1;
	
	pGlyphs = new sGlyph[ 256 ];
	
	factorU = 1.0f / ( float )gimp_debugfont.width;
	factorV = 1.0f / ( float )gimp_debugfont.height;
	
	offsetU = config.GetTextOffsetU() * factorU;
	offsetV = config.GetTextOffsetV() * factorV;
	//pFontHeight = 15;
	pFontHeight = 10;
	
	for( i=0; i<256; i++ ){
		pGlyphs[ i ].x1 = factorU * ( float )( tempGlyphs[ i ].u + glyphOffset ) + offsetU;
		pGlyphs[ i ].y1 = factorV * ( float )( tempGlyphs[ i ].v ) + offsetV;
		pGlyphs[ i ].x2 = factorU * ( float )( tempGlyphs[ i ].u + glyphOffset + tempGlyphs[ i ].width + glyphEnlarge ) + offsetU;
		pGlyphs[ i ].y2 = factorV * ( float )( tempGlyphs[ i ].v + pFontHeight ) + offsetV;
		pGlyphs[ i ].width = tempGlyphs[ i ].width + glyphEnlarge;
		pGlyphs[ i ].height = pFontHeight;
		pGlyphs[ i ].advance = tempGlyphs[ i ].width + glyphEnlarge;
	}
}


// reimplementation of GIMP_DEBUGFONT_RUN_LENGTH_DECODE to not fail GCC sanitizer checks
void gimpDecodeRLE( unsigned char * const image_buf, const unsigned char *rle_data, int size, int bpp){
	const int pixelCount = size * bpp;
	int pixelIndex = 0;
	
	if (bpp > 3) {
		/* RGBA */
		while( pixelIndex < pixelCount ) {
			unsigned int rle_data_cur = *(rle_data++);
			
			if (rle_data_cur & 128) {
				rle_data_cur = rle_data_cur - 128;
				do {
					memcpy (image_buf + pixelIndex, rle_data, 4);
					pixelIndex += 4;
				}while (--rle_data_cur);
				rle_data += 4;
				
			} else {
				rle_data_cur *= 4;
				memcpy (image_buf + pixelIndex, rle_data, rle_data_cur);
				pixelIndex += rle_data_cur;
				rle_data += rle_data_cur;
			}
		}
		
	}else{
		/* RGB */
		while( pixelIndex < pixelCount ) {
			unsigned int rle_data_cur = *(rle_data++);
			
			if (rle_data_cur & 128) {
				rle_data_cur = rle_data_cur - 128;
				do {
					memcpy (image_buf + pixelIndex, rle_data, 3);
					pixelIndex += 3;
				}while (--rle_data_cur);
				rle_data += 3;
				
			} else {
				rle_data_cur *= 3;
				memcpy (image_buf + pixelIndex, rle_data, rle_data_cur);
				pixelIndex += rle_data_cur;
				rle_data += rle_data_cur;
			}
		}
	}
}

void deoglDebugFont::pBuildTexture(){
	deoglPixelBuffer pixbuf( deoglPixelBuffer::epfByte4, gimp_debugfont.width, gimp_debugfont.height, 1 );
	//deoglPixelBuffer pixbuf2( deoglPixelBuffer::epfByte4, gimp_debugfont.width, gimp_debugfont.height, 1 );
	//unsigned int i, line = gimp_debugfont.width * gimp_debugfont.bytes_per_pixel;
	
	// GIMP_DEBUGFONT_RUN_LENGTH_DECODE
	gimpDecodeRLE( ( unsigned char * )pixbuf.GetPointerByte4(), &gimp_debugfont.rle_pixel_data[ 0 ],
		gimp_debugfont.width * gimp_debugfont.height, gimp_debugfont.bytes_per_pixel );
	
	//for( i=0; i<gimp_debugfont.height; i++ ){
	//	memcpy( pixbuf2.GetPointerInt32() + gimp_debugfont.width * i, pixbuf.GetPointerInt32() + gimp_debugfont.width * ( gimp_debugfont.height - i - 1 ), line );
	//}
	
	pTexture = new deoglTexture( pRenderThread );
	
	pTexture->SetSize( gimp_debugfont.width, gimp_debugfont.height );
	pTexture->SetMapingFormat( 4, false, false );
	
	pTexture->SetPixels( pixbuf );
}
