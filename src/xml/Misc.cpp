#include <xml/Misc.h>

namespace xml
{
	const xml::Node& GetNode( const xml::Node& node, const std::string& name )
	{
		auto sections = node.GetChildren();
		for ( auto it = sections.begin(); it != sections.end(); ++it )
		{
			if ( ( * it )->GetName() == name )
			{
				return ( * ( * it ) );
			}
		}
		
		return node;
	}
	
	xml::Attribute GetAttribute( const xml::Node& node, const std::string& name )
	{
		auto attributes = node.GetAttributes();
		for ( auto it = attributes.begin(); it != attributes.end(); ++it )
		{
			if ( it->GetName() == name )
			{
				return ( * it );
			}
		}
		
		return xml::Attribute();
	}
}
