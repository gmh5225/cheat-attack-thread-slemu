#include "imports.h"

auto main( ) -> int
{
	dbg	( "emuation status: %d", emulator::emulate( L"SCPSL.exe", L"SL-AC.dll" ) );

	return std::cin.get( );
}