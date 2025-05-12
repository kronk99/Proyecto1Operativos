# Compilador
CC = gcc

# Opciones de compilación
CFLAGS = -Wall -Wextra -g -Icar

# Archivos fuente
SRCS = main.c \
	Calendarizador/algoritmos/prioridad.c \
	Calendarizador/algoritmos/RoundRobin.c \
	Calendarizador/algoritmos/SJF.c \
	Calendarizador/ReadyQueue.c \
	Calendarizador/Scheduler.c \
	Threads/CEmutex_destroy.c \
	Threads/CEmutex_init.c \
	Threads/CEmutex_lock.c \
	Threads/CEmutex_unlock.c \
	Threads/CEthread_create.c \
	Threads/CEthread_end.c \
	Threads/CEthread_join.c \
	Flow/flow.c \
	Testing.c \
	interfaz.c

# Archivos objeto
OBJS = $(SRCS:.c=.o)

# Nombre del ejecutable
EXEC = main

# Regla por defecto
all: $(EXEC)

# Enlazar ejecutable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lSDL2 -lSDL2_image

# Regla general para compilar .c a .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpiar objetos y ejecutable
clean:
	rm -f $(OBJS) $(EXEC)

# Regla para compilar y ejecutar el programa normalmente
run: $(EXEC)
	./$(EXEC)

# Regla para ejecutar el programa en GDB
gdb: $(EXEC)
	gdb ./$(EXEC)
