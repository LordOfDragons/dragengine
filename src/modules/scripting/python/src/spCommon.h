/* 
 * Drag[en]gine Python Script Module
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

#ifndef _SPCOMMON_H_
#define _SPCOMMON_H_

// includes
// #ifdef HAVE_PYTHON_2_5_H
// 	#define HAVE_PYTHON_2_X 1
// 	#include <python2.5/Python.h>
// 	
// #elif defined( HAVE_PYTHON_2_6_H )
// 	#define HAVE_PYTHON_2_X 1
// 	#include <python2.6/Python.h>
// 	
// #elif defined( HAVE_PYTHON_2_7_H )
// 	#define HAVE_PYTHON_2_X 1
// 	#include <python2.7/Python.h>
// 	
// #elif defined( HAVE_PYTHON_3_3_H )
#ifdef HAVE_PYTHON_3_3_H
	#include <python3.3/Python.h>
	
#elif defined HAVE_PYTHON_3_4_H
	#include <python3.4/Python.h>
	
#elif defined HAVE_PYTHON_3_4m_H
	#include <python3.4m/Python.h>
	
#else
	#error No Usable Python Found
#endif

#define SP_INTERNAL_DATA	"_SP_Internal_Data_"

#endif
