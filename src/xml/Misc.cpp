#include <xml/Misc.hpp>

namespace xml
{
	const xml::Node& getNode( const xml::Node& node, const std::string& name )
	{
		auto sections = node.getChildren();
		for ( auto it = sections.begin(); it != sections.end(); ++it )
		{
			if ( ( * it )->getName() == name )
			{
				return ( * ( * it ) );
			}
		}
		
		return node;
	}
	
	xml::Attribute getAttribute( const xml::Node& node, const std::string& name )
	{
		auto attributes = node.getAttributes();
		for ( auto it = attributes.begin(); it != attributes.end(); ++it )
		{
			if ( it->getName() == name )
			{
				return ( * it );
			}
		}
		
		return xml::Attribute();
	}
}
