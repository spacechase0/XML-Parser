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
	
	std::string Attribute::GetName() const
	{
		return name;
	}
	
	void Attribute::SetName( const std::string& theName )
	{
		name = theName;
	}
	
	std::string Attribute::GetValue() const
	{
		return value;
	}
	
	void Attribute::SetValue( const std::string& theValue )
	{
		value = theValue;
	}
	
	std::string Attribute::GetAsString() const
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
