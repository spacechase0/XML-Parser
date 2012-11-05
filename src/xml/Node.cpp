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

#include <xml/Node.hpp>

namespace xml
{
	Attribute Node::dummyAttribute;
	Node Node::dummyNode;
	
	Node::Node()
	   : name( "" ),
	     attributes(),
	     children(),
	     flag( None )
	{
	}
	
	Node::Node( const Node& other )
	   : name( other.name ),
	     attributes( other.attributes ),
	     children(),
	     flag( other.flag ),
	     text( other.text )
	{
		setChildren( other.children );
	}
	
	Node::Node( const std::string& theName, bool isSelfClosing )
	   : name( theName ),
	     attributes(),
	     children(),
	     flag( isSelfClosing ? SelfClosing : None )
	{
	}
	
	Node::Node( const std::string& theName, const std::vector< Attribute >& theAttributes, bool isSelfClosing )
	   : name( theName ),
	     attributes( theAttributes ),
	     children(),
	     flag( isSelfClosing ? SelfClosing : None )
	{
	}
	
	Node::Node( const std::string& theName, const std::vector< Attribute >& theAttributes, const std::vector< std::shared_ptr< Node > >& theChildren )
	   : name( theName ),
	     attributes( theAttributes ),
	     children(),
	     flag( None )
	{
		setChildren( theChildren );
	}
	
	Node& Node::operator = ( const Node& other )
	{
		setName( other.name );
		setAttributes( other.attributes );
		setChildren( other.children );
		flag = other.flag;
		text = other.text;
		
		return ( * this );
	}
	
	void Node::setSelfClosing( bool theSelfClosing )
	{
		if ( theSelfClosing != isSelfClosing() )
		{
			flag = theSelfClosing ? SelfClosing : None;
			children.clear();
		}
	}
	
	
	bool Node::isSelfClosing() const
	{
		return flag == SelfClosing;
	}
	
	void Node::setCharacterData( bool theCharData )
	{
		if ( theCharData != isCharacterData() )
		{
			flag = theCharData ? CharacterData : None;
			text.clear();
		}
	}
	
	bool Node::isCharacterData() const
	{
		return flag == CharacterData;
	}
	
	void Node::setTextNode( bool theTextNode )
	{
		if ( theTextNode != isTextNode() )
		{
			flag = theTextNode ? TextNode : None;
			text.clear();
		}
	}
	
	bool Node::isTextNode() const
	{
		return flag == TextNode;
	}
	
	void Node::setText( const std::string theText )
	{
		if ( !isValidCharacterData( theText ) )
		{
			return;
		}
		
		text = theText;
	}
	
	std::string Node::getText() const
	{
		return text;
	}
	
	bool Node::isValidCharacterData( const std::string& theText )
	{
		if ( theText.find( "]]>" ) != std::string::npos )
		{
			return false;
		}
		
		return true;
	}
	
	void Node::setName( const std::string& theName )
	{
		if ( !isNameValid( theName ) )
		{
			return;
		}
		
		name = theName;
	}
	
	std::string Node::getName() const
	{
		return name;
	}
	
	bool Node::isNameValid( const std::string& theName )
	{
		static const std::string invalidStartingCharacters = "-.0123456789";
		static const std::string invalidCharacters = "!\"#$%&'()*+,/;<=>?@[\\]^`{|}~ ";
		
		if ( theName.length() == 0 )
		{
			return false;
		}
		
		for ( size_t i = 0; i < invalidStartingCharacters.size(); ++i )
		{
			if ( theName[ 0 ] == invalidStartingCharacters[ i ] )
			{
				return false;
			}
		}
		
		for ( size_t i = 0; i < theName.size(); ++i )
		{
			for ( size_t ii = 0; ii < invalidCharacters.size(); ++ii )
			{
				if ( theName[ i ] == invalidCharacters[ ii ] )
				{
					return false;
				}
			}
		}
		
		return true;
	}
	
	void Node::setAttributes( const std::vector< Attribute >& theAttributes )
	{
		attributes = theAttributes;
	}
	
	const std::vector< Attribute >& Node::getAttributes() const
	{
		return attributes;
	}
	
	void Node::setChildren( const std::vector< std::shared_ptr< Node > >& theChildren )
	{
		children.clear();
		for ( auto it = theChildren.begin(); it != theChildren.end(); ++it )
		{
			Node* child = new Node();
			( * child ) = ( * ( * it ) );
			children.push_back( NodePointer( child ) );
		}
	}
	
	const std::vector< std::shared_ptr< Node > >& Node::getChildren() const
	{
		return children;
	}
	
	std::string Node::getString() const
	{
		std::string toReturn;
		if ( isTextNode() )
		{
			toReturn = text;
		}
		else if ( isCharacterData() )
		{
			toReturn =  "<![CDATA[";
			toReturn += text;
			toReturn += "]]>";
		}
		else
		{
			toReturn =  "<";
			toReturn += name;
			for ( auto it = attributes.begin(); it != attributes.end(); ++it )
			{
				toReturn += " ";
				toReturn += it->getString();
			}
			
			if ( isSelfClosing() )
			{
				toReturn += " />";
			}
			else
			{
				toReturn += ">";
				
				for ( auto it = children.begin(); it != children.end(); ++it )
				{
					toReturn += "\r\n";
					toReturn += ( * it )->getString();
				}
				
				if ( getChildren().size() != 0 )
				{
					toReturn += "\r\n";
				}
				
				toReturn += "</";
				toReturn += name;
				toReturn += ">";
			}
		}
		
		return toReturn;
	}
}
