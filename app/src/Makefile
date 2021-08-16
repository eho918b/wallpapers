CFLAGS = /O2
LIBS = shell32.lib ws2_32.lib gdi32.lib user32.lib comctl32.lib comdlg32.lib ole32.lib
INCL = -I..\include 
RES = ..\res\ribbon.res

all: main

main: main.c config.c time.c game.c ribbon.c
	$(CC) /Fe:..\bin\sc2.exe /Fo:..\obj\ $(CFLAGS) $(INCL) main.c config.c time.c game.c ribbon.c $(LIBS) $(RES)

clean:
	del ..\bin\sc2.exe