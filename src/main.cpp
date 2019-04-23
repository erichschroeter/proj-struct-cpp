#include <iostream>

#include <docopt/docopt.h>

#include "proj-struct.h"

static const char USAGE[] =
R"(proj-struct

Usage:
  proj-struct [--pretend] <input-file>

Options:
  --pretend  Only pretend to execute commands.

<input-file> Rules:
  1. Directories are lines that end with a path separator '/'.
  2. Files are lines that do not end with a path separator '/'.
  3. Empty lines are ignored.

<input-file> Example:
  /a/path/to/a/directory/
  another/path/to/a/directory/
  a.txt
  a/path/to/b.txt
)";

int main( int argc, char ** argv )
{
    std::map< std::string, docopt::value > args = docopt::docopt(
        USAGE,
        { argv + 1, argv + argc },
        true,
        "v1.0"
    );

    std::vector< projstruct::Command > commands = projstruct::Parse(
        R"(/some/root/path/folder/
some/relative/path/folder/
a.txt
a/path/folder/with/file/b.txt)"
    );

    if ( args[ "--pretend" ] )
    {
        for ( auto & cmd : commands )
        {
            std::cout << "would have executed: \"" << cmd << "\"" << std::endl;
        }
    }
    else
    {
        for ( auto & cmd : commands )
        {
            cmd.Execute();
        }
    }

    return 0;
}