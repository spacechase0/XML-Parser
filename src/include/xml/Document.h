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
			
			std::string GetVersion() const;
			void SetVersion( const std::string& theVersion );
			
			std::string GetEncoding() const;
			void SetEncoding( const std::string& theEncoding );
			
			Node& GetRootNode();
			const Node& GetRootNode() const;
			void SetRootNode( const Node& theRootNode );
			
			typedef std::tuple< bool, std::string, size_t > ParseStatus; // success, message, endLocation
			ParseStatus Parse( const std::string& theXml );
			std::string GetAsString() const;
		
		private:
			std::string version;
			std::string encoding;
			Node rootNode;
			
			ParseStatus ParseHeader( const std::string& str );
			typedef std::tuple< Node, ParseStatus > NodeStatus;
			NodeStatus ParseNode( const std::string& str, size_t start );
			typedef std::tuple< Attribute, ParseStatus > AttributeStatus; // name, value, end, status
			AttributeStatus ParseAttribute( const std::string& str, size_t start );
			
			bool IsWhitespace( char c ) const;
	};
}

#endif // XML_DOCUMENT_H
