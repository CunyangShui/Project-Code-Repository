#pragma once

///////////////////////////////////////////////////////////////////////////
// HttpMessage.h - http style message generator                          //
//  ver 1.1                                                              //
//  Source:     Jim Fawcett, CSE687 - Object Oriented Design,            //
//              Spring 2016                                              //
//  Author:		Cunyang Shui, Computer Engineering   		             //
//				cushui@syr.edu								             //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package generate a http style messagem, transfer message between client and server
*
*  Required Files:
*  ---------------
*  HttpMessage.cpp, HttpMessage.h,
*  Utilities.h, Utilities.cpp
*
*  Build Command:
*  --------------
*  devenv Project4.sln /rebuild debug
*
*  Maintenance History:
*  --------------------
*  ver 1.1 : 3 May 2016
*  - first release
*/

#include <vector>
#include <string>
#include <array>

class HttpMessage
{
public:
	using byte = char;
	using Name = std::string;
	using Value = std::string;
	using Attribute = std::pair<Name, Value>;
	using Attributes = std::vector<Attribute>;
	using Terminator = std::string;
	using Body = std::vector<byte>;

	// message attributes
	void addAttribute(const Attribute& attrib);
	Value findValue(const Name& name);
	size_t findAttribute(const Name& name);
	bool removeAttribute(const Name& name);
	Attributes& attributes();
	static std::string attribString(const Attribute& attrib);
	static Attribute attribute(const Name& name, const Value& value);
	static Attribute parseAttribute(const std::string& src);

	// message body
	void setBody(byte buffer[], size_t Buflen);
	size_t getBody(byte buffer[], size_t& Buflen);
	void addBody(const Body& body);
	void addBody(const std::string& body);
	void addBody(size_t numBytes, byte* pBuffer);
	Body& body();
	size_t bodyLength();

	// display
	std::string headerString() const;
	std::string bodyString() const;
	std::string toString() const;
	std::string toIndentedHeaderString() const;
	std::string toIndentedBodyString() const;
	std::string toIndentedString() const;

	// cleanup
	void clear();
	static void fillBuffer(byte buffer[], size_t BufSize, byte fill = '\0');

private:
	Attributes attributes_;
	Terminator term_ = "\n";
	Body body_;
};