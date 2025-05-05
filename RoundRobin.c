//la velocidad de los carros afecta la velocidad con la que se disminuye el quantum
//cuando el letrero cambia, el quantum se setea directamente en 0.
//burst time es cuanto le falta para llegar al otro lado de la carretera
#include "RoundRobin.h"
//inicializacion del calendarizador round robin.
void initRR(roundRobin *circular){ //no se usa
    circular = malloc(sizeof(roundRobin));
};
//insertar un carro a la lista de calendarizacion de RR (circular)
void scheduleCar(roundRobin *circular,Car *car){
    if(circular->head!=NULL){
        Node *new_node = malloc(sizeof(Node)); //crea un nuevo nodo
        new_node->Car = car; //le inserta el carro
        new_node->next = circular->tail; //coloca el puntero del nex del nuevo nodo a la cola
        circular->tail = new_node; //convierte la nueva cola en el circularTail
        //ya que lo insertara de ultimo
        circular->head->next = new_node ;//el next de la cabeza apunta al nuevo tail.
    }
    else{ //si la cabeza es nula
        Node *new_node = malloc(sizeof(Node)); //crea un nuevo nodo
        new_node->Car = car; //le inserta el carro
        new_node->next = NULL; //coloca el puntero de next en nulo
        circular->head = new_node ;//asigna la cabeza al nuevo nodo
        circular->tail = new_node;
        circular->current = new_node; //va a apuntar hacia la cabeza siempre
    }
    

}; //inserta el carro a la la lista circular
void checkQuantum(roundRobin *circular, int quantum){
    if(circular->Qos==0){//si el quantum llega a cero , hace el cambio de contexto
        //al current actual se llama al mutex para ponerlo a dormir
        //se cambia el current
        circular->current= circular->current->next; //cambio de contexto
        //aca se llama al flutex para despertar el hilo nuevamente., en realidad
        //se deberia de llamar al flutex en la cola de listo, para despertarlo, INVESTIGAR
        //SI EL FLUTEX LO PUEDE PONER EN UN ESTADO DE LISTO, Y NO DE DURMIENDO A EJECUTANDO

        //esto en lugar de ser una lista de carros, deberia de ser una lista de ceThreads 
        //que contenga los cethreads asociado al carro, ahora bien , deberia de llamar
        //aca al mutex, para ponerlo en espera, y cuando haga el checkCuantum 
        circular->Qos=quantum; //reset del cuantum 
    }
    else{
        decreaseQuantum(circular);
    }
    //el detalle de la funcion anterior es para hacer el cambio de contexto, primero checkeo
    //el quantum , si ya es cero hago el cambio de contexto, yo deberia de hacer un dequeue
    //y para ese hilo , al hacer dequeue llamar al mutex y ponerlo a dormir, luego deberia 
    //de cambiar el current de la lista circular y llamar a calendarizar para anadirlo
    //con el nuevo head de la cola de listo deberia de activarlo, pero solo se activa
    //ese primer head, ya que si esta activo se esta usando el hilo, esa activacion es 
    //despues de hacer el pop a la cola supongo //pero cuando llame al checkquantum
    //deberia de ponerlo en mutex y hacer el cambio de contexto.
}; //hace el context switch 

Node* getcurrent(roundRobin *circular){
    //aca debo de verificar el burst time , si es 0,
    return circular->current;
}; //standby 
void decreaseQuantum(roundRobin *circular){
    
    circular->Qos-=1;
    circular->current->Car->burstTime-=1;
    //checkeo el burst time
    if (circular->current->Car->burstTime ==0){//FALTA LA ELIMINACION
        Node *oldcurrent =circular->current;
        deleteNode(circular); //elimina el nodo current
        //resetea el quantum
    }
};
void nextcurrent(roundRobin *circular){ //elimina de la lista circular
    circular->current = circular->current->next;
}
//IDEA EN LA QUE QUEDE, BASICAMENTE USAR EL CIRCULAR LIST COMO LA READY QUEUE
//SACAR DE LA LISTA A MEDIDA QUE SE COMPLETEN LOS Carros.

