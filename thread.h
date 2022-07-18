class c_thread
{
public:

	auto get_id( ) -> uint32_t
	{
		return GetThreadId( reinterpret_cast< HANDLE >( this ) );
	}

	auto get_ctx( ) -> CONTEXT
	{
		CONTEXT ctx { 0 };
		ctx.ContextFlags = CONTEXT_FULL;
		GetThreadContext( reinterpret_cast< HANDLE >( this ), &ctx );

		return ctx;
	}

	auto close( ) -> bool
	{
		return CloseHandle( reinterpret_cast< HANDLE >( this ) );
	}

	auto set_ctx( CONTEXT ctx ) -> bool
	{
		return SetThreadContext( reinterpret_cast< HANDLE >( this ), &ctx );
	}

	auto get_win32_start_address( ) -> uintptr_t
	{
		uintptr_t win32_start_address;

		NtQueryInformationThread( reinterpret_cast< HANDLE > ( this ), static_cast<THREADINFOCLASS> ( 9 ), &win32_start_address, sizeof( DWORD64 ), 0 );

		return win32_start_address;
	}
};