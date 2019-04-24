
#include <iostream>

#include <windows.h>

#include "platform.h"

using namespace projstruct;

//Returns the last Win32 error, in string format. Returns an empty string if there is no error.
// Copied from https://stackoverflow.com/a/17387176/561624
std::string GetLastErrorAsString()
{
    //Get the error message, if any.
    DWORD errorMessageID = ::GetLastError();
    if(errorMessageID == 0)
        return std::string(); //No error message has been recorded

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    std::string message(messageBuffer, size);

    //Free the buffer.
    LocalFree(messageBuffer);

    return message;
}

/* From http://blog.nuclex-games.com/2012/06/how-to-create-directories-recursively-with-win32/
 * Retrieved April 12, 2017
 * Posted by user Cygon (http://blog.nuclex-games.com/author/cygon/)
 *
 * This code is free for the taking and you can use it however you want.
 * 
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * As I just found out, the CreateDirectory function on Win32 can only create one directory at a time. 
 * If one, for example, specifies C:\Users\All Users\FirstNew\SecondNew as the directory to create, and 
 * both FirstNew and SecondNew do not exist, then CreateDirectory() fails.... Here's a workaround:
 */

/// <summary>Creates all directories down to the specified path</summary>
/// <param name="directory">Directory that will be created recursively</param>
/// <remarks>
///   The provided directory must not be terminated with a path separator.
/// </remarks>
void projstruct::Mkdir_p( std::string path )
{
    static const std::string separators( "\\/" );
    // If the specified directory name doesn't exist, do our thing
    DWORD fileAttributes = ::GetFileAttributes( path.c_str() );

    if ( fileAttributes == INVALID_FILE_ATTRIBUTES )
    {
        // Recursively do it all again for the parent directory, if any
        std::size_t slashIndex = path.find_last_of( separators );

        if ( slashIndex != std::string::npos )
        {
            Mkdir_p( path.substr( 0, slashIndex ) );
        }

        // Create the last directory on the path (the recursive calls will have taken
        // care of the parent directories by now)
        BOOL result = ::CreateDirectory( path.c_str(), nullptr );

        if ( result == FALSE )
        {
            DWORD errorMessageID = ::GetLastError();

            if ( errorMessageID != ERROR_ALREADY_EXISTS )
            {
                std::cerr << path << ": " << GetLastErrorAsString() << std::endl;
            }
        }
    }
    else
    { // Specified directory name already exists as a file or directory
        bool isDirectoryOrJunction =
        ((fileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) ||
        ((fileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0);

        if ( !isDirectoryOrJunction )
        {
            throw std::runtime_error(
            "Could not create directory because a file with the same name exists"
            );
        }
    }
}