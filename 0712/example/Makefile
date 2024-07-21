TARGETS=creat creat1 creat2 game selectex game1 termios game2  curses curses2 curses3 curses4 c4mod
LIBS=-lcurses

dummy: $(TARGETS)
	echo end

% : %.c
	$(CC) $< -o $@ $(LIBS)

clean:
	/bin/rm -f $(TARGETS)
