
#include <catch2/catch.hpp>

#include <proj-struct.h>

using namespace projstruct;

SCENARIO( "Is_Dir returns false for /a/path/with/no/separator/suffix", "[proj-struct_tests.cpp]" )
{
    const std::string path = "/a/path/with/no/separator/suffix";
    GIVEN( "A path \"" << path << "\"" )
    {
        THEN( "Is_Dir returns false" )
        {
            REQUIRE_FALSE( Is_Dir( path ) );
        }
    }
}

SCENARIO( "Is_Dir returns true for /a/path/with/separator/suffix/", "[proj-struct_tests.cpp]" )
{
    const std::string path = "/a/path/with/separator/suffix/";
    GIVEN( "A path \"" << path << "\"" )
    {
        THEN( "Is_Dir returns true" )
        {
            REQUIRE( Is_Dir( path ) );
        }
    }
}

SCENARIO( "Parsing empty lines between two non-empty lines is ignored", "[proj-struct_tests.cpp]" )
{
    const std::string config = R"(/a/valid/directory/before/empty/line/

/a/valid/directory/after/empty/line/)";

    GIVEN( "Parse is passed the following input:\n" << config )
    {
        std::vector< Command > commands = Parse( config );

        THEN( "Only 2 entries exist" )
        {
            REQUIRE( commands.size() == 2 );
            AND_THEN( "The first entry is \"/a/valid/directory/before/empty/line/\"" )
            {
                REQUIRE( commands[ 0 ].To_String() == "/a/valid/directory/before/empty/line/" );
                AND_THEN( "The second entry is \"/a/valid/directory/after/empty/line/\"" )
                {
                    REQUIRE( commands[ 1 ].To_String() == "/a/valid/directory/after/empty/line/" );
                }
            }
        }
    }
}
