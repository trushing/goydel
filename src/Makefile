NAME=Goydel
CFLAGS=-std=c++11 -g -Wall -o $(NAME) -fdiagnostics-color
DEBUG=-g -Wall -fdiagnostics-color
LINKER=-I. -IXEffects -lIrrlicht -lXrandr -lXext -lX11
BULLETFLAGS=`pkg-config --libs --cflags bullet`
OBJS=main.o client.o scene.o
SOURCE=main.cpp eventNode.cpp eventqueue.cpp playermanager.cpp scene.cpp controllers.cpp entity.cpp client.cpp weapons/projectile.cpp
CC=g++
all : $(SOURCE)
	$(CC) $(SOURCE) $(LINKER) $(BULLETFLAGS) $(CFLAGS)


#~ all : $(OBJS)
#~ 	$(CC) $(OBJS) $(LINKER) $(BULLETFLAGS) $(CFLAGS)
#~ main.o : main.cpp weapons/projectile.h weapons/weapons.h weapons/weapon.h
#~ 	$(CC) -c main.cpp $(LINKER) $(BULLETFLAGS) $(DBUG)
#~ client.o : client.cpp client.h weapons/projectile.h weapons/weapons.h weapons/weapon.h
#~ 	$(CC) -c client.cpp $(LINKER) $(BULLETFLAGS) $(DBUG)
#~ #player.o : player.cpp player.h weapons/projectile.h weapons/weapons.h weapons/weapon.h
#~ #	$(CC) -c  player.cpp $(LINKER) $(BULLETFLAGS) $(DBUG)
#~ scene.o : scene.cpp scene.h weapons/projectile.h weapons/weapons.h weapons/weapon.h
#~ 	$(CC) -c scene.cpp $(LINKER) $(BULLETFLAGS) $(DBUG)
#controller.o : controller.cpp controller.h weapons/projectile.h weapons/weapons.h weapons/weapon.h controller.h controls.h
#	$(CC) -c controller.cpp $(LINKER) $(BULLETFLAGS) $(DBUG)
#vehicle.o : vehicle.cpp vehicle.h weapons/projectile.h weapons/weapons.h weapons/weapon.h
#	$(CC) -c vehicle.cpp $(LINKER) $(BULLETFLAGS) $(DBUG)
#include ""
#include ""
