#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define WIDTH 600
#define HEIGHT 500
#define BUTTON_HEIGHT 40
#define BUTTON_MARGIN 10
#define MAX_TEXT_LEN 10

typedef struct {
    int x, y, width, height;
    char label[50];
    int selected;
} Button;

Button planning_buttons[] = {
    {50, 50, 200, BUTTON_HEIGHT, "RR", 0},
    {50, 100, 200, BUTTON_HEIGHT, "Prioridad", 0},
    {50, 150, 200, BUTTON_HEIGHT, "SJF", 0},
    {50, 200, 200, BUTTON_HEIGHT, "FCFS", 0},
    {50, 250, 200, BUTTON_HEIGHT, "Tiempo Real", 0}
};

Button flow_buttons[] = {
    {300, 50, 200, BUTTON_HEIGHT, "Equidad", 0},
    {300, 100, 200, BUTTON_HEIGHT, "Letrero", 0},
    {300, 150, 200, BUTTON_HEIGHT, "FIFO", 0}
};

Button play_button = {50, 400, 200, BUTTON_HEIGHT, "Jugar", 0};  // El nuevo botón "Jugar"
char input_text[MAX_TEXT_LEN] = "";
int input_selected = 0;
int game_started = 0;  // Variable que indica si el juego ha comenzado

void draw_button(Display *d, Window w, GC gc, Button btn) {
    XSetForeground(d, gc, btn.selected ? 0x00ff00 : 0xcccccc);
    XFillRectangle(d, w, gc, btn.x, btn.y, btn.width, btn.height);
    XSetForeground(d, gc, 0x000000);
    XDrawRectangle(d, w, gc, btn.x, btn.y, btn.width, btn.height);
    XDrawString(d, w, gc, btn.x + 10, btn.y + 25, btn.label, strlen(btn.label));
}

int is_inside(Button btn, int x, int y) {
    return x >= btn.x && x <= btn.x + btn.width && y >= btn.y && y <= btn.y + btn.height;
}

void redraw(Display *d, Window w, GC gc) {
    // Redibujar los botones de planificación
    for (int i = 0; i < sizeof(planning_buttons)/sizeof(Button); i++)
        draw_button(d, w, gc, planning_buttons[i]);

    // Redibujar los botones de flujo
    for (int i = 0; i < sizeof(flow_buttons)/sizeof(Button); i++)
        draw_button(d, w, gc, flow_buttons[i]);

    
    // Redibujar el botón "Jugar"
    draw_button(d, w, gc, play_button);

    // Input de cantidad de carros
    XSetForeground(d, gc, 0xaaaaaa);
    XFillRectangle(d, w, gc, 50, 350, 200, BUTTON_HEIGHT);
    XSetForeground(d, gc, 0x000000);
    XDrawRectangle(d, w, gc, 50, 350, 200, BUTTON_HEIGHT);
    XDrawString(d, w, gc, 60, 375, input_text, strlen(input_text));

    // Etiqueta
    XDrawString(d, w, gc, 50, 340, "Cantidad de carros:", 19);
}

int main() {
    Display *display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "No se pudo abrir el display\n");
        return 1;
    }

    int screen = DefaultScreen(display);
    Window win = XCreateSimpleWindow(display, RootWindow(display, screen), 100, 100, WIDTH, HEIGHT, 1,
                                     BlackPixel(display, screen), WhitePixel(display, screen));
    XSelectInput(display, win, ExposureMask | ButtonPressMask | KeyPressMask);
    XMapWindow(display, win);

    GC gc = XCreateGC(display, win, 0, NULL);
    XFontStruct *font = XLoadQueryFont(display, "fixed");
    if (!font) font = XQueryFont(display, XGContextFromGC(gc));
    XSetFont(display, gc, font->fid);

    XEvent e;
    while (1) {
        XNextEvent(display, &e);
        if (e.type == Expose) {
            redraw(display, win, gc);  // Redibujar toda la interfaz cuando se expone la ventana
        } else if (e.type == ButtonPress) {
            int x = e.xbutton.x, y = e.xbutton.y;
            for (int i = 0; i < sizeof(planning_buttons)/sizeof(Button); i++) {
                if (is_inside(planning_buttons[i], x, y)) {
                    for (int j = 0; j < sizeof(planning_buttons)/sizeof(Button); j++)
                        planning_buttons[j].selected = 0;
                    planning_buttons[i].selected = 1;
                    printf("Seleccionado algoritmo: %s\n", planning_buttons[i].label);
                    break;
                }
            }
            for (int i = 0; i < sizeof(flow_buttons)/sizeof(Button); i++) {
                if (is_inside(flow_buttons[i], x, y)) {
                    for (int j = 0; j < sizeof(flow_buttons)/sizeof(Button); j++)
                        flow_buttons[j].selected = 0;
                    flow_buttons[i].selected = 1;
                    printf("Seleccionado flujo: %s\n", flow_buttons[i].label);
                    break;
                }
            }
            if (is_inside(play_button, x, y)) {
                game_started = 1;  // Cambiar el estado del juego
                printf("Juego iniciado\n");
                XClearWindow(display, win);  // Limpiar la ventana para mostrar la carretera
            }
            // Área de entrada seleccionada
            if (x >= 50 && x <= 250 && y >= 350 && y <= 390)
                input_selected = 1;
            else
                input_selected = 0;
            redraw(display, win, gc);  // Redibujar la ventana después del clic
        } else if (e.type == KeyPress && input_selected) {
            char buf[32];
            KeySym key;
            int len = XLookupString(&e.xkey, buf, sizeof buf, &key, NULL);
            if (key == XK_BackSpace && strlen(input_text) > 0) {
                input_text[strlen(input_text)-1] = '\0';
            } else if (len > 0 && strlen(input_text) + len < MAX_TEXT_LEN) {
                strncat(input_text, buf, len);
            }
            redraw(display, win, gc);  // Redibujar la ventana después de la entrada de texto
        }
    }

    XFreeGC(display, gc);
    XCloseDisplay(display);
    return 0;
}
