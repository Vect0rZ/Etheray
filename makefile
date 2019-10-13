OBJ = main.c color.c color3f.c matrix4x4.c material.c object.c vector3.c bmp16.c file.c camera.c
OUTPUT = etheray
DEPS = color.h color3f.h constants.h enums.h material.h matrix4x4.h object.h plane.h sphere.h triangle.h vector3.h bmp16.h file.h camera.h

make: $(OBJ)
	gcc -o bin/$(OUTPUT) $(OBJ) -I.