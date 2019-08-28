#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <fstream>
using namespace std;


struct Nodo
{
    string valor;
    Nodo *der;
    Nodo *izq;
    Nodo *padre;
};

ofstream fp;
int r=0;
Nodo *crearNodo(string, Nodo *);
Nodo *arbol = NULL;
void insertar(Nodo *&arbol, string s, Nodo *);
void eliminar();
void menu();
void eliminar(Nodo *, string);
void eliminarNodo(Nodo *);
Nodo *minimo(Nodo *);
void reemplazar(Nodo *, Nodo *);
void destruir(Nodo *);

Nodo *crearNodo(string s, Nodo *padre)
{
    Nodo *nuevo = new Nodo();

    nuevo->valor=s;
    nuevo->der=NULL;
    nuevo->izq=NULL;
    nuevo->padre=padre;

    return nuevo;
}

void insertar(Nodo *&arbol, string s, Nodo *padre)
{
    if (arbol == NULL)
    {
        Nodo *nuevo = crearNodo(s, padre);
        arbol = nuevo;
    }
    else
    {
        string valorRaiz = arbol->valor;

        if(s<valorRaiz)
        {
            insertar(arbol->izq,s,arbol);
        }

        else
        {
            insertar(arbol->der,s,arbol);
        }

    }



}

void preorder(Nodo *ra)
{
    if (ra!=NULL)
    {
        fp<<"\n";
        fp<<ra->valor;
        fp<<"[label=\"<C0>|";
        fp<<ra->valor;
        fp<<"|<C1>\"];";
        preorder(ra->izq);
        preorder(ra->der);
    }


}

void ordenar(Nodo *ra)
{
    if(ra!=NULL)
    {

    fp<<"\n";

        if(ra->izq!=NULL)
        {
            fp<<ra->valor;
            fp<<":C0->";
            fp<<ra->izq->valor;
            fp<<";\n";
        }

        if(ra->der!=NULL)
        {
            fp<<ra->valor;
            fp<<":C1->";
            fp<<ra->der->valor;
            fp<<";\n";
        }

        ordenar(ra->izq);
        ordenar(ra->der);
    }
}

void preorder2(Nodo *raiz)
{
    if (raiz!=NULL)
    {
        cout<<raiz->valor<<endl;
        preorder2(raiz->izq);
        preorder2(raiz->der);
    }


}

Nodo *minimo(Nodo *arbol)
{
    if (arbol==NULL)
    {
        return NULL;
    }
    if (arbol->izq)
    {
        return minimo(arbol->izq);
    }
    else
    {
        return arbol;
    }
}

void eliminar(Nodo *arbol, string s)
{
    if (arbol == NULL)
    {
        return;
    }
    else if (s<arbol->valor)
    {
        eliminar(arbol->izq, s);
    }

    else if(s>arbol->valor)
    {
        eliminar(arbol->der,s);
    }

    else
    {
        eliminarNodo(arbol);
    }

}

void reemplazar(Nodo *arbol,Nodo *nuevoNodo)
{

        if (arbol->padre->izq)
        {
            if (arbol->valor == arbol->padre->izq->valor)
            {
                arbol->padre->izq = nuevoNodo;
            }

            else if (arbol->valor == arbol->padre->der->valor)
            {
                arbol->padre->der = nuevoNodo;
            }
        }

        else if (arbol->padre->der)
        {
            if (arbol->valor == arbol->padre->der->valor)
            {
                arbol->padre->der = nuevoNodo;
            }

            else if (arbol->valor == arbol->padre->izq->valor)
            {
                arbol->padre->izq = nuevoNodo;
            }

        }

    if (nuevoNodo)
    {
        nuevoNodo->padre = arbol->padre;
    }
}

void destruir(Nodo *nodo)
{
    nodo->izq=NULL;
    nodo->der=NULL;
    nodo->padre=NULL;
    nodo->valor="";

    delete nodo;
}

void eliminarNodo(Nodo *nodoEliminar)
{

    if(nodoEliminar->izq!=NULL && nodoEliminar->der!=NULL)
    {
        Nodo *menor = minimo(nodoEliminar->der);
        nodoEliminar->valor = menor->valor;
        eliminarNodo(menor);
    }

    else if(nodoEliminar->izq!=NULL)
    {
        reemplazar(nodoEliminar, nodoEliminar->izq);
        destruir(nodoEliminar);
    }

    else if (nodoEliminar->der!=NULL)
    {
        reemplazar(nodoEliminar, nodoEliminar->der);
        destruir(nodoEliminar);
    }
    else if (nodoEliminar->izq==NULL && nodoEliminar->der==NULL && nodoEliminar->padre==NULL)
    {
        nodoEliminar->der=NULL;
        nodoEliminar->izq=NULL;
        nodoEliminar->valor="";
        delete nodoEliminar;
        arbol=NULL;
    }
    else
    {
        reemplazar(nodoEliminar, NULL);
        destruir(nodoEliminar);
    }
}

void mostrar()
{

    fp.open("Graphiz.dot",ios::out);

    fp<<"digraph Programa {\n";
    fp<<"\tnode [shape = record, style=filled, width=0.7,height=0.2]\n";
    fp<<"\tsplines=false;\n";
    fp<<"\trankdir=TP\n";
    fp<<"\tcompound=true;\n";

    Nodo *raiz = NULL;

    raiz = arbol;



    if(raiz!=NULL)
    {
        preorder(raiz);
        raiz = arbol;
        ordenar(raiz);

    }
    else
    {
        fp<<"\n\tsubgraph Arbol{\nnode[shape=box]\nedge[dir=forward]\n";

        fp<<"\t\"Arbol Vacio\"";

        fp<<"\n\t}";
    }


    fp<<"\n}";
    fp.close();

    system("dot -Tpng Graphiz.dot -o Graphiz.png");
    system("xdg-open Graphiz.png");

}


void menu()
{
    int opcion;
    string entrada;

    do
    {
        cout<<"1. Insertar"<<endl;
        cout<<"2. Eliminar"<<endl;
        cout<<"3. Mostrar"<<endl;
        cout<<"0. Salir\n"<<endl;
        cin>>opcion;

        switch(opcion)
        {
            case 1:
                cout<<"\nEscribir valor para ingresar"<<endl;
                cin>>entrada;
                insertar(arbol, entrada, NULL);
                system("clear");
                menu();
                break;

            case 2:
                cout<<"\nEscribir valor a eliminar"<<endl;
                cin>>entrada;
                eliminar(arbol,entrada);
                system("clear");
                menu();
                break;

            case 3:
                mostrar();
                break;

            case 0:
                system("exit");
                break;

            default:
                system("clear");
                menu();
                break;
        }

    }

    while (opcion!=0);
}


int main()
{
    menu();
}
