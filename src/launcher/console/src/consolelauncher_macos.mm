/*
 * Drag[en]gine Console Launcher
 *
 * Copyright (C) 2017, Plüss Roland ( roland@rptd.ch )
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

#import <Cocoa/Cocoa.h>

int macosMain( int argc, char **argv );

int main( int argc, char * argv[] ){
	NSApplication *app = [NSApplication sharedApplication];
	
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	[app finishLaunching];
	
	int result = macosMain( argc, argv );
	
	[pool release];
	return result;
	
	
	
	// run loop. works similar to NSApplication.run but allows to operate
	// the game engine at full performance. kept here for reference for
	// gui launcher to use later on
	/*
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	[app finishLaunching];
	
	shouldKeepRunning = YES;
	while(shouldKeepRunning){
		[pool release];
		pool = [[NSAutoreleasePool alloc] init];
		
		NSEvent *event = [app nextEventMatchingMask:NSAnyEventMask untilDate:nil inMode:NSDefaultRunLoopMode dequeue:YES];
		
		if(event){
			// process event
			[app sendEvent:event];
			[app updateWindows];
			
		}else{
			// run game loop
		}
	};
	[pool release];
	*/
}
