#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <errno.h>

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

    std::ifstream ifs;
    ifs.open( args[ "<input-file>" ].asString() );

    if ( ifs.is_open() )
    {
      std::stringstream buf;
      buf << ifs.rdbuf();
      std::vector< projstruct::Command * > commands = projstruct::Parse( buf.str() );

      if ( args[ "--pretend" ].asBool() )
      {
          for ( auto & cmd : commands )
          {
              std::cout << "would have executed: \"" << (*cmd) << "\"" << std::endl;
          }
      }
      else
      {
          for ( auto & cmd : commands )
          {
              cmd->Execute();
          }
      }
    }
    else
    {
      std::cerr << args[ "<input-file>" ].asString() << ": " << strerror( errno ) << std::endl;
    }

    return 0;
}