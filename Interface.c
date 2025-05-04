#include <X11/Xlib.h>
#include <X11/xpm.h>
#include <X11/Xatom.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "Carretera3.xpm"  // Asegúrate de que este archivo contenga `char *Carretera3[]`

int main() {
    // Abrir display
    Display *display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "No se pudo abrir el display.\n");
        return 1;
    }

    int screen = DefaultScreen(display);

    // Crear ventana
    unsigned long background = 0x000000; // color azul claro en formato hexadecimal RGB
    Window window = XCreateSimpleWindow(display, RootWindow(display, screen),
                                        100, 100, 1700, 900, 1,
                                        BlackPixel(display, screen),
                                        background); // este es el fondo

    // Seleccionar eventos que queremos escuchar
    XSelectInput(display, window, ExposureMask);

    // Mostrar ventana
    XMapWindow(display, window);
    XFlush(display);

    // Esperar un poco para que el window manager registre la ventana
    usleep(100000); // 100 ms

    // Solicitar maximizar la ventana
    Atom wm_state = XInternAtom(display, "_NET_WM_STATE", False);
    Atom max_horz = XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
    Atom max_vert = XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_VERT", False);

    XEvent xev = {0};
    xev.type = ClientMessage;
    xev.xclient.window = window;
    xev.xclient.message_type = wm_state;
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = 1; // 1 = agregar propiedad
    xev.xclient.data.l[1] = max_horz;
    xev.xclient.data.l[2] = max_vert;
    XSendEvent(display, RootWindow(display, screen), False,
               SubstructureRedirectMask | SubstructureNotifyMask, &xev);

    // Cargar imagen XPM
    Pixmap pixmap, mask;
    XpmAttributes attr;
    attr.valuemask = XpmSize;

    if (XpmCreatePixmapFromData(display, window, (char **)Carretera3, &pixmap, &mask, &attr) != 0) {
        fprintf(stderr, "Error al cargar la imagen XPM.\n");
        return 1;
    }

    // Crear contexto gráfico (GC)
    GC gc = XCreateGC(display, window, 0, 0);

    // Dibujar la imagen de inmediato
    XCopyArea(display, pixmap, window, gc, 0, 0, attr.width, attr.height, 300, 300);

    // Bucle de eventos
    XEvent event;
    while (1) {
        XNextEvent(display, &event);
        if (event.type == Expose) {
            // Redibujar en caso de Expose (maximizar, restaurar, etc.)
            XCopyArea(display, pixmap, window, gc, 0, 0, attr.width, attr.height, 400, 300);
        }
    }

    // Liberar recursos (no se alcanza en este caso)
    XFreeGC(display, gc);
    XCloseDisplay(display);

    return 0;
}
