#pragma once

/////////////////////////////////////////////////////////////////////////
// ServerRepository.h - Server in Project 4, OOD, S16                  //
// Application: OOD Project #4                                         //
// Platform:    Macbook Pro                                            //
//  Source:     Jim Fawcett, CSE687 - Object Oriented Design,          //
//              Spring 2016                                            //
//  Author:		Cunyang Shui, Computer Engineering                     //
//				cushui@syr.edu						                   //
/////////////////////////////////////////////////////////////////////////
/*
* This package implements a server repository, used to handle checked in
* package.
*/
/*
* Public inter face :
*   checkinDone();
*
* Required Files:
*   ServerRepository.cpp, ServerRepository.h
*   Utilities.h, Utilities.cpp
*  Maintenance History:
*  --------------------
*  ver 1.0 : 3 Mar 16
*  - first release
*/
#include <string>
#include <map>
#include <list>
#include <Windows.h>

#include "../XMLEngine/XmlParser.h"
#include "../XMLEngine/XmlDocument.h"
#include "../FileMgr/FileMgr.h"
#include "../ParallelDependencyAnalysis/ParaDepAnalysis.h"

using namespace std;
using namespace XmlProcessing;
using sPtr = std::shared_ptr < AbstractXmlElement >;

struct Package {
	string fullName;
	string shortName;
	string timeString;
	vector<string> files;
	vector<string> depPkgs;
};

class ServerRep {
public:
	ServerRep(string directory) : _repDir(directory) {
		// _relaTable = read all xml file and get this table
	}
	void checkinDone(string pkgName);
	void cancelCheckin(string pkgName);
	~ServerRep() {}
	void createMetadateXML(string directory, string pkgName, vector<string> depPkgs, vector<string> files, time_t timeUnix, string timeString, string user, string descr = "Description");
	std::string timeConverter(time_t timev);
	void initRep();
	list<Package> getAllPkgs() { return _allPkgs; }

private:
	bool _autoCheckDep = true;
	string _repDir;
	map<string, list<string>> _relaTable;
	list<Package> _latestPkgs;
	list<Package> _allPkgs;
	void latestPkg();
	void removePkgFromLatest(string shName);
	string getShortPkgNameFromFileName(string fileName);
	string getShortPkgNameFromPkgName(string pkgName);
	string getFileNameFromFull(string fileName);
};