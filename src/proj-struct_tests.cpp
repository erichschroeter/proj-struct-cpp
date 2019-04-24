
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
        std::vector< Command * > commands = Parse( config );

        THEN( "Only 2 entries exist" )
        {
            REQUIRE( commands.size() == 2 );
            AND_THEN( "Both entries are non-empty" )
            {
                for ( auto & cmd : commands )
                {
                    REQUIRE_FALSE( (*cmd).To_String().empty() );
                }
            }
        }
    }
}

SCENARIO( "Parsing absolute folder paths are converted to relative paths", "[proj-struct_tests.cpp]" )
{
    const std::string config = "/an/absolute/directory/";

    GIVEN( "Parse is passed the following input:\n" << config )
    {
        std::vector< Command * > commands = Parse( config );

        THEN( "The folder argument is \"an/absolute/directory/\"" )
        {
            REQUIRE( commands[ 0 ]->args[ 2 ] == "an/absolute/directory/" );
        }
    }
}

SCENARIO( "Parsing ignores prefixed whitespace", "[proj-struct_tests.cpp]" )
{
    const std::string config_prefixed_with_spaces = " /an/absolute/directory/";
    const std::string config_prefixed_with_tabs = "\t/an/absolute/directory/";

    GIVEN( "Parse is passed the following input:\n" << config_prefixed_with_spaces )
    {
        std::vector< Command * > commands = Parse( config_prefixed_with_spaces );

        THEN( "The folder argument is \"an/absolute/directory/\"" )
        {
            REQUIRE( commands[ 0 ]->args[ 2 ] == "an/absolute/directory/" );
        }
    }

    GIVEN( "Parse is passed the following input:\n" << config_prefixed_with_tabs )
    {
        std::vector< Command * > commands = Parse( config_prefixed_with_tabs );

        THEN( "The folder argument is \"an/absolute/directory/\"" )
        {
            REQUIRE( commands[ 0 ]->args[ 2 ] == "an/absolute/directory/" );
        }
    }
}
