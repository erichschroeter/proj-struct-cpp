#ifndef PROJ_STRUCT_H
#define PROJ_STRUCT_H

#include <vector>
#include <string>

namespace projstruct
{
    class Command
    {
    public:
        virtual void Execute();
        virtual std::string To_String() const;
        std::vector< std::string > args;
    };

    class Mkdir_Command : public Command
    {
    public:
        Mkdir_Command();
        void Execute();
    };

    class Touch_Command : public Command
    {
    public:
        Touch_Command();
        void Execute();
    };

    std::ostream & operator<<( std::ostream & os, const Command & cmd );

    void Strip_Absolute_Path_Prefix( std::string & path );
    bool Is_Dir( std::string path );
    std::string Get_Path( const std::string & path );
    std::vector< Command * > Parse( std::string config_string );
}

#endif /* #ifndef PROJ_STRUCT_H */
