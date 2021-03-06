#ifndef XMLDOCUMENT_H
#define XMLDOCUMENT_H
///////////////////////////////////////////////////////////////////
// XmlDocument.h - a container of XmlElement nodes               //
// Ver 2.3                                                       //
// Application: Help for CSE687 Pr#2, Spring 2015                //
// Platform:    Dell XPS 2720, Win 8.1 Pro, Visual Studio 2013   //
//  Source:     Jim Fawcett, CSE687 - Object Oriented Design,    //
//              Spring 2016                                      //
//  Author:		Cunyang Shui, Computer Engineering               //
//				cushui@syr.edu		    v                        //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package is intended to help students in CSE687 - Object Oriented Design
* get started with Project #2 - XML Document Model.  It uses C++11 constructs,
* most noteably std::shared_ptr.  The XML Document Model is essentially
* a program-friendly wrapper around an Abstract Syntax Tree (AST) used to
* contain the results of parsing XML markup.
*
* Abstract Syntax Trees, defined in this package, are unordered trees with
* two types of nodes:
*   Terminal nodes     - nodes with no children
*   Non-Terminal nodes - nodes which may have a finite number of children
* They are often used to contain the results of parsing some language.
*
* The elements defined in the companion package, XmlElement, will be used in
* the AST defined in this package.  They are:
*   AbstractXmlElement - base for all the XML Element types
*   DocElement         - XML element with children designed to hold prologue, Xml root, and epilogue
*   TaggedElement      - XML element with tag, attributes, child elements
*   TextElement        - XML element with only text, no markup
*   CommentElement     - XML element with comment markup and text
*   ProcInstrElement   - XML element with markup and attributes but no children
*   XmlDeclarElement   - XML declaration element with attributes but no children
*
* Required Files:
* ---------------
*   - XmlDocument.h, XmlDocument.cpp,
*     XmlElement.h, XmlElement.cpp
*
* Build Process:
* --------------
*   devenv Project4.sln /debug rebuild
*
* Maintenance History:
* --------------------
* ver 2.3 : 3 Mar 16
* - delete some extend functions unused in Project4
* ver 2.2 : 01 Jun 2015
* - added building document from XML file using XmlParser in constructor
* - added test to teststub
* ver 2.1 : 07 Mar 2015
* - added an XmlDocument method DFS(...) to implement Depth First Search.
*   This isn't really needed, but was added to illustrate the answer to
*   a Midterm question.
* ver 2.0 : 04 Mar 2015
* This version can programmatically create, edit, and save an XML document.
* It is not yet able to parse an XML string or file.
* - deleted copy constructor and assignment operator
* - defined move constructor and move assignment
* - defined search methods element, elements, descendents, select, find
* - defined access methods docElement() and xmlRoot()
* - defined toString()
* ver 1.2 : 21 Feb 2015
* - modified these comments
* ver 1.1 : 14 Feb 2015
* - minor changes to comments, method arguments
* Ver 1.0 : 11 Feb 2015
* - first release

*/

#include <memory>
#include <string>
#include "XmlElement.h"

namespace XmlProcessing
{
	///////////////////////////////////////////////////////////////////////////
	// XmlDocument class

	class XmlDocument
	{
	public:
		using sPtr = std::shared_ptr<AbstractXmlElement>;
		enum sourceType { file, str };

		// construction and assignment

		XmlDocument(sPtr pRoot = nullptr) : pDocElement_(pRoot) {}
		XmlDocument(const std::string& src, sourceType srcType = str);
		XmlDocument(const XmlDocument& doc) = delete;
		XmlDocument(XmlDocument&& doc);
		XmlDocument& operator=(const XmlDocument& doc) = delete;
		XmlDocument& operator=(XmlDocument&& doc);

		// access to docElement and XML root

		std::shared_ptr<AbstractXmlElement>& docElement() { return pDocElement_; }
		std::shared_ptr<AbstractXmlElement> xmlRoot();
		bool xmlRoot(sPtr pRoot);

		// queries return XmlDocument references so they can be chained, e.g., doc.element("foobar").descendents();

		XmlDocument& element(const std::string& tag);           // found_[0] contains first element (DFS order) with tag
		XmlDocument& elements(const std::string& tag);          // found_ contains all children of first element with tag
		XmlDocument& descendents(const std::string& tag = "");  // found_ contains descendents of prior found_[0]
		std::vector<sPtr> select();                             // returns found_.  Uses std::move(found_) to clear found_
		bool find(const std::string& tag, sPtr pElem, bool findall = true);

		size_t size();
		std::string toString();
		void save(std::string fileName);
	private:
		sPtr pDocElement_;         // AST that holds procInstr, comments, XML root, and more comments
		std::vector<sPtr> found_;  // query results
	};
}
#endif
