#ifndef PROJ_STRUCT_H
#define PROJ_STRUCT_H

#include <vector>
#include <string>

namespace projstruct
{
    class Command
    {
    public:
        void Execute();
        std::string To_String() const;
        std::vector< std::string > args;
    };

    std::ostream & operator<<( std::ostream & os, const Command & cmd );

    bool Is_Dir( std::string path );
    std::vector< Command > Parse( std::string config_string );
}

#endif /* #ifndef PROJ_STRUCT_H */
