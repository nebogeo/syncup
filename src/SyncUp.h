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

#include <pthread.h>
#include <lo/lo.h>
#include <iostream>
#include <vector>
#include <spiralcore/Types.h>
#include <spiralcore/Time.h>
#include <spiralcore/Trace.h>
#include <spiralcore/EventQueue.h>

#ifndef KITCHEN_SYNC
#define KITCHEN_SYNC

using namespace std;
using namespace spiralcore;

class SyncUp
{
public:
	SyncUp(const string &Port, const string &DestPort, const vector<string> &ClockDestPorts);
	~SyncUp() {}
	
	void Run();
	
private:
	static int SyncHandler(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);	
	static int DelayHandler(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);	
	static int ClockHandler(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);	
	static int DefaultHandler(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);
	static void ErrorHandler(int num, const char *m, const char *path);	
	
	static EventQueue m_EventQueue;
	static float m_Delay;
	static float m_ClockBPM;
	static int m_ClockBPB;
	
	lo_server_thread m_Server;
	lo_address m_Destination;
	vector<lo_address> m_ClockDestPorts;
};

#endif
