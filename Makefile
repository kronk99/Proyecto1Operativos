# Compilador
CC = gcc

# Opciones de compilación
CFLAGS = -Wall -Wextra -g -pthread -Icar

# Archivos fuente
SRCS = Road.c src/cars.c src/flow.c src/read.c

# Archivos objeto (automáticamente generados a partir de SRCS)
OBJS = $(SRCS:.c=.o)

# Nombre del ejecutable
EXEC = road

# Regla por defecto
all: $(EXEC)

# Enlazar ejecutable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Regla general para compilar .c a .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpiar objetos y ejecutable
clean:
	rm -f $(OBJS) $(EXEC)

# Regla para compilar y ejecutar el programa
run: $(EXEC)
	./$(EXEC)

