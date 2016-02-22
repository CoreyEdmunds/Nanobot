LDLIBS=-L/usr/lib/ -L/usr/lib/x86_64-linux-gnu -L/usr/X11R6/lib -lGL -lGLU -lglut -lXt -lX11 -lXext -lm -lICE -lSM -lXmu 
CFLAGS=-Os -Wall -Wshadow -Wstrict-prototypes \
          -Wmissing-prototypes -Wmissing-declarations \
          -Wredundant-decls -Wunreachable-code \

nanobot-lite: src/nanobot.o src/animate.o src/materials.o src/vector.o src/render.o src/draw.o src/joint.o src/particles-lite.o nanobot
	gcc src/nanobot.o src/animate.o src/materials.o src/vector.o src/render.o src/draw.o src/joint.o src/particles-lite.o -o nanobot-lite $(LDLIBS)
	make nanobot

nanobot: src/particles.o src/nanobot.o src/animate.o src/materials.o src/vector.o src/render.o src/draw.o src/joint.o 
	gcc src/nanobot.o src/animate.o src/materials.o src/vector.o src/render.o src/draw.o src/joint.o src/particles.o -o nanobot $(LDLIBS)

nanobot.o: src/nanobot.c src/materials.h src/render.h src/draw.h src/vector.h src/materials.h src/joint.h

src/materials.o: src/materials.c src/materials.h
src/vector.o: src/vector.c src/vector.h
src/render.o: src/render.c src/render.h src/draw.h src/vector.h src/materials.h src/joint.h
src/draw.o: src/draw.c src/draw.h
src/joint.o: src/joint.c src/joint.h src/vector.h
src/animate.o: src/animate.c src/animate.h
src/particles.o: src/particles.c src/particles.h
src/particles-lite.o: src/particles.c src/particles.h
	gcc $(CFLAGS) -DNO_SMOKELIGHT -c -o src/particles-lite.o src/particles.c
clean:
	rm -f src/*.o nanobot nanobot-lite
