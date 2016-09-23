#ifndef MOCKCHANNEL_H
#define MOCKCHANNEL_H
/////////////////////////////////////////////////////////////////////////////
// MockChannel.h - solution for Project 4, support for WPF, S16            //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016               //
//  ver 1.1                                                                //
// Application: OOD Project #4                                             //
//  Source:     Jim Fawcett, CSE687 - Object Oriented Design,              //  
//              Spring 2015                                                //
//  Author:		Cunyang Shui, Computer Engineering   		               //
//				cushui@syr.edu								               //
/////////////////////////////////////////////////////////////////////////////
/*
* This package is a mock channel. support for WPF /clr working with dll
*/
/*
*  Required Files:
*  --------------
*   MockChannel.h, MockChannel.cpp
*   ReceiveHandle.h

*  Build Command:
*  --------------
*  devenv Project4.sln /rebuild debug
*/
/*
*  Maintenance History :
*--------------------
*  ver 1.1 : 3 May 2016
* - reference from MsgClient.cpp
* - add message process functions
*/

#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

#include <string>
using Message = std::string;

struct ISendr
{
	virtual void postMessage(const Message& msg) = 0;
};

struct IRecvr
{
	virtual std::string getMessage() = 0;
};

struct IMockChannel
{
public:
	virtual void start() = 0;
	virtual void stop() = 0;
};

extern "C" {
	struct ObjectFactory
	{
		DLL_DECL ISendr* createSendr();
		DLL_DECL IRecvr* createRecvr();
		DLL_DECL IMockChannel* createMockChannel(ISendr* pISendr, IRecvr* pIRecvr);
	};
}

#endif


