////////////////////////////////////////////////////////////
//
// XML Parser
// Copyright (C) 2012 Chase Warrington (staff@spacechase0.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef XML_DOCUMENT_H
#define XML_DOCUMENT_H

#include <xml/Attribute.h>
#include <xml/Node.h>
#include <string>
#include <tuple>

namespace xml
{
	class Document
	{
		public:
			Document();
			Document( const std::string& version, const Node& theRootNode );
			Document( const std::string& version, const std::string& encoding, const Node& theRootNode );
			Document( const std::string& theXml );
			
			void setVersion( const std::string& theVersion );
			std::string getVersion() const;
			
			void setEncoding( const std::string& theEncoding );
			std::string getEncoding() const;
			
			void setRootNode( const Node& theRootNode );
			Node& getRootNode();
			const Node& getRootNode() const;
			
			struct ParseStatus
			{
				public:
					bool success;
					std::string message;
					std::size_t loc;
			};
			ParseStatus parse( const std::string& theXml );
			
			std::string getString() const;
		
		private:
			std::string version;
			std::string encoding;
			Node rootNode;
			
			struct NodeStatus
			{
				public:
					bool success;
					std::string message;
					std::size_t loc;
					Node node;
			};
			
			struct AttributeStatus
			{
				public:
					bool success;
					std::string message;
					std::size_t loc;
					Attribute attr;
			};
			
			ParseStatus parseHeader( const std::string& str );
			NodeStatus parseNode( const std::string& str, size_t start );
			AttributeStatus parseAttribute( const std::string& str, size_t start );
			
			bool isWhitespace( char c ) const;
	};
}

#endif // XML_DOCUMENT_H
