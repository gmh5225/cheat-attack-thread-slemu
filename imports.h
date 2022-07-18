#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <vector>
#include <thread>
#include <iostream>
#include <winternl.h>
#include <windef.h>

#define dbg(content, ...) printf( "[+] " content "\n", __VA_ARGS__)

#include "thread.h"
#include "process.h"
#include "utils.h"
#include "bytes.h"
#include "emulator.h"