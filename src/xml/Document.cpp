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

#include <xml/Document.h>

namespace xml
{
	Document::Document()
	   : version( "1.0" ),
	     encoding( "UTF-8" ),
	     rootNode()
	{
	}
	
	Document::Document( const std::string& version, const Node& theRootNode )
	   : version( version ),
	     encoding( "UTF-8" ),
	     rootNode( theRootNode )
	{
	}
	
	Document::Document( const std::string& version, const std::string& encoding, const Node& theRootNode )
	   : version( version ),
	     encoding( encoding ),
	     rootNode( theRootNode )
	{
	}
	
	Document::Document( const std::string& theXml )
	   : version( "1.0" ),
	     encoding( "UTF-8" ),
	     rootNode()
	{
		parse( theXml );
	}
	
	void Document::setVersion( const std::string& theVersion )
	{
		version = theVersion;
	}
	
	std::string Document::getVersion() const
	{
		return version;
	}
	
	void Document::setEncoding( const std::string& theEncoding )
	{
		encoding = theEncoding;
	}
	
	std::string Document::getEncoding() const
	{
		return encoding;
	}
	
	void Document::setRootNode( const Node& theRootNode )
	{
		rootNode = theRootNode;
	}
	
	Node& Document::getRootNode()
	{
		return rootNode;
	}
	
	const Node& Document::getRootNode() const
	{
		return rootNode;
	}
	
	Document::ParseStatus Document::parse( const std::string& theXml )
	{
		ParseStatus headerStatus = parseHeader( theXml );
		if ( !headerStatus.success )
		{
			return headerStatus;
		}
		
		NodeStatus nodeStatus = parseNode( theXml, headerStatus.loc );
		if ( !nodeStatus.success )
		{
			ParseStatus status;
			status.success = nodeStatus.success;
			status.message = nodeStatus.message;
			status.loc = nodeStatus.loc;
			
			return status;
		}
		rootNode = nodeStatus.node;
		
		return ParseStatus{ true, "", 0 };
	}
	
	std::string Document::getString() const
	{
		std::string toReturn;
		toReturn += "<?xml version=\"" + version + "\" encoding=\"" + encoding + "\" ?>\n";
		toReturn += rootNode.getString();
		return toReturn;
	}
	
	
	Document::ParseStatus Document::parseHeader( const std::string& str )
	{
		bool started = false;
		std::string snippet = "";
		
		bool parsedName = false;
		
		std::string name = "";
		std::string value = "";
		size_t i = 0;
		for ( ; i < str.length(); ++i )
		{
			if ( isWhitespace( str[ i ] ) and !started )
			{
				continue;
			}
			else if ( !started )
			{
				if ( str[ i ] != '<' )
				{
					return ParseStatus{ false, "Extra characters found before XML header.", i };
				}
				started = true;
				snippet = str[ i ];
			}
			else
			{
				snippet += str[ i ];
				if ( !parsedName )
				{
					if ( isWhitespace( str[ i ] ) )
					{
						snippet = snippet.substr( 0, snippet.length() - 1 );
					}
					
					if ( snippet.length() == std::string( "<?xml" ).length() )
					{
						parsedName = true;
						snippet = "";
						if ( !isWhitespace( str[ i + 1 ] ) )
						{
							return ParseStatus{ false, "Unknown element before XML header.", i };
						}
						else
						{
							++i;
						}
					}
				}
				else
				{
					if ( str.substr( i, 2 ) == "?>" )
					{
						break;
					}
					
					if ( isWhitespace( str[ i ] ) )
					{
						continue;
					}
					
					AttributeStatus attrStatus = parseAttribute( str, i );
					if ( !attrStatus.success )
					{
						return ParseStatus{ attrStatus.success, attrStatus.message, attrStatus.loc };
					}
					else
					{
						Attribute attribute = attrStatus.attr;
						if ( attribute.getName() == "version" )
						{
							version = attribute.getValue();
						}
						else if ( attribute.getName() == "encoding" )
						{
							encoding = attribute.getValue();
						}
						i = attrStatus.loc;
					}
				}
			}
		}
		
		return ParseStatus{ true, "", i };
	}
	
	Document::NodeStatus Document::parseNode( const std::string& str, size_t start )
	{
		//if(start==0){return NodeStatus(Node(),ParseStatus(false,"Huh?",0));}
		std::string snippet = "";
		
		enum Stage
		{
			NotStarted,
			ParsingNodeStart,
			ParsingContents,
			ParsingNodeEnd
		} stage = NotStarted;
		enum StartStage
		{
			ParseName,
			parseAttributes,
			ParseEnd
		} startStage = ParseName;
		enum ContentStage
		{
			LookingForNodes,
			ParsingText,
			ParsingElement,
		} contentStage = LookingForNodes;
		
		std::string name = "";
		std::string text = "";
		size_t nameStart = std::string::npos;
		Node node;
		for ( size_t i = start; i < str.length(); ++i )
		{
			if ( isWhitespace( str[ i ] ) and stage == NotStarted )
			{
				continue;
			}
			
			if ( stage == NotStarted and ( str[ i ] == '<' and str.substr( i, 2 ) != "<!" ) )
			{
				stage = ParsingNodeStart;
				for ( size_t ii = i + 1; ii < str.length(); ++ii )
				{
					//if ( isWhitespace( str[ ii ] ) and nameStart == std::string::npos )
					if ( isWhitespace( str[ ii ] ) or str[ ii ] == '>' )
					{
						break;
					}
					
					std::string tmpString = "";
					tmpString += str[ ii ];
					if ( !Node::isNameValid( tmpString ) )
					{
						//std::cout << str.substr( i, 10 ) << std::endl;
						return NodeStatus{ false, "Invalid name for node.", ii, Node() };
					}
					else
					{
						nameStart = ii;
						i = nameStart - 1;
						break;
					}
				}
				if ( nameStart == std::string::npos )
				{
					return NodeStatus{ false, "Could not find node name.", i, Node() };
				}
				continue;
			}
			else if ( stage == NotStarted and str.substr( i, 2 ) == "<!" )
			{
				stage = ParsingNodeStart;
				nameStart = i-- + 1;
				continue;
			}
			else if ( stage == ParsingNodeStart )
			{
				if ( startStage == ParseName )
				{
					if ( i == nameStart )
					{
						if ( str[ i ] == '/' )
						{
							return NodeStatus{ false, "Found node end without start.", i, Node() };
						}
						else if ( str[ i ] == '!' )
						{
							#warning TO DO: Implement checking for comments
							#warning TO DO: Put PROPER CData implementation
							//return NodeStatus( Node(), ParseStatus( false, "CData!", std::string::npos ) );
							std::string text = str.substr( i + 8, str.find( "]]>", i ) - ( i + 8 ) );
							Node tmp;
							tmp.setCharacterData( true );
							tmp.setText( text );
							//std::cout << text << std::endl << std::endl << std::endl;
							return NodeStatus{ true, "", str.find( "]]>", i ) + 3, tmp };
						}
					}
					
					if ( !isWhitespace( str[ i ] ) and str[ i ] != '>' )
					{
						name += str[ i ];
					}
					else
					{
						node.setName( name );
						if ( str[ i ] != '>' )
						{
							startStage = parseAttributes;
						}
						else
						{
							--i;
							startStage = ParseEnd;
						}
					}
				}
				else if ( startStage == parseAttributes )
				{
					if ( str[ i ] == '>' or str.substr( i, 2 ) == "/>" )
					{
						startStage = ParseEnd;
						--i;
						continue;
					}
					
					if ( isWhitespace( str[ i ] ) )
					{
						continue;
					}
					
					AttributeStatus status = parseAttribute( str, i );
					if ( !status.success )
					{
						return NodeStatus{ status.success, status.message, status.loc, Node() };
					}
					else
					{
						std::vector< Attribute > attrs = node.getAttributes();
						attrs.push_back( status.attr );
						node.setAttributes( attrs );
						
						i = status.loc;
					}
				}
				else if ( startStage == ParseEnd )
				{
					if ( isWhitespace( str[ i ] ) )
					{
						continue;
					}
					
					if ( str.substr( i, 2 ) == "/>" )
					{
						node.setSelfClosing( true );
						return NodeStatus{ true, "", i + 1, node };
					}
					else if ( str[ i ] == '>' )
					{
						stage = ParsingContents;
					}
					else
					{
						return NodeStatus{ false, "This should never happen. (1)", i, node };
					}
				}
			}
			else if ( stage == ParsingContents )
			{
				if ( contentStage == LookingForNodes and isWhitespace( str[ i ] ) )
				{
					continue;
				}
				else if ( contentStage == LookingForNodes )
				{
					if ( str[ i ] == '<' and str.substr( i, 2 ) != "</" )
					{
						contentStage = ParsingElement;
					}
					else if ( str.substr( i, 2 ) == "</" )
					{
						stage = ParsingNodeEnd;
						--i;
						text = "";
						continue;
					}
					else
					{
						text = "";
						contentStage = ParsingText;
					}
				}
				
				if ( contentStage == ParsingElement )
				{
					NodeStatus status = parseNode( str, i );
					if ( !status.success )
					{
						return status;
					}
					{
						std::vector< std::shared_ptr< Node > > nodes = node.getChildren();
						nodes.push_back( std::shared_ptr< Node >( new Node( status.node ) ) );
						node.setChildren( nodes );
					}
					i = status.loc;
					
					contentStage = LookingForNodes;
				}
				else
				{
					if ( str[ i ] == '<' or ( text.length() != 0 and isWhitespace( text[ text.length() - 1 ] ) and isWhitespace( str[ i ] ) ) )
					{
						Node textNode;
						textNode.setTextNode( true );
						textNode.setText( text );
						
						{
							std::vector< std::shared_ptr< Node > > nodes = node.getChildren();
							nodes.push_back( std::shared_ptr< Node >( new Node( textNode ) ) );
							node.setChildren( nodes );
						}
						
						contentStage = LookingForNodes;
						if ( str[ i ] == '<' )
						{
							--i;
						}
						continue;
					}
					
					text += str[ i ];
					//std::cout << (int)str[i] << std::endl;
					if ( text.find( "/W" ) != std::string::npos )
					{
						//std::cout << "Hold it! " << node.getName() << std::endl;
						return NodeStatus{ true, "Found something bad", i, node };
					}
				}
			}
			else if ( stage == ParsingNodeEnd )
			{
				if ( isWhitespace( str[ i ] ) )
				{
					continue;
				}
				text += str[ i ];
				
				if ( text[ 0 ] != '<' )
				{
					return NodeStatus{ false, "This should never happen. (2)", i, node };
				}
				else if ( text.length() >= 2 and text[ 1 ] != '/' )
				{
					return NodeStatus{ false, "This should never happen. (3)", i, node };
				}
				
				if ( str[ i ] == '>' )
				{
					if ( text.length() < 4 )
					{
						return NodeStatus{ false, "Ending tag not long enough.", i, node };
					}
					
					if ( text.substr( 2, text.length() - 3 ) != node.getName() )
					{
						return NodeStatus{ false, "Non-matching ending tag.", i, node };
					}
					
					return NodeStatus{ true, "", i, node };
				}
			}
		}
		return NodeStatus{ true, "parseNode not completely implemented.", std::string::npos, node };
	}
	
	Document::AttributeStatus Document::parseAttribute( const std::string& str, size_t start )
	{
		bool inQuotes = false;
		bool escaped = false;
		
		std::string name = "";
		std::string value = "";
		for ( size_t i = start; i < str.length(); ++i )
		{
			if ( !inQuotes )
			{
				if ( str.substr( i, 2 ) == "?>" )
				{
					break;
				}
				if ( str[ i ] != '=' and !isWhitespace( str[ i ] ) )
				{
					name += str[ i ];
				}
				else if ( !isWhitespace( str[ i ] ) )
				{
					size_t found = std::string::npos;
					for ( size_t ii = i + 1; ii < str.length(); ++ii )
					{
						if ( !isWhitespace( str[ ii ] ) and str[ ii ] != '\"' )
						{
							return AttributeStatus{ false, "Invalid characters as attribute value.", i, Attribute() };
						}
						else if ( str[ ii ] == '\"' )
						{
							found = ii;
							break;
						}
					}
					if ( found == std::string::npos )
					{
						return AttributeStatus{ false, "Could not find attribute value.", i, Attribute() };
					}
					i = found;
					inQuotes = true;
					continue;
				}
			}
			else
			{
				if ( str[ i ] == '\\' and !escaped )
				{
					escaped = true;
				}
				else if ( str[ i ] == '\"' and !escaped )
				{
					inQuotes = false;
					return AttributeStatus{ true, "", i, Attribute( name, value ) };
				}
				else
				{
					value += str[ i ];
				}
			}
		}
		
		return AttributeStatus{ false, "Reached end of string.", std::string::npos, Attribute() };
	}
	
	bool Document::isWhitespace( char c ) const
	{
		if ( isspace( c ) or c == '\t' or c == '\n' or c == '\r' )
		{
			return true;
		}
		
		return false;
	}
}
