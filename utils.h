namespace utils
{
	auto get_process( const wchar_t *name ) -> c_process *
	{
		PROCESSENTRY32 process_info;
		process_info.dwSize = sizeof( process_info );

		const auto snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );

		Process32First( snapshot, &process_info );
		do
		{
			if ( wcscmp( process_info.szExeFile, name ) == 0 )
			{
				CloseHandle( snapshot );

				return reinterpret_cast<c_process*>( OpenProcess( PROCESS_ALL_ACCESS, 0, process_info.th32ProcessID ) );
			}
		} while ( Process32Next( snapshot, &process_info ) );

		CloseHandle( snapshot );

		return nullptr;
	}

	auto get_module( const wchar_t* name ) -> HMODULE
	{
		const auto handle = GetModuleHandle( name );

		return ( handle ? handle : LoadLibrary( name ) );
	}
}