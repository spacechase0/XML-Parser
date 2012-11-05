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

#ifndef XML_NODE_HPP
#define XML_NODE_HPP

#include <xml/Attribute.hpp>
#include <memory>
#include <string>
#include <vector>

namespace xml
{
	class Node
	{
		public:
			// Typedefs
			typedef std::shared_ptr< Node > NodePointer;
			
			typedef std::vector< Attribute > AttributeContainer;
			typedef std::vector< NodePointer > NodeContainer;
			
			// Constructors and assignment operator
			Node();
			Node( const Node& other );
			Node( const std::string& theName, bool theSelfClosing = false );
			Node( const std::string& theName, const std::vector< Attribute >& theAttributes, bool isSelfClosing = false );
			Node( const std::string& theName, const std::vector< Attribute >& theAttributes, const NodeContainer& theChildren );
			Node& operator = ( const Node& other );
			
			void setSelfClosing( bool theSelfClosing );
			bool isSelfClosing() const;
			
			void setCharacterData( bool theCharData );
			static bool isValidCharacterData( const std::string& theText );
			bool isCharacterData() const;
			
			void setTextNode( bool theTextNode );
			bool isTextNode() const;
			
			void setText( const std::string theText );
			std::string getText() const;
			
			void setName( const std::string& theName );
			static bool isNameValid( const std::string& theName );
			std::string getName() const;
			
			void setAttributes( const std::vector< Attribute >& theAttributes );
			const std::vector< Attribute >& getAttributes() const;
			
			void setChildren( const std::vector< std::shared_ptr< Node > >& theChildren );
			const std::vector< std::shared_ptr< Node > >& getChildren() const;
			
			std::string getString() const;
		
		private:
			enum Flag
			{
				None          = 0,
				SelfClosing   = 1,
				CharacterData = 2,
				TextNode      = 3
			};
			
			std::string name;
			std::vector< Attribute > attributes;
			std::vector< std::shared_ptr< Node > > children;
			Flag flag;
			std::string text;
			
			static Attribute dummyAttribute;
			static Node dummyNode;
	};
}

#endif // XML_NODE_HPP
