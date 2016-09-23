#pragma once
/////////////////////////////////////////////////////////////////////////////
// Receivehandle.h - handel receive message in client, S16                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016               //
//  ver 1.1                                                                //
// Application: OOD Project #4                                             //
//  Source:     Jim Fawcett, CSE687 - Object Oriented Design,              //  
//              Spring 2015                                                //
//  Author:		Cunyang Shui, Computer Engineering   		               //
//				cushui@syr.edu								               //
/////////////////////////////////////////////////////////////////////////////
/*
* This package is support for clientAgent, receive message from server
*/
/*
*  Required Files:
*  --------------
*   ReceiveHandle.h, ReceiveHandle.cpp
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
#include <string>
#include "../ThreadPool/BlockingQueue.h"
#include "../RemoteSupport/HttpMessage.h"
#include "../RemoteSupport/Sockets.h"
#include "../RemoteSupport/Logger.h"
#include "../Utilities/Utilities.h"
#include "../FileMgr/FileSystem.h"
using namespace Scanner;
using Show = StaticLogger<1>;
class ReceiveHandler
{
	using EndPoint = std::string;
public:
	ReceiveHandler(BlockingQueue<HttpMessage>& msgQ) : msgQ_(msgQ) {}
	void operator()(Socket socket);
private:
	bool connectionClosed_;
	HttpMessage readMessage(Socket& socket);
	bool readFile(const std::string& filename, size_t fileSize, Socket& socket);
	BlockingQueue<HttpMessage>& msgQ_;
};

