// Copyright (C) 2004 David Griffiths <dave@pawfal.org>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#include <iostream>
#include "SyncUp.h"

int main(int argc, char **argv)
{
	if (argc<3)
	{
		cerr<<"-------------------------------------------"<<endl;
		cerr<<"syncup - for all your clock and sync needs!"<<endl;
		cerr<<"-------------------------------------------"<<endl;
		cerr<<"usage: syncup oscportnumber destport clockdest1 clockdest2 ..."<<endl<<endl;
		
		cerr<<"Sync Mode:"<<endl; 
		cerr<<"Listens to oscportnumber for [/sync if : bpb bpm] "<<endl;
		cerr<<"then calcuates next bar start time, and sends a similar "<<endl;
		cerr<<"message [/sync iif : timesecs timefrac bpm] with a time"<<endl;
		cerr<<"stamp for the next bar start time to destport."<<endl;
		cerr<<"This is useful because the application being synced"<<endl;		
		cerr<<"can do so accurately within it's own time - it's probably"<<endl;		
		cerr<<"already a bit late by the time we get it, so it minimises"<<endl;		
		cerr<<"the error."<<endl;		
		cerr<<"Use [/delay f : seconds] to add a delay offset."<<endl<<endl;
		
		cerr<<"Clock mode:"<<endl; 
		cerr<<"When started with [/clock if : bpb bpm] to oscportnumber,"<<endl;
		cerr<<"sends [/sync if : bpb bpm] to all clockdest's periodically."<<endl<<endl;
		
		cerr<<"To use sync mode with the clock, set one of the "<<endl;
		cerr<<"clockdest ports to oscportnumber."<<endl;
		return -1;
	}

	vector<string> clockports;
	for (int i=3; i<argc; i++)
	{
		clockports.push_back(argv[i]);
	}

	SyncUp engine(argv[1],argv[2],clockports);
	engine.Run();
	return 0;
}
