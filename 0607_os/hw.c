#include <efi.h>
#include <efilib.h>

EFI_STATUS efi_main (EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *systab) {
    EFI_STATUS st;
    InitializeLib(image_handle, systab);
    st=systab->ConOut->OutputString(systab->ConOut, L"Hello World!\n\r");

    WaitForSingleEvent(systab->ConIn->WaitForKey, 0);

    return st;
}
