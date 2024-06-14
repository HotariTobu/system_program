#include <efi.h>
#include <efilib.h>

EFI_GUID gid1=EFI_LOADED_IMAGE_PROTOCOL_GUID;
EFI_GUID gid2=EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
EFI_GUID gid3=EFI_FILE_INFO_ID;

void mes(EFI_STATUS s,CHAR16*m){
    if(s==EFI_SUCCESS){Print(L"OK:%s\n\r",m);}else{Print(L"NG:%s\n\r",m);} }

EFI_STATUS efi_main (EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *systab) {
    InitializeLib(image_handle, systab);
    Print(L"Hello World!\n\r");

    EFI_HANDLE_PROTOCOL HandleProtocol = systab->BootServices->HandleProtocol;
    EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
    HandleProtocol(image_handle, &gid1, (void**)&LoadedImage);

    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
    HandleProtocol(LoadedImage->DeviceHandle,&gid2,(void**)&FileSystem);

    EFI_FILE *Root, *file;
    EFI_STATUS s=FileSystem->OpenVolume(FileSystem,&Root);
    mes(s,L"OpenVolume");

    s = Root->Open(Root,&file,L"\\EFI\\AHA.TXT",
                   EFI_FILE_MODE_READ,EFI_FILE_READ_ONLY);
    mes(s,L"Open file");

    EFI_ALLOCATE_POOL AllocatePool = systab->BootServices->AllocatePool;

    UINTN FileInfoSize=0;
    EFI_FILE_INFO *FileInfo;

    /* The first call fails because of the 4th arg, but get the 3rd arg */
    s=file->GetInfo(file,&gid3,&FileInfoSize,NULL);
    /* s==EFI_BUFFER_TOO_SMALL is expected status */
    if( s==EFI_BUFFER_TOO_SMALL) Print(L"NG:EFI_BUFFER_TOO_SMALL\n\r");
    Print(L"  FileInfoSize=%d\n\r",FileInfoSize);
  
    s=AllocatePool(EfiLoaderData,FileInfoSize,(void**)&FileInfo);
    mes(s,L"AllocatePool");
    /* The 4th argument should be "(void*)FileInfo" !!!         */
    /* The 4th argument should not be "(void**)&FileInfo", this is WRONG! */
    s=file->GetInfo(file,&gid3,&FileInfoSize,(void*)FileInfo);
    mes(s,L"GetInfo FileInfo 2nd");
    Print(L"FileInfo->FileSize=%lx\n\r",FileInfo->FileSize);

    UINTN i=0,size=FileInfo->FileSize+1; /* for \0 at the end of string */
    char*buf; s=AllocatePool(EfiLoaderData,size,(void**)&buf);
    mes(s,L"AllocatePool for buf");
    file->Read(file, &size, buf);
    buf[size]=0; Print(L"after Read size=%d\n\r",size);
    for(i=0;i<size;i++){Print(L"%lc",buf[i]);}
    Print(L"Hex Dump:\n\r");
    for(i=0;i<size;i++){Print(L"%02x ",buf[i]);if(i%16==15)Print(L"\n\r");}
    Print(L"=== The end of File ===\n\r");

    WaitForSingleEvent(systab->ConIn->WaitForKey, 0);
    return EFI_SUCCESS;
}
