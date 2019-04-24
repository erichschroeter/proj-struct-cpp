
#include <iostream>
#include <fstream>
#include <sstream>

#include "proj-struct.h"
#include "platform.h"

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
    if ( args.size() > 2 )
    {
        Mkdir_p( args[ 2 ] );
    }
}

Touch_Command::Touch_Command()
{
    args.push_back( "touch" );
}

void Touch_Command::Execute()
{
    if ( args.size() > 1 )
    {
        std::ofstream ofs( args[ 1 ] );
    }
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

std::string projstruct::Get_Path( const std::string & path )
{
    const char separator = '/';
    size_t index = path.rfind( separator, path.length() );

    if ( index != std::string::npos )
    {
        return path.substr( 0, index );
    }

    return "";
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

inline std::string & ltrim( std::string & s, const char * t = " \t\n\r\f\v" )
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

inline std::string & rtrim( std::string & s, const char * t = " \t\n\r\f\v" )
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

inline std::string & Trim( std::string & s, const char * t = " \t\n\r\f\v" )
{
    return ltrim(rtrim(s, t), t);
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
                Strip_Absolute_Path_Prefix( Trim( line ) );

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
