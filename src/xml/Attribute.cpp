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

#include <xml/Attribute.h>

namespace xml
{
	Attribute::Attribute()
	   : name( "" ),
	     value( "" )
	{
	}
	
	Attribute::Attribute( const std::string& theName, const std::string& theValue )
	   : name( theName ),
	     value( theValue )
	{
	}
	
	void Attribute::setName( const std::string& theName )
	{
		name = theName;
	}
	
	std::string Attribute::getName() const
	{
		return name;
	}
	
	void Attribute::setValue( const std::string& theValue )
	{
		value = theValue;
	}
	
	std::string Attribute::getValue() const
	{
		return value;
	}
	
	std::string Attribute::getString() const
	{
		std::string toReturn = "";
		toReturn += name;
		toReturn += "=";
		
		std::string theValue = value;
		for ( size_t i = 0; i < theValue.length(); ++i )
		{
			if ( theValue[ i ] == '\"' )
			{
				theValue.replace( i, 1, "\\\"" );
				++i;
			}
		}
		toReturn += "\"" + theValue + "\"";
		
		return toReturn;
	}
}
