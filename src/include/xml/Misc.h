#ifndef XML_MISC_H
#define XML_MISC_H

#include <xml/Node.h>
#include <xml/Attribute.h>

namespace xml
{
	const xml::Node& GetNode( const xml::Node& node, const std::string& name );
	xml::Attribute GetAttribute( const xml::Node& node, const std::string& name );
}

#endif // XML_MISC_H
