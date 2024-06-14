#include <efi.h>
#include <efilib.h>

EFI_GUID gid1=EFI_LOADED_IMAGE_PROTOCOL_GUID;
EFI_GUID gid2=EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
EFI_GUID gid3=EFI_FILE_INFO_ID;

void mes(EFI_STATUS s,CHAR16*m){
    if(s==EFI_SUCCESS){Print(L"OK:%s\n\r",m);}else{Print(L"NG:%s\n\r",m);} }

int getInt(char*p,int s){ int v=0;
    while(s-->0){v=v*10 + ((int)(*p++) - 0x30);} return v; }

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

    s = Root->Open(Root,&file,L"\\EFI\\MYPHOTO.PNM",
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

    UINTN i,j,size=FileInfo->FileSize+1; /* for \0 at the end of string */
    char*buf; s=AllocatePool(EfiLoaderData,size,(void**)&buf);
    mes(s,L"AllocatePool for buf");
    file->Read(file, &size, buf);
    buf[size]=0; Print(L"after Read size=%d\n\r",size);
    if( buf[0]=='P' && buf[1]=='6' ) Print(L"Recognize P6 format\n\r");
    else {Print(L"Cant' Recoginzie P6 format\n\r"); goto QUIT;}
    UINTN wd=getInt(buf+3,3);
    UINTN ht=getInt(buf+7,3);
    UINTN depth=getInt(buf+11,3);
    Print(L" wd=%d ht=%d depth=%d\n\r",wd,ht,depth);
    Print(L"=== Start Viewing ===\n\r");
    //WaitForSingleEvent(systab->ConIn->WaitForKey, 0);

    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    s=LibLocateProtocol(&GraphicsOutputProtocol, (void**)&gop);
    mes(s,L"Open GraphicsOutputProtocol");
    UINT32 *fb=(UINT32*)gop->Mode->FrameBufferBase;
    UINTN fbsize=gop->Mode->FrameBufferSize;
    UINTN width=gop->Mode->Info->HorizontalResolution;
    UINTN height=gop->Mode->Info->VerticalResolution;
    Print(L" fb=%08x fbsize=%d width=%d height=%d\n\r",fb,fbsize,width,height);


    UINT32*p=fb; int ofst=15,vofst=(height-ht)/2,hofst=(width-wd)/2;
    for(i=0;i<ht;i++) {
      for(j=0;j<wd;j++) {
         UINTN r=buf[ofst+(i*wd+j)*3];
         UINTN g=buf[ofst+(i*wd+j)*3+1];
         UINTN b=buf[ofst+(i*wd+j)*3+2];
         UINT32 v=(r&0xff)*0x010000+(g&0xff)*0x0100+(b&0xff);
         p[(i+vofst)*width+j+hofst]=v;
      }
    }

  QUIT:
    WaitForSingleEvent(systab->ConIn->WaitForKey, 0);
    return EFI_SUCCESS;
}
