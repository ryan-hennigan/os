/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "assert.H"

#include "utils.H"
#include "console.H"

/*--------------------------------------------------------------------------*/
/* _assert() FUNCTION: gets called when assert() macro fails. */
/*--------------------------------------------------------------------------*/
using namespace kutils;
void _assert (const char* _file, const uint32_t _line, const char* _message )  {
  /* Prints current file, line number, and failed assertion. */
  char temp[15];
  Console::puts("Assertion failed at file: ");
  Console::puts(_file);
  Console::puts(" line: ");
  int2str(_line, temp);
  Console::puts(temp);
  Console::puts(" assertion: ");
  Console::puts(_message);
  Console::puts("\n");
  abort();
}/* end _assert */

