/* 2024.06.07  kk@numinfo.ise.chuo-u.ac.jp */
#include <efi.h>
#include <efilib.h>

EFI_STATUS efi_main (EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *systab) {
    EFI_STATUS st;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    InitializeLib(image_handle, systab);
    st=systab->ConOut->OutputString(systab->ConOut, L"Hello World!\n\r");

    st = LibLocateProtocol(&GraphicsOutputProtocol, (void**)&gop);
    if(st==EFI_SUCCESS) {
      st=systab->ConOut->OutputString(systab->ConOut, L"SUCCESS\n\r");
    }
    Print(L" pitch %d FBBase=%08x FBSize=%08x\r\n",
      gop->Mode->Info->PixelsPerScanLine,
      gop->Mode->FrameBufferBase,
      gop->Mode->FrameBufferSize);
    int wd=gop->Mode->Info->HorizontalResolution;
    int ht=gop->Mode->Info->VerticalResolution;
    Print(L" wd=%d x ht=%d\n\r", wd, ht);

    int*fb=(int*)gop->Mode->FrameBufferBase;
    int col[]={0xff0000,0xff00,0xff,0xffff00,0xff00ff,0xffff,0xffffff,0};
    int i,j,d=48;
    for(i=0;i<ht;i++) for(j=0;j<wd;j++){
        if((i<d||i>ht-d)||(j<d||j>wd-d))fb[i*wd+j]=col[((i/16)+(j/16))&0x7];
    }

    WaitForSingleEvent(systab->ConIn->WaitForKey, 0);

    return st;
}
