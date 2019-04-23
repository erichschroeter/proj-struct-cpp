
#include <iostream>
#include <sstream>

#include "proj-struct.h"

using namespace projstruct;

void Command::Execute()
{
}

std::string Command::To_String() const
{
    std::stringstream ss;
    bool is_first_arg = true;

    for ( auto & arg : args )
    {
        if ( is_first_arg )
        {
            is_first_arg = false;
            ss << arg;
        }
        else
        {
            ss << ' ' << arg;
        }
    }

    return ss.str();
}

std::ostream & projstruct::operator<<( std::ostream & os, const Command & cmd )
{
    return os << cmd.To_String();
}

bool projstruct::Is_Dir( std::string path )
{
    if ( !path.empty() )
    {
        return path.back() == '/';
    }

    return false;
}

void projstruct::Strip_Absolute_Path_Prefix( std::string & path )
{
    if ( !path.empty() )
    {
        if ( path[0] == '/' )
        {
            path.erase( 0, 1 );
        }
    }
}

std::vector< Command > projstruct::Parse( std::string config_string )
{
    std::vector< Command > commands;
    std::stringstream ss( config_string );
    std::string line;

    if ( !config_string.empty() )
    {
        while ( std::getline( ss, line, '\n' ) )
        {
            if ( !line.empty() )
            {
                Command cmd;
                cmd.args.push_back( line );
                commands.push_back( cmd );
            }
        }
    }

    return commands;
}
