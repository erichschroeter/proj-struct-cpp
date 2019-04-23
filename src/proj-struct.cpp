
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

    for ( auto & arg : args )
    {
        ss << ' ' << arg;
    }

    return ss.str();
}

Mkdir_Command::Mkdir_Command()
{
    args.push_back( "mkdir" );
    args.push_back( "-p" );
}

void Mkdir_Command::Execute()
{
    system( To_String().c_str() );
}

Touch_Command::Touch_Command()
{
    args.push_back( "touch" );
}

void Touch_Command::Execute()
{
    system( To_String().c_str() );
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

std::vector< Command * > projstruct::Parse( std::string config_string )
{
    std::vector< Command * > commands;
    std::stringstream ss( config_string );
    std::string line;

    if ( !config_string.empty() )
    {
        while ( std::getline( ss, line, '\n' ) )
        {
            if ( !line.empty() )
            {
                if ( Is_Dir( line ) )
                {
                    Mkdir_Command * cmd = new Mkdir_Command();
                    cmd->args.push_back( line );
                    commands.push_back( cmd );
                }
                else
                {
                    Touch_Command * cmd = new Touch_Command();
                    cmd->args.push_back( line );
                    commands.push_back( cmd );
                }
            }
        }
    }

    return commands;
}
