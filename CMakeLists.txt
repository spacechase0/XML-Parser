# A lot of this (if not all) is taken from SFML's CMake files. :P

cmake_minimum_required( VERSION 2.8 )

# Build type
if ( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE )
endif()

# Boost
find_package( Boost )
if ( Boost_FOUND )
    include_directories( ${Boost_INCLUDE_DIR} )
else()
	message( FATAL_ERROR "We need Boost!" )
endif()

# Misc
set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=gnu++0x" )

# Project
project( "XML Parser" )

set( VERSION_MAJOR 1 )
set( VERSION_MINOR 0 )
set( VERSION_PATCH 0 )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )

# Build
set( INCROOT ${PROJECT_SOURCE_DIR}/include )
set( SRCROOT ${PROJECT_SOURCE_DIR}/src )

set( INC ${INCROOT}/xml/Document.hpp
		 ${INCROOT}/xml/Node.hpp
		 ${INCROOT}/xml/Attribute.hpp
		 ${INCROOT}/xml/Misc.hpp
		 ${INCROOT}/xml/Xml.hpp )

set( SRC ${SRCROOT}/xml/Document.cpp
		 ${SRCROOT}/xml/Node.cpp
		 ${SRCROOT}/xml/Attribute.cpp
		 ${SRCROOT}/xml/Misc.cpp )

add_library( sc0-xml SHARED ${SRC} ${INC} )
set_target_properties(sc0-xml PROPERTIES DEBUG_POSTFIX -d)

# Install
install( TARGETS sc0-xml
		 DESTINATION bin )
install( DIRECTORY include
		 DESTINATION . )
