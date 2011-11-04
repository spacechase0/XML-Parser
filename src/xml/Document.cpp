////////////////////////////////////////////////////////////
//
// XML Parser
// Copyright (C) 2011 Chase Warrington (staff@spacechase0.com)
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
		Parse( theXml );
	}
	
	std::string Document::GetVersion() const
	{
		return version;
	}
	
	void Document::SetVersion( const std::string& theVersion )
	{
		version = theVersion;
	}
	
	std::string Document::GetEncoding() const
	{
		return encoding;
	}
	
	void Document::SetEncoding( const std::string& theEncoding )
	{
		encoding = theEncoding;
	}
	
	Node& Document::GetRootNode()
	{
		return rootNode;
	}
	
	const Node& Document::GetRootNode() const
	{
		return rootNode;
	}
	
	void Document::SetRootNode( const Node& theRootNode )
	{
		rootNode = theRootNode;
	}
	
	Document::ParseStatus Document::Parse( const std::string& theXml )
	{
		auto headerStatus = ParseHeader( theXml );
		if ( !std::get< 0 >( headerStatus ) )
		{
			return headerStatus;
		}
		
		auto nodeStatus = ParseNode( theXml, std::get< 2 >( headerStatus ) );
		if ( !std::get< 0 >( std::get< 1 >( nodeStatus ) ) )
		{
			return std::get< 1 >( nodeStatus );
		}
		rootNode = std::get< 0 >( nodeStatus );
		
		return ParseStatus( true, "" );
	}
	
	std::string Document::GetAsString() const
	{
		std::string toReturn;
		toReturn += "<?xml version=\"" + version + "\" encoding=\"" + encoding + "\" ?>";
		toReturn += rootNode.GetAsString();
		return toReturn;
	}
	
	
	Document::ParseStatus Document::ParseHeader( const std::string& str )
	{
		bool started = false;
		std::string snippet = "";
		
		bool parsedName = false;
		
		std::string name = "";
		std::string value = "";
		size_t i = 0;
		for ( ; i < str.length(); ++i )
		{
			if ( IsWhitespace( str[ i ] ) and !started )
			{
				continue;
			}
			else if ( !started )
			{
				if ( str[ i ] != '<' )
				{
					return ParseStatus( false, "Extra characters found before XML header.", std::string::npos );
				}
				started = true;
				snippet = str[ i ];
			}
			else
			{
				snippet += str[ i ];
				if ( !parsedName )
				{
					if ( IsWhitespace( str[ i ] ) )
					{
						snippet = snippet.substr( 0, snippet.length() - 1 );
					}
					
					if ( snippet.length() == std::string( "<?xml" ).length() )
					{
						parsedName = true;
						snippet = "";
						if ( !IsWhitespace( str[ i + 1 ] ) )
						{
							return ParseStatus( false, "Unknown element before XML header.", std::string::npos );
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
					
					if ( IsWhitespace( str[ i ] ) )
					{
						continue;
					}
					
					auto status = ParseAttribute( str, i );
					auto parseStatus = std::get< 1 >( status );
					if ( !std::get< 0 >( parseStatus ) )
					{
						return parseStatus;
					}
					else
					{
						Attribute attribute = std::get< 0 >( status );
						if ( attribute.GetName() == "version" )
						{
							version = attribute.GetValue();
						}
						else if ( attribute.GetName() == "encoding" )
						{
							encoding = attribute.GetValue();
						}
						i = std::get< 2 >( parseStatus );
					}
				}
			}
		}
		
		return ParseStatus( true, "", i );
	}
	
	Document::NodeStatus Document::ParseNode( const std::string& str, size_t start )
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
			ParseAttributes,
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
			if ( IsWhitespace( str[ i ] ) and stage == NotStarted )
			{
				continue;
			}
			
			if ( stage == NotStarted and ( str[ i ] == '<' and str.substr( i, 2 ) != "<!" ) )
			{
				stage = ParsingNodeStart;
				for ( size_t ii = i + 1; ii < str.length(); ++ii )
				{
					//if ( IsWhitespace( str[ ii ] ) and nameStart == std::string::npos )
					if ( IsWhitespace( str[ ii ] ) or str[ ii ] == '>' )
					{
						break;
					}
					
					std::string tmpString = "";
					tmpString += str[ ii ];
					if ( !Node::IsNameValid( tmpString ) )
					{
						//std::cout << str.substr( i, 10 ) << std::endl;
						return NodeStatus( Node(), ParseStatus( false, "Invalid name for node.", std::string::npos ) );
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
					return NodeStatus( Node(), ParseStatus( false, "Could not find node name.", std::string::npos ) );
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
							return NodeStatus( Node(), ParseStatus( false, "Found node end without start.", std::string::npos ) );
						}
						else if ( str[ i ] == '!' )
						{
							#warning TO DO: Implement checking for comments
							#warning TO DO: Put PROPER CData implementation
							//return NodeStatus( Node(), ParseStatus( false, "CData!", std::string::npos ) );
							std::string text = str.substr( i + 8, str.find( "]]>", i ) - ( i + 8 ) );
							Node tmp;
							tmp.IsCharacterData( true );
							tmp.SetText( text );
							//std::cout << text << std::endl << std::endl << std::endl;
							return NodeStatus( tmp, ParseStatus( true, "cdstring", str.find( "]]>", i ) + 3 ) );
						}
					}
					
					if ( !IsWhitespace( str[ i ] ) and str[ i ] != '>' )
					{
						name += str[ i ];
					}
					else
					{
						node.SetName( name );
						if ( str[ i ] != '>' )
						{
							startStage = ParseAttributes;
						}
						else
						{
							--i;
							startStage = ParseEnd;
						}
					}
				}
				else if ( startStage == ParseAttributes )
				{
					if ( str[ i ] == '>' or str.substr( i, 2 ) == "/>" )
					{
						startStage = ParseEnd;
						--i;
						continue;
					}
					
					if ( IsWhitespace( str[ i ] ) )
					{
						continue;
					}
					
					auto status = ParseAttribute( str, i );
					auto parseStatus = std::get< 1 >( status );
					if ( !std::get< 0 >( parseStatus ) )
					{
						return NodeStatus( Node(), parseStatus );
					}
					else
					{
						node.AddAttribute( std::get< 0 >( status ) );
						i = std::get< 2 >( parseStatus );
					}
				}
				else if ( startStage == ParseEnd )
				{
					if ( IsWhitespace( str[ i ] ) )
					{
						continue;
					}
					
					if ( str.substr( i, 2 ) == "/>" )
					{
						node.IsSelfClosing( true );
						return NodeStatus( node, ParseStatus( true, "", i + 1 ) );
					}
					else if ( str[ i ] == '>' )
					{
						stage = ParsingContents;
					}
					else
					{
						return NodeStatus( node, ParseStatus( false, "This should never happen. (1)", std::string::npos ) );
					}
				}
			}
			else if ( stage == ParsingContents )
			{
				if ( contentStage == LookingForNodes and IsWhitespace( str[ i ] ) )
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
					auto nodeStatus = ParseNode( str, i );
					auto status = std::get< 1 >( nodeStatus );
					if ( !std::get< 0 >( status ) )
					{
						return nodeStatus;
					}
					node.AddChild( std::get< 0 >( nodeStatus ) );
					i = std::get< 2 >( status );
					
					contentStage = LookingForNodes;
				}
				else
				{
					if ( str[ i ] == '<' or ( text.length() != 0 and IsWhitespace( text[ text.length() - 1 ] ) and IsWhitespace( str[ i ] ) ) )
					{
						Node textNode;
						textNode.IsTextNode( true );
						textNode.SetText( text );
						
						node.AddChild( textNode );
						
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
						//std::cout << "Hold it! " << node.GetName() << std::endl;
						return NodeStatus( node, ParseStatus( true, "Found something bad", i ) );
					}
				}
			}
			else if ( stage == ParsingNodeEnd )
			{
				if ( IsWhitespace( str[ i ] ) )
				{
					continue;
				}
				text += str[ i ];
				
				if ( text[ 0 ] != '<' )
				{
					return NodeStatus( node, ParseStatus( false, "This should never happen. (2)", std::string::npos ) );
				}
				else if ( text.length() >= 2 and text[ 1 ] != '/' )
				{
					return NodeStatus( node, ParseStatus( false, "This should never happen. (3)", std::string::npos ) );
				}
				
				if ( str[ i ] == '>' )
				{
					if ( text.length() < 4 )
					{
						return NodeStatus( node, ParseStatus( false, "Ending tag not long enough.", std::string::npos ) );
					}
					
					if ( text.substr( 2, text.length() - 3 ) != node.GetName() )
					{
						return NodeStatus( node, ParseStatus( false, "Non-matching ending tag.", std::string::npos ) );
					}
					
					return NodeStatus( node, ParseStatus( true, "", i ) );
				}
			}
		}
		return NodeStatus( node, ParseStatus( true, "ParseNode not completely implemented.", std::string::npos ) );
	}
	
	Document::AttributeStatus Document::ParseAttribute( const std::string& str, size_t start )
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
				if ( str[ i ] != '=' and !IsWhitespace( str[ i ] ) )
				{
					name += str[ i ];
				}
				else if ( !IsWhitespace( str[ i ] ) )
				{
					size_t found = std::string::npos;
					for ( size_t ii = i + 1; ii < str.length(); ++ii )
					{
						if ( !IsWhitespace( str[ ii ] ) and str[ ii ] != '\"' )
						{
							return AttributeStatus( Attribute(), ParseStatus( false, "Invalid characters as attribute value.", std::string::npos ) );
						}
						else if ( str[ ii ] == '\"' )
						{
							found = ii;
							break;
						}
					}
					if ( found == std::string::npos )
					{
						return AttributeStatus( Attribute(), ParseStatus( false, "Could not find attribute value.", std::string::npos ) );
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
					return AttributeStatus( Attribute( name, value ), ParseStatus( true, "", i ) );
				}
				else
				{
					value += str[ i ];
				}
			}
		}
		
		return AttributeStatus( Attribute(), ParseStatus( false, "Reached end of string.", std::string::npos ) );
	}
	
	bool Document::IsWhitespace( char c ) const
	{
		if ( isspace( c ) or c == '\t' or c == '\n' or c == '\r' )
		{
			return true;
		}
		
		return false;
	}
}
