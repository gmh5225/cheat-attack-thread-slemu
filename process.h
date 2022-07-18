class c_process
{
public:

    auto get_id( ) -> std::uint32_t
    {
        return GetProcessId( reinterpret_cast< HANDLE >( this ) );
    }

    auto get_module_entry( const wchar_t* name ) -> MODULEENTRY32
    {
        const auto snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, this->get_id( ) );

        MODULEENTRY32 module_entry = { 0 };
        module_entry.dwSize = sizeof( module_entry );
       
        Module32First( snapshot, &module_entry );

        do
        {
            if ( wcscmp( module_entry.szModule, name ) )
            {
                continue;
            }

            CloseHandle( snapshot );
            return module_entry;

        } while ( Module32Next( snapshot, &module_entry ) );

        CloseHandle( snapshot );

        return { 0 };
    }

    auto close( ) -> bool
    {
        return CloseHandle( reinterpret_cast< HANDLE >( this ) );
    }

	auto get_threads( ) -> std::vector<c_thread *>
	{
        std::vector<c_thread *> threads;
        threads.clear( );

        THREADENTRY32 thread_entry;
        thread_entry.dwSize = sizeof( thread_entry );

        const auto snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, 0 );

        Thread32First( snapshot, &thread_entry );
       
        do
        {
            if ( thread_entry.dwSize < FIELD_OFFSET( THREADENTRY32, th32OwnerProcessID ) + sizeof( thread_entry.th32OwnerProcessID ) || GetProcessId(reinterpret_cast<HANDLE>( this ) ) != thread_entry.th32OwnerProcessID )
            {
                continue;
            }

            threads.push_back( reinterpret_cast< c_thread * > ( OpenThread( THREAD_ALL_ACCESS, 0, thread_entry.th32ThreadID ) ) );

            thread_entry.dwSize = sizeof( thread_entry );
        } while ( Thread32Next( snapshot, &thread_entry ) );

        CloseHandle( snapshot );

        return threads;
	}

    auto write( std::uintptr_t address, void* buffer, std::size_t size ) -> bool
    {
        std::size_t bytes;
        if ( !WriteProcessMemory( reinterpret_cast< HANDLE >( this ), ( void * )address, buffer, size, &bytes ) || bytes != size )
        {
            return false;
        }

        return true;
    }

    auto allocate( const std::size_t size, unsigned long protection ) -> std::uintptr_t
    {
        return reinterpret_cast< std::uintptr_t> ( VirtualAllocEx( reinterpret_cast<HANDLE> ( this ), 0, size, MEM_COMMIT | MEM_RESERVE, protection ) );
    }
};