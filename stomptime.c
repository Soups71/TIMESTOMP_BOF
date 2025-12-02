/*
Created by: Soups71
Date: 01DEC2025

*/

#include <windows.h>
#include "imports/beacon.h"
#include "imports/windefs.h"



void copyTimeStamp(char* source_file, char* dest_file){
    BeaconPrintf(CALLBACK_OUTPUT, "[-] Source: %s\n", source_file);
    BeaconPrintf(CALLBACK_OUTPUT, "[-] Target: %s\n", dest_file);


    HANDLE hSourceFile = KERNEL32$CreateFileA(
        source_file,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hSourceFile == INVALID_HANDLE_VALUE) {
        BeaconPrintf(CALLBACK_ERROR, "[!] Failed to open source file (Error: %d)\n", KERNEL32$GetLastError());
        return;
    }
    // Get file times from the original file
    FILETIME ftCreation, ftLastAccess, ftLastWrite;
    if (!KERNEL32$GetFileTime(hSourceFile, &ftCreation, &ftLastAccess, &ftLastWrite)) {
        BeaconPrintf(CALLBACK_ERROR, "[!] Failed to get source timestamps (Error: %d)\n", KERNEL32$GetLastError());
        KERNEL32$CloseHandle(hSourceFile);
        return;
    }

    KERNEL32$CloseHandle(hSourceFile);

    HANDLE hDestFile = KERNEL32$CreateFileA(
        dest_file,
        FILE_WRITE_ATTRIBUTES,
        FILE_SHARE_READ | FILE_SHARE_READ	,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hDestFile == INVALID_HANDLE_VALUE) {
        BeaconPrintf(CALLBACK_ERROR, "[!] Failed to open target file (Error: %d)\n", KERNEL32$GetLastError());
        return;
    }
    // Apply file times from source file to the target/dest
    if (!KERNEL32$SetFileTime(hDestFile, &ftCreation, &ftLastAccess, &ftLastWrite)) {
        BeaconPrintf(CALLBACK_ERROR, "[!] Failed to modify timestamps (Error: %d)\n", KERNEL32$GetLastError());
    }
    else {
        BeaconPrintf(CALLBACK_OUTPUT, "[+] Successfully modified MAC timestamp\n");
    }

    KERNEL32$CloseHandle(hDestFile);
}

void manualTime(char* timestamp, char* dest_file){
    SYSTEMTIME st;
    FILETIME ft;

    sscanf_s(timestamp, "%d-%d-%dT%d:%d:%d", &st.wYear, &st.wMonth, &st.wDay, &st.wHour, &st.wMinute, &st.wSecond);
    
    // Converts a system time to file time format. System time is based on Coordinated Universal Time (UTC). 
    // https://learn.microsoft.com/en-us/windows/win32/api/timezoneapi/nf-timezoneapi-systemtimetofiletime
    if (!KERNEL32$SystemTimeToFileTime(&st, &ft)) {
        BeaconPrintf(CALLBACK_ERROR, "[-] Error: %d\n", KERNEL32$GetLastError());
        BeaconPrintf(CALLBACK_ERROR, "[-] Common issues: Not using correct format \"YYYY-MM-DDThh:mm:ss\" or not providing a valid date and time.\n");
    }
    HANDLE hDestFile = KERNEL32$CreateFileA(
        dest_file,
        FILE_WRITE_ATTRIBUTES,
        FILE_SHARE_READ | FILE_SHARE_READ	,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hDestFile == INVALID_HANDLE_VALUE) {
        BeaconPrintf(CALLBACK_ERROR, "[!] Failed to open target file (Error: %d)\n", KERNEL32$GetLastError());
        return;
    }
    
    if (!KERNEL32$SetFileTime(hDestFile, &ft, &ft, &ft)) {
        BeaconPrintf(CALLBACK_ERROR, "[!] Failed to modify timestamps (Error: %d)\n", KERNEL32$GetLastError());
    }
    else {
        BeaconPrintf(CALLBACK_OUTPUT, "[+] Successfully modified MAC timestamp\n");
    }

    KERNEL32$CloseHandle(hDestFile);
}

void go(char *args, int length ) {
    datap parser;
    int type;
    char* usr_source_file;
    char* usr_dest_file;
    char* timestamp;
    BeaconDataParse(&parser, args, length);
    type = BeaconDataInt(&parser);
    usr_source_file = BeaconDataExtract(&parser, NULL);
    usr_dest_file = BeaconDataExtract(&parser, NULL);
    timestamp = BeaconDataExtract(&parser, NULL);


    if(type == 1){
        BeaconPrintf(CALLBACK_OUTPUT, "[+] Copying Timestamp\n");
        copyTimeStamp(usr_source_file, usr_dest_file);
    }else if(type == 2){
        BeaconPrintf(CALLBACK_OUTPUT, "[+] Creating New Timestamp\n");
        manualTime(timestamp, usr_dest_file);
    }

    
}