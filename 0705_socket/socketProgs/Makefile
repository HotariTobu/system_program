FILES= int-client.c int-server.c messerv.c messerv.txt
TARGETS=int-client int-server messerv
CFLAGS=-Wall -O2

dummy: $(TARGETS)
	echo end

% : %.c
	$(CC) $(CFLAGS) $< -o $@

zip:
	zip socketProgs.zip $(FILES) Makefile

clean:
	/bin/rm -f $(TARGETS)
