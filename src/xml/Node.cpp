#include <xml/Node.h>

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
		SetChildren( other.children );
	}
	
	Node::Node( const std::string& theName, bool isSelfClosing )
	   : name( theName ),
	     attributes(),
	     children(),
	     flag( isSelfClosing ? SelfClosing : None )
	{
	}
	
	Node::Node( const std::string& theName, const AttributeContainer& theAttributes, bool isSelfClosing )
	   : name( theName ),
	     attributes( theAttributes ),
	     children(),
	     flag( isSelfClosing ? SelfClosing : None )
	{
	}
	
	Node::Node( const std::string& theName, const AttributeContainer& theAttributes, const NodeContainer& theChildren )
	   : name( theName ),
	     attributes( theAttributes ),
	     children(),
	     flag( None )
	{
		SetChildren( theChildren );
	}
	
	Node& Node::operator = ( const Node& other )
	{
		SetName( other.name );
		SetAttributes( other.attributes );
		SetChildren( other.children );
		flag = other.flag;
		text = other.text;
		
		return ( * this );
	}
	
	bool Node::IsSelfClosing() const
	{
		return flag == SelfClosing;
	}
	
	void Node::IsSelfClosing( bool isSelfClosing )
	{
		if ( isSelfClosing != IsSelfClosing() )
		{
			flag = isSelfClosing ? SelfClosing : None;
			children.clear();
		}
	}
	
	bool Node::IsCharacterData() const
	{
		return flag == CharacterData;
	}
	
	void Node::IsCharacterData( bool isCharData )
	{
		if ( isCharData != IsCharacterData() )
		{
			flag = isCharData ? CharacterData : None;
			text.clear();
		}
	}
	
	bool Node::IsTextNode() const
	{
		return flag == TextNode;
	}
	
	void Node::IsTextNode( bool isTextNode )
	{
		if ( isTextNode != IsTextNode() )
		{
			flag = isTextNode ? TextNode : None;
			text.clear();
		}
	}
	
	std::string Node::GetText() const
	{
		return text;
	}
	
	void Node::SetText( const std::string theText )
	{
		if ( !IsTextValid( theText ) )
		{
			return;
		}
		
		text = theText;
	}
	
	bool Node::IsTextValid( const std::string& theText ) const
	{
		if ( flag != CharacterData )
		{
			return true;
		}
		
		if ( theText.find( "]]>" ) != std::string::npos )
		{
			return false;
		}
		
		return true;
	}
	
	std::string Node::GetName() const
	{
		return name;
	}
	
	void Node::SetName( const std::string& theName )
	{
		if ( !IsNameValid( theName ) )
		{
			return;
		}
		
		name = theName;
	}
	
	bool Node::IsNameValid( const std::string& theName )
	{
		static std::string invalidStartingCharacters = "-.0123456789";
		static std::string invalidCharacters = "!\"#$%&'()*+,/;<=>?@[\\]^`{|}~ ";
		
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
	
	size_t Node::GetAttributeAmount() const
	{
		return attributes.size();
	}
	
	const Node::AttributeContainer& Node::GetAttributes() const
	{
		return attributes;
	}
	
	void Node::SetAttributes( const AttributeContainer& theAttributes )
	{
		attributes = theAttributes;
	}
	
	Attribute& Node::GetAttribute( size_t index )
	{
		if ( index >= GetAttributeAmount() )
		{
			return dummyAttribute;
		}
		
		return attributes[ index ];
	}
	
	const Attribute& Node::GetAttribute( size_t index ) const
	{
		if ( index >= GetAttributeAmount() )
		{
			return dummyAttribute;
		}
		
		return attributes[ index ];
	}
	
	void Node::SetAttribute( size_t index, const Attribute& attribute )
	{
		if ( index >= GetAttributeAmount() )
		{
			return;
		}
		
		attributes[ index ] = attribute;
	}
	
	void Node::AddAttribute( const std::string& name, const std::string& value )
	{
		AddAttribute( Attribute( name, value ) );
	}
	
	void Node::AddAttribute( const Attribute& attribute )
	{
		attributes.push_back( attribute );
	}
	
	void Node::AddAttribute( size_t index, const Attribute& attribute )
	{
		if ( index >= GetAttributeAmount() )
		{
			AddAttribute( attribute );
			return;
		}
		
		attributes.insert( attributes.begin() + index, attribute );
	}
	
	void Node::RemoveAttribute( size_t index )
	{
		if ( index >= GetAttributeAmount() )
		{
			return;
		}
		
		attributes.erase( attributes.begin() + index );
	}
	
	size_t Node::GetChildAmount() const
	{
		return children.size();
	}
	
	const Node::NodeContainer& Node::GetChildren() const
	{
		return children;
	}
	
	void Node::SetChildren( const NodeContainer& theChildren )
	{
		children.clear();
		for ( auto it = theChildren.begin(); it != theChildren.end(); ++it )
		{
			Node* child = new Node();
			( * child ) = ( * ( * it ) );
			children.push_back( NodePointer( child ) );
		}
	}
	
	Node& Node::GetChild( size_t index )
	{
		if ( index >= GetChildAmount() )
		{
			return dummyNode;
		}
		
		return ( * children[ index ] );
	}
	
	const Node& Node::GetChild( size_t index ) const
	{
		if ( index >= GetChildAmount() )
		{
			return dummyNode;
		}
		
		return ( * children[ index ] );
	}
	
	void Node::SetChild( size_t index, const Node& child )
	{
		if ( index >= GetChildAmount() )
		{
			return;
		}
		
		( * children[ index ] ) = child;
	}
	
	void Node::AddChild( const Node& child )
	{
		children.push_back( NodePointer( new Node( child ) ) );
	}
	
	void Node::AddChild( size_t index, const Node& child )
	{
		if ( index >= GetChildAmount() )
		{
			AddChild( child );
			return;
		}
		
		children.insert( children.begin() + index, NodePointer( new Node( child ) ) );
	}
	
	void Node::RemoveChild( size_t index )
	{
		if ( index >= GetChildAmount() )
		{
			return;
		}
		
		children.erase( children.begin() + index );
	}
	
	std::string Node::GetAsString() const
	{
		std::string toReturn;
		if ( IsTextNode() )
		{
			toReturn = text;
		}
		else if ( IsCharacterData() )
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
				toReturn += it->GetAsString();
			}
			
			if ( IsSelfClosing() )
			{
				toReturn += " />";
			}
			else
			{
				toReturn += ">";
				
				for ( auto it = children.begin(); it != children.end(); ++it )
				{
					toReturn += "\r\n";
					toReturn += ( * it )->GetAsString();
				}
				
				if ( GetChildAmount() != 0 )
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
