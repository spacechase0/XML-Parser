////////////////////////////////////////////////////////////
//
// XML Parser
// Copyright (C) 2011 Chase Warrington (staff@spacechase0.com)
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
