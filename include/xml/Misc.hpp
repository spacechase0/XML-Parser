#ifndef XML_MISC_HPP
#define XML_MISC_HPP

#include <xml/Node.hpp>
#include <xml/Attribute.hpp>

namespace xml
{
	const xml::Node& getNode( const xml::Node& node, const std::string& name );
	xml::Attribute getAttribute( const xml::Node& node, const std::string& name );
}

#endif // XML_MISC_HPP
