#include <xml/Attribute.hpp>

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
