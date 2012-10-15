// Copyright (C) 2003 David Griffiths <dave@pawfal.org>
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

#include <unistd.h>
#include <sys/time.h>
#include <spiralcore/NoteTable.h>
#include "SyncUp.h"

using namespace spiralcore;

EventQueue SyncUp::m_EventQueue;
float SyncUp::m_Delay = 0;
float SyncUp::m_ClockBPM = 0;
int SyncUp::m_ClockBPB = 0;

SyncUp::SyncUp(const string &Port, const string &DestPort, const vector<string> &ClockDestPorts)
{
	cerr<<"Starting syncup on "<<Port<<" sending to "<<DestPort<<endl;
	m_Destination=lo_address_new_from_url(DestPort.c_str());
	
	for (vector<string>::const_iterator i=ClockDestPorts.begin(); i!=ClockDestPorts.end(); ++i)
	{ 
		cerr<<"Adding clock destination: "<<(*i)<<endl;
		m_ClockDestPorts.push_back(lo_address_new_from_url(i->c_str()));
    }
	
	m_Server = lo_server_thread_new(Port.c_str(), ErrorHandler);
    lo_server_thread_add_method(m_Server, NULL, NULL, DefaultHandler, NULL);
    lo_server_thread_add_method(m_Server, "/sync", "if", SyncHandler, this);
    lo_server_thread_add_method(m_Server, "/delay", "f", DelayHandler, this);
    lo_server_thread_add_method(m_Server, "/clock", "if", ClockHandler, this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Audio thread side

void SyncUp::Run()
{		
	lo_server_thread_start(m_Server);
	Time TimeNow;
	Time TimeThen;
	double Clock=0;
	Time NextTick;
	
    while (1) 
	{
		TimeNow.SetToNow();
		
		if (m_ClockBPM>0 && m_ClockBPB>0)
		{
			double NewClock = m_ClockBPB*(1/(m_ClockBPM/60.0f));
			
			if (Clock!=NewClock || TimeNow>NextTick)
			{
				Clock = NewClock;
				NextTick=TimeNow;
				NextTick+=Clock;
				
				for (vector<lo_address>::iterator i=m_ClockDestPorts.begin(); i!=m_ClockDestPorts.end(); ++i)
				{
			    	lo_send(*i, "/sync", "if", m_ClockBPB, m_ClockBPM);
					cerr<<"clock sending sync..."<<endl;
				}	
			}
		}
		
		TimeThen=TimeNow;
		usleep(100); 
	}  
}

void SyncUp::ErrorHandler(int num, const char *msg, const char *path)
{
    //cerr<<"liblo server error "<<num<<" in path "<<path<<": "<<msg<<endl;
    cerr<<"liblo server error "<<num<<endl;
}

int SyncUp::DefaultHandler(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
	return 1;
}

int SyncUp::DelayHandler(const char *path, const char *types, lo_arg **argv,
		    int argc, void *data, void *user_data)
{
	SyncUp *server = (SyncUp*)user_data;
	server->m_Delay=argv[0]->f;
	cerr<<"delay now: "<<server->m_Delay<<endl;
	return 1;
}

int SyncUp::ClockHandler(const char *path, const char *types, lo_arg **argv,
		    int argc, void *data, void *user_data)
{
	SyncUp *server = (SyncUp*)user_data;
	server->m_ClockBPB=argv[0]->i;
	server->m_ClockBPM=argv[1]->f;
	return 1;
}


int SyncUp::SyncHandler(const char *path, const char *types, lo_arg **argv,
		    int argc, void *data, void *user_data)
{
	SyncUp *server = (SyncUp*)user_data;
	
	if (types[0]=='i' && types[1]=='f')
	{
		int beatsperbar = argv[0]->i;
		float beatsperminute = argv[1]->f;
		
		cerr<<"syncup: sync to "<<beatsperbar<<"bpb "<<beatsperminute<<"bpm"<<endl;
		Time time;
		time.SetToNow();
		time+=m_Delay;
		
		if (beatsperminute>0)
		{
			// advance to next bar sync
			//time+=beatsperbar*(1/(beatsperminute/60.0f));

			time.Print();

			// send the sync out
			lo_message oscmsg=lo_message_new();
			lo_message_add_int64(oscmsg,time.Seconds); 
			lo_message_add_int64(oscmsg,time.Fraction); 
			lo_message_add_int32(oscmsg,beatsperbar); 
			lo_message_add_float(oscmsg,beatsperminute); 
			lo_send_message(server->m_Destination, "/sync", oscmsg);
			lo_message_free(oscmsg);
		}
	}
	return 1;
}
