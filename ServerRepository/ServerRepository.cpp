/////////////////////////////////////////////////////////////////////////
// ServerRepository.cpp - Server in Project 4, OOD, S16                //
// Application: OOD Project #4                                         //
// Platform:    Macbook Pro                                            //
//  Source:     Jim Fawcett, CSE687 - Object Oriented Design,          //
//              Spring 2016                                            //
//  Author:		Cunyang Shui, Computer Engineering                     //
//				cushui@syr.edu						                   //
/////////////////////////////////////////////////////////////////////////
#include "ServerRepository.h"

// ---< process check in close, create metadata--->
void ServerRep::checkinDone(string pkgName) {
	cout << "I'm check in done function" << endl;
	Package pkg;
	time_t timev(0);
	time(&timev);
	string timeString = timeConverter(timev);
	string fullName = pkgName + "_" + timeString;
	string directory = "repository\\" + fullName;
	const char *dir = directory.c_str();
	CreateDirectory(dir, NULL);

	vector<string> allFiles;
	FileMgr fm("..\\ServerRcv");
	fm.addPattern(pkgName + "*");
	fm.search();
	std::vector<std::string> ds = fm.getStores();
	cout << endl;
	//copy them to right place
	for (auto fromFile : ds)
	{
		string affix = fromFile;
		size_t pos = fromFile.find_last_of(".");
		affix.erase(0, pos);
		string toFile = directory + "\\" + fullName + affix;
		cout << timeString << timeString << endl;
		cout << fromFile << endl;
		cout << toFile << endl;
		pkg.files.push_back(toFile);
		allFiles.push_back(toFile);
		MoveFile(fromFile.c_str(), toFile.c_str());
	}

	pkg.fullName = fullName;
	pkg.shortName = pkgName;
	ostringstream convert;
	convert << timev;
	pkg.timeString = convert.str();
	removePkgFromLatest(pkgName);  //need to add pkg.dep 
	for (auto pkg : _latestPkgs)		//get all useful files
	{
		for (auto x : pkg.files)
		{
			allFiles.push_back(x);
		}
	}

	//std::vector<std::string> ds = fm.getStores();
	ParaDepAnalysis *para = new ParaDepAnalysis(allFiles);
	para->doParaTypeAnalysis();
	Display display;
	display.showTypeTable(para->getTypeTable());

	para->doParaDepAnalysis();
	display.showDepTable(para->getDepTable());

	para->combineDependency();
	display.showCombindResult(para->getCombinedResult());

	map<string, list<string>> fileDep = para->getCombinedResult();
	map<string, list<string>>::iterator my_Itr;
	for (my_Itr = fileDep.begin(); my_Itr != fileDep.end(); ++my_Itr)
	{
		cout << getShortPkgNameFromFileName(my_Itr->first) << endl;
		if (getShortPkgNameFromFileName(my_Itr->first) == pkgName)
		{
			for (auto x : my_Itr->second)
			{
				bool exist = false;
				x.erase(x.find_last_of("."), x.size() - 1);
				if (x.find("\\")) x.erase(0, x.find_last_of("\\") + 1);
				for (auto m : pkg.depPkgs)
					if (m == x)
					{
						exist = true;
					}
				if (!exist) pkg.depPkgs.push_back(x);
			}

		}
	}
	_latestPkgs.push_back(pkg);
	_allPkgs.push_back(pkg);

	for (auto pkg2 : _latestPkgs)
	{
		cout << endl;
		cout << endl;
		cout << pkg2.fullName << endl;
		cout << pkg2.shortName << endl;
		cout << pkg2.timeString << endl;
		for (auto s : pkg2.depPkgs)
			cout << s << endl;
		for (auto s : pkg2.files)
			cout << s << endl;
	}
	createMetadateXML(directory, pkgName, pkg.depPkgs, pkg.files, timev, timeString, "client_1");
}
// ---< cancel check in --->

void ServerRep::cancelCheckin(string pkgName) {
}

// ---< createxml file(metadata)--->

void ServerRep::createMetadateXML(string directory, string pkgName, vector<string> depPkgs, vector<string> files, time_t timeUnix, string timeString, string user, string descr) {
	XmlDocument newDoc;
	sPtr pRoot = makeTaggedElement("metadata");
	sPtr docEle = XmlProcessing::makeDocElement();
	sPtr dclEle = XmlProcessing::makeXmlDeclarElement();
	dclEle->addAttrib("version", "1.0");
	dclEle->addAttrib("encoding", "utf-8");
	docEle->addChild(dclEle);
	docEle->addChild(XmlProcessing::makeCommentElement("metadata of package " + pkgName + "_" + timeString));
	docEle->addChild(pRoot);
	XmlDocument doc(docEle);
	doc.xmlRoot(pRoot);

	sPtr child1 = makeTaggedElement("package");
	child1->addChild(makeTextElement(pkgName + "_" + timeString));
	sPtr child2 = makeTaggedElement("user");
	child2->addChild(makeTextElement(user));
	sPtr child3 = makeTaggedElement("timeStamp");
	ostringstream convert;
	convert << timeUnix;
	child3->addChild(makeTextElement(convert.str()));
	sPtr child4 = makeTaggedElement("files");
	sPtr child5 = makeTaggedElement("dependency");
	sPtr child6 = makeTaggedElement("description");
	child6->addChild(makeTextElement(descr));

	for (auto x : files) {
		sPtr grandChild = makeTaggedElement("fileContain");
		grandChild->addChild(makeTextElement(getFileNameFromFull(x)));
		child4->addChild(grandChild);
	}

	for (auto x : depPkgs) {
		sPtr grandChild = makeTaggedElement("depPackage");
		grandChild->addChild(makeTextElement(x));
		child5->addChild(grandChild);
	}
	pRoot->addChild(child1);
	pRoot->addChild(child2);
	pRoot->addChild(child3);
	pRoot->addChild(child4);
	pRoot->addChild(child5);
	pRoot->addChild(child6);

	std::cout << "\n" << doc.toString() << "\n\n";
	string file = directory + "/" + pkgName + "_" + timeString + ".xml";
	doc.save(file);
}
// ---< time_t convert to string --->

std::string ServerRep::timeConverter(time_t timev) {
	std::string timestring;
	char str[30];
	ctime_s(str, 30, &timev);

	str[3] = '_';
	str[7] = '_';
	str[10] = '_';
	str[13] = '_';
	str[16] = '_';
	if (str[8] == 32)
		str[8] = '0';
	str[0] = str[20];
	str[1] = str[21];
	str[2] = str[22];
	str[3] = str[23];
	str[21] = '\0';
	for (int i = 20; i >= 5; i--)
		str[i] = str[i - 1];
	str[4] = '_';
	str[20] = '\0';
	timestring = str;
	//std::cout << timev << std::endl;
	//std::cout << timestring << std::endl;
	return timestring;
}

// ---< read packages in repository--->
void ServerRep::initRep() {
	FileMgr fm("repository");
	fm.addPattern("*.xml");
	fm.search();

	map<string, list<string>> tempRelaTable;
	std::vector<std::string> ds = fm.getStores();
	for (auto fs : ds) {
		Package pkg;
		XmlParser parser(fs);
		parser.verbose(false); //false to not display details
		XmlDocument *pDoc = parser.buildDocument();
		std::cout << "\n" << pDoc->toString();
		std::cout << "\n\n";
		XmlDocument& desc = pDoc->elements("dependency");
		std::vector<sPtr> eles;
		eles = pDoc->element("package").select();
		std::cout << eles[0]->children()[0]->value() << std::endl;
		pkg.fullName = eles[0]->children()[0]->value();
		eles = pDoc->element("timeStamp").select();
		std::cout << eles[0]->children()[0]->value() << std::endl;
		pkg.timeString = eles[0]->children()[0]->value();
		eles = pDoc->elements("dependency").select();
		for (auto x : eles) {
			pkg.depPkgs.push_back(x->children()[0]->value());
			std::cout << x->children()[0]->value() << std::endl;
		}

		string name = pkg.fullName;
		name.erase(name.size() - 21, name.size() - 1);
		std::cout << "\n" << name << std::endl;
		pkg.shortName = name;
		FileMgr fm2("repository\\" + pkg.fullName);
		fm2.addPattern("*.h");
		fm2.addPattern("*.cpp");
		fm2.search();
		std::vector<std::string> ds2 = fm2.getStores();
		for (auto fs2 : ds2) {
			pkg.files.push_back(fs2);
			std::cout << fs2 << std::endl;
		}
		_allPkgs.push_back(pkg);
	}

	this->latestPkg();
}

// ---< update latest packages --->
void ServerRep::latestPkg() {
	for (auto x : _allPkgs)
	{
		bool push = true;
		size_t i = 0;
		std::list<Package>::iterator my_Itr;
		for (my_Itr = _latestPkgs.begin(); my_Itr != _latestPkgs.end(); ++my_Itr)
		{
			if (x.shortName == my_Itr->shortName)
				if (x.timeString > my_Itr->timeString)
				{
					_latestPkgs.erase(my_Itr);
					break;
				}
				else
					push = false;
		}
		if (push) _latestPkgs.push_back(x);
	}
}

//// ---< as name --->

void ServerRep::removePkgFromLatest(string shName) {
	std::list<Package>::iterator my_Itr;
	for (my_Itr = _latestPkgs.begin(); my_Itr != _latestPkgs.end(); ++my_Itr) {
		if (my_Itr->shortName == shName)
		{
			_latestPkgs.erase(my_Itr);
			return;
		}
	}

}

// ---< name -> short package name --->

string ServerRep::getShortPkgNameFromFileName(string fileName) {
	string name = fileName;
	name.erase(name.find_last_of("."), name.size() - 1);
	if (name.find_last_of("\\") < name.size())
		name.erase(0, name.find_last_of("\\") + 1);
	return getShortPkgNameFromPkgName(name);
}
// ---< name -> short package name --->

string ServerRep::getShortPkgNameFromPkgName(string pkgName) {
	string name = pkgName;
	name.erase(name.size() - 21, name.size() - 1);
	//std::cout << "\n" << name << std::endl;
	return name;
}
// ---< name -> short package name --->

string ServerRep::getFileNameFromFull(string fileName) {
	string name = fileName;
	if (fileName.find_last_of("\\") < name.size())
		name.erase(0, name.find_last_of("\\") + 1);
	return name;
}
#ifdef TEST_SERVERREP
int main(int argc, char* argv[]) {
	StringHelper *ut = new StringHelper();
	
	CreateDirectory("repository\\dir1", NULL);
	ServerRep server("repository");
	vector<string> depPkgs;
	depPkgs.push_back("dependent_f1");
	depPkgs.push_back("dependent_f2");

	time_t timev(0);
	time(&timev);
	//server.createMetadateXML("repository", "pkg1", depPkgs, timev, "2016-01-01-01-23-45", "client1");
	cout << server.timeConverter(timev) << endl;

	server.initRep();
	cout << "\n\n" << endl;

	//rename("repository\\oldname", "repository\\newname");
	
	//ut->Title("Testing XmlParser", '=');
	//std::cout << "\n";
	//std::string src = "repository/Test_1_(time)/Test_1.xml";
	//XmlParser parser(src);
	//parser.verbose(); //false to not display details
	//ut->title("Compressed form of formatted XML:");
	//std::cout << "\n" << src << "\n";
	//ut->title("Parsing compressed XML:");
	//XmlDocument* pDoc = parser.buildDocument();
	//ut->title("Resulting XML Parse Tree:");
	//std::cout << "\n" << pDoc->toString();
	//std::cout << "\n\n";

	//pDoc->save("repository/xmlOutput.xml");

	return 0;
}

#endif