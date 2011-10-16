#ifndef XML_NODE_H
#define XML_NODE_H

#include <boost/shared_ptr.hpp>
#include <xml/Attribute.h>
#include <string>
#include <vector>

namespace xml
{
	class Node
	{
		public:
			// Typedefs
			typedef boost::shared_ptr< Node > NodePointer;
			
			typedef std::vector< Attribute > AttributeContainer;
			typedef std::vector< NodePointer > NodeContainer;
			
			// Constructors and assignment operator
			Node();
			Node( const Node& other );
			Node( const std::string& theName, bool isSelfClosing = false );
			Node( const std::string& theName, const AttributeContainer& theAttributes, bool isSelfClosing = false );
			Node( const std::string& theName, const AttributeContainer& theAttributes, const NodeContainer& theChildren );
			// I've heard of the copy constructor-assignment operator-destructor triangle thing,
			// but boost::shared_pointer will take care of the destructor work for us. :)
			Node& operator = ( const Node& other );
			
			// Flags
			bool IsSelfClosing() const;
			void IsSelfClosing( bool isSelfClosing );
			
			bool IsCharacterData() const;
			void IsCharacterData( bool isCharData );
			
			bool IsTextNode() const;
			void IsTextNode( bool isTextNode );
			
			std::string GetText() const;
			void SetText( const std::string theText );
			bool IsTextValid( const std::string& theText ) const;
			
			// Name
			std::string GetName() const;
			void SetName( const std::string& theName );
			static bool IsNameValid( const std::string& theName );
			
			// Attributes
			size_t GetAttributeAmount() const;
			const AttributeContainer& GetAttributes() const;
			void SetAttributes( const AttributeContainer& theAttributes );
			
			Attribute& GetAttribute( size_t index );
			const Attribute& GetAttribute( size_t index ) const;
			void SetAttribute( size_t index, const Attribute& attribute );
			void AddAttribute( const std::string& name, const std::string& value );
			void AddAttribute( const Attribute& attribute );
			void AddAttribute( size_t index, const Attribute& attribute );
			void RemoveAttribute( size_t index );
			
			// Children
			size_t GetChildAmount() const;
			const NodeContainer& GetChildren() const;
			void SetChildren( const NodeContainer& theChildren );
			
			Node& GetChild( size_t index );
			const Node& GetChild( size_t index ) const;
			void SetChild( size_t index, const Node& child );
			void AddChild( const Node& child );
			void AddChild( size_t index, const Node& child );
			void RemoveChild( size_t index );
			
			// Misc.
			std::string GetAsString() const;
		
		private:
			enum Flag
			{
				None          = 0,
				SelfClosing   = 1,
				CharacterData = 2,
				TextNode      = 3
			};
			
			std::string name;
			AttributeContainer attributes;
			NodeContainer children;
			Flag flag;
			std::string text;
			
			static Attribute dummyAttribute;
			static Node dummyNode;
	};
}

#endif // XML_NODE_H
