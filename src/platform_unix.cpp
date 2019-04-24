
#include <stdexcept>

#include <string.h>
#include <limits.h>     /* PATH_MAX */
#include <sys/stat.h>   /* mkdir(2) */
#include <errno.h>

#include "platform.h"

using namespace projstruct;

// Adapted from https://gist.github.com/JonathonReinhart/8c0d90191c38af2dcadb102c4e202950
void projstruct::Mkdir_p( std::string path )
{
    /* Adapted from http://stackoverflow.com/a/2336245/119527 */
    const size_t len = strlen( path.c_str() );
    char _path[PATH_MAX];
    char *p;

    errno = 0;

    /* Copy string so its mutable */
    if ( len > sizeof( _path ) - 1 )
    {
        errno = ENAMETOOLONG;
        throw std::runtime_error( "Could not create directory because a file with the same name exists" );
    }

    strcpy( _path, path.c_str() );

    /* Iterate the string */
    for ( p = _path + 1; *p; p++ )
    {
        if ( *p == '/' )
        {
            /* Temporarily truncate */
            *p = '\0';

            if ( mkdir(_path, S_IRWXU) != 0 )
            {
                if (errno != EEXIST)
                {
                    throw std::runtime_error( strerror( errno ) );
                }
            }

            *p = '/';
        }
    }

    if ( mkdir( _path, S_IRWXU ) != 0 )
    {
        if ( errno != EEXIST )
            throw std::runtime_error( strerror( errno ) );
    }
}
