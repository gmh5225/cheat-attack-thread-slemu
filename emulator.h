namespace emulator
{
	auto emulate( const wchar_t* process_name, const wchar_t* module_name ) -> bool
	{
		const auto process = utils::get_process( process_name );

		if ( !process )
		{
			dbg( "%S not found", process_name );
			return false;
		}
		dbg( "%S: %i", process_name, process->get_id( ) );

		const auto module = process->get_module_entry( module_name );

		if ( !module.modBaseAddr )
		{
			dbg( "%S not found", module_name );
			return false;
		}
		dbg( "%S base: 0x%llx", module_name, module.modBaseAddr );

		for ( auto &thread : process->get_threads( ) )
		{
			const auto win32_start_address = thread->get_win32_start_address( );

			if ( win32_start_address > reinterpret_cast< std::uintptr_t > ( module.modBaseAddr ) + module.modBaseSize ||
				win32_start_address < reinterpret_cast< std::uintptr_t > ( module.modBaseAddr ) )
			{
				continue;
			}
			dbg( "%S thread found 0x%llx", module_name, win32_start_address );

			const auto local_kernel_base = utils::get_module( L"KERNELBASE.dll" );
			const auto local_sleep_ex = reinterpret_cast< std::uintptr_t > ( GetProcAddress( local_kernel_base, "SleepEx" ) );

			const auto remote_kernel_base = process->get_module_entry( L"KERNELBASE.dll" ).modBaseAddr;
			dbg( "%S's kernelbase.dll: 0x%llx", process_name, remote_kernel_base );

			const auto remote_sleep_ex = remote_kernel_base + local_sleep_ex - reinterpret_cast< std::uintptr_t > ( local_kernel_base );

			*reinterpret_cast< void ** >( &bytes::loop[17] ) = remote_sleep_ex;
			dbg( "prepared shellcode [SleepEx: 0x%llx]", remote_sleep_ex );

			const auto shellcode = process->allocate( sizeof( bytes::loop ), PAGE_EXECUTE_READWRITE );
			process->write( shellcode, bytes::loop, sizeof( bytes::loop ) );
			dbg( "shellcode created: 0x%llx", shellcode );

			auto ctx = thread->get_ctx( );
			ctx.Rip = shellcode;

			thread->set_ctx( ctx );
			dbg( "shellcode executed" );

			thread->close( );
			dbg( "thread trapped" );
		}

		process->close( );

		return true;
	}
}