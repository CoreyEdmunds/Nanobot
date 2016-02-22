LDLIBS=-L/usr/lib/ -L/usr/lib/x86_64-linux-gnu -L/usr/X11R6/lib -lGL -lGLU -lglut -lXt -lX11 -lXext -lm -lICE -lSM -lXmu 
CFLAGS=-Os -Wall -Wshadow -Wstrict-prototypes \
          -Wmissing-prototypes -Wmissing-declarations \
          -Wredundant-decls -Wunreachable-code \

nanobot-lite: nanobot.o animate.o materials.o vector.o render.o draw.o joint.o particles-lite.o nanobot
	gcc nanobot.o animate.o materials.o vector.o render.o draw.o joint.o particles-lite.o -o nanobot-lite $(LDLIBS)
	make nanobot

nanobot: particles.o nanobot.o animate.o materials.o vector.o render.o draw.o joint.o 
	gcc nanobot.o animate.o materials.o vector.o render.o draw.o joint.o particles.o -o nanobot $(LDLIBS)

nanobot.o: nanobot.c materials.h render.h draw.h vector.h materials.h joint.h

materials.o: materials.c materials.h
vector.o: vector.c vector.h
render.o: render.c render.h draw.h vector.h materials.h joint.h
draw.o: draw.c draw.h
joint.o: joint.c joint.h vector.h
animate.o: animate.c animate.h
particles.o: particles.c particles.h
particles-lite.o: particles.c particles.h
	gcc $(CFLAGS) -DNO_SMOKELIGHT -c -o particles-lite.o particles.c
clean:
	rm -f *.o nanobot nanobot-lite
