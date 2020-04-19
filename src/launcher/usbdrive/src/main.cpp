/* 
 * Drag[en]gine USB Launcher
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

#include "foxtoolkit.h"
#include "windowMain.h"


int main( int argc, char **argv ){
	FXApp * const app = new FXApp( "Drag[en]gine USB Drive Launcher", "Drag[en]gine" );
	app->init( argc, argv );
	
	new FXToolTip( app );
	app->create();
	
	WindowMain * const window = new WindowMain( app );
	window->create();
	
	return app->run();
}
