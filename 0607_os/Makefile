# kubota@ise.chuo-u.ac.jp, 2024年  6月 7日(金)
GNUEFI=gnu-efi-3.0.18
QEMU=qemu-system-x86_64
PROG=hw

dummy:
	echo end
all:
	make compile
	make mkimg
	make qemu1
compile:
	(cd apps; make $(PROG).efi)
	cp apps/$(PROG).efi BOOTX64.EFI
mkimg: 
	dd if=/dev/zero of=fat.img bs=1k count=14400
	mkfs.msdos fat.img
	mmd -i fat.img ::/EFI
	mmd -i fat.img ::/EFI/BOOT
	mcopy -i fat.img BOOTX64.EFI ::/EFI/BOOT
qemu1:
	$(QEMU) -bios /usr/share/ovmf/OVMF.fd -hda fat.img

clean:
	/bin/rm -f fat.img
