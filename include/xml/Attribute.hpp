#ifndef XML_ATTRIBUTE_HPP
#define XML_ATTRIBUTE_HPP

#include <string>

namespace xml
{
	class Attribute
	{
		public:
			Attribute();
			Attribute( const std::string& theName, const std::string& theValue );
			
			void setName( const std::string& theName );
			std::string getName() const;
			
			void setValue( const std::string& theValue );
			std::string getValue() const;
			
			std::string getString() const;
		
		private:
			std::string name;
			std::string value;
	};
}

#endif // XML_ATTRIBUTE_HPP
