#ifndef XML_ATTRIBUTE_H
#define XML_ATTRIBUTE_H

#include <string>

namespace xml
{
	class Attribute
	{
		public:
			Attribute();
			Attribute( const std::string& theName, const std::string& theValue );
			
			std::string GetName() const;
			void SetName( const std::string& theName );
			
			std::string GetValue() const;
			void SetValue( const std::string& theValue );
			
			std::string GetAsString() const;
		
		private:
			std::string name;
			std::string value;
	};
}

#endif // XML_ATTRIBUTE_H
