#include <xml/Xml.h>
#include <iostream>
#include <fstream>
#include <string>

int main()
{
	/*
	xml::Node node( "node", true );
	std::cout << node.GetAsString() << std::endl
	          << std::endl;
	
	node.AddAttribute( xml::Attribute( "key", "value" ) );
	std::cout << node.GetAsString() << std::endl
	          << std::endl;
	
	node.IsSelfClosing( false );
	std::cout << node.GetAsString() << std::endl
	          << std::endl;
	
	xml::Node text;
	text.IsTextNode( true );
	text.SetText( "Hello World!" );
	node.AddChild( text );
	std::cout << node.GetAsString() << std::endl
	          << std::endl;
	
	node.GetChild( 0 ).IsCharacterData( true );
	std::cout << node.GetAsString() << std::endl
	          << std::endl;
	
	xml::Node child( "child", true );
	child.AddAttribute( xml::Attribute( "key", "value" ) );
	node.AddChild( child );
	std::cout << node.GetAsString() << std::endl
	          << std::endl;
	//*/
	std::fstream file( "test.xml", std::fstream::in );
	std::string contents;
	while ( true )
	{
		std::string line;
		std::getline( file, line );
		if ( !file.eof() )
		{
			contents += line;
		}
		else
		{
			break;
		}
	}
	file.close();
	
	xml::Document document;
	auto status = document.Parse( contents );
	std::cout << std::get< 0 >( status ) << " " << std::get< 1 >( status ) << std::endl
	          << document.GetVersion() << " " << document.GetEncoding() << std::endl
	          << document.GetRootNode().GetAsString() << std::endl;
	
	file.open( "out.xml", std::fstream::out | std::fstream::trunc );
	file << document.GetAsString();
	file.close();
	
	std::string dummyString;
	std::getline( std::cin, dummyString );
}
