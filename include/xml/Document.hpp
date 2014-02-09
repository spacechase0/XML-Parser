#ifndef XML_DOCUMENT_HPP
#define XML_DOCUMENT_HPP

#include <xml/Attribute.hpp>
#include <xml/Node.hpp>
#include <string>

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

#endif // XML_DOCUMENT_HPP
