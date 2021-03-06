//
//  Poligono.cpp
//  OpenGLTest
//
//  Created by Márcio Sarroglia Pinho on 18/08/20.
//  Copyright © 2020 Márcio Sarroglia Pinho. All rights reserved.
//
#include <iostream>
#include <fstream>
using namespace std;

#include "Poligono.h"

Poligono::Poligono()
{

}

void Poligono::insereVertice(Ponto p)
{
    Vertices.push_back(p);
}

void Poligono::insereVertice(Ponto p, int pos)
{
    if ((pos < 0) || (pos>Vertices.size()))
    {
        cout << "Metodo " << __FUNCTION__ << ". Posicao Invalida. Vertice nao inserido." << endl;
        return;
    }
    Vertices.insert(Vertices.begin()+pos, p);
}

Ponto Poligono::getVertice(int i)
{
    return Vertices[i];
}

void Poligono::pintaPoligono()
{
    glBegin(GL_POLYGON);
    for (int i=0; i<Vertices.size(); i++)
        glVertex3f(Vertices[i].x,Vertices[i].y,Vertices[i].z);
    glEnd();
}

void Poligono::desenhaPoligono()
{
    glBegin(GL_LINE_LOOP);
    for (int i=0; i<Vertices.size(); i++)
        glVertex3f(Vertices[i].x,Vertices[i].y,Vertices[i].z);
    glEnd();
}
void Poligono::desenhaVertices()
{
    glBegin(GL_POINTS);
    for (int i=0; i<Vertices.size(); i++)
        glVertex3f(Vertices[i].x,Vertices[i].y,Vertices[i].z);
    glEnd();
}
void Poligono::imprime()
{
    for (int i=0; i<Vertices.size(); i++)
        Vertices[i].imprime();
}
unsigned long Poligono::getNVertices()
{
    return Vertices.size();
}

void Poligono::obtemLimites(Ponto &Min, Ponto &Max)
{
    Max = Min = Vertices[0];

    for (int i=0; i<Vertices.size(); i++)
    {
        Min = ObtemMinimo (Vertices[i], Min);
        Max = ObtemMaximo (Vertices[i], Max);
    }
}

// **********************************************************************
//
// **********************************************************************
void Poligono::LePoligono(const char *nome)
{
    ifstream input;            // ofstream arq;
    input.open(nome, ios::in); //arq.open(nome, ios::out);
    if (!input)
    {
        cout << "Erro ao abrir " << nome << ". " << endl;
        exit(0);
    }
    cout << "Lendo arquivo " << nome << "...";
    string S;
    //int nLinha = 0;
    unsigned int qtdVertices;

    input >> qtdVertices;  // arq << qtdVertices

    for (int i=0; i< qtdVertices; i++)
    {
        double x,y;
        // Le cada elemento da linha
        input >> x >> y; // arq << x  << " " << y << endl
        if(!input)
            break;
        //nLinha++;
        insereVertice(Ponto(x,y));
    }
    cout << "Poligono lido com sucesso!" << endl;

}

void Poligono::getAresta(int n, Ponto &P1, Ponto &P2)
{
    P1 = Vertices[n];
    int n1 = (n+1) % Vertices.size();
    P2 = Vertices[n1];
}

void Poligono::desenhaAresta(int n)
{
    glBegin(GL_LINES);
        glVertex3f(Vertices[n].x,Vertices[n].y,Vertices[n].z);
        int n1 = (n+1) % Vertices.size();
        glVertex3f(Vertices[n1].x,Vertices[n1].y,Vertices[n1].z);
    glEnd();
}

Poligono Poligono::GeraConvexHull()
{
    Poligono convex;
    Ponto PMin, PMax;
    obtemLimites(PMin, PMax);

    // localiza o ponto inicial para gerar o convex
    Ponto Start;
    // procura o menor ponto em y e começa com produto escalar pro vetor (1,0)
    for(int i = 0; i < Vertices.size(); i++)
    {
        if(PMin.y == getVertice(i).y){
            Start = getVertice(i);
            break;
        }
    }

    Ponto VetorComparacao = Ponto (1.0f, 0.0f, 0.0f); // direita
    Ponto VetorTemp;
    Ponto Temp = Start;
    Ponto ProxPonto;

    do{
        if(Temp.x == PMin.x){
            VetorComparacao = Ponto (0.0f, -1.0f, 0.0f); // pra baixo -terceiro que deve acontecer
        } else if(Temp.y == PMax.y){
            VetorComparacao = Ponto (-1.0f, 0.0f, 0.0f); // pra esquerda -segundo que deve acontecer
        }
        else if(Temp.x == PMax.x){
            VetorComparacao = Ponto (0.0f, 1.0f, 0.0f); // pra cima -primeiro que deve acontecer
        }

        double ProdEsc = 0;
        for(int i = 0; i < Vertices.size(); i++)
        {
            // calcular o vetor unitario
            // 1. gerar um vetor do ponto inicial ate os outro:
            if( PontosIguais(Temp, getVertice(i)))// evitar divisão por zero
                exit;

            VetorTemp = operator-(Temp, getVertice(i));
            VetorTemp = VetorUnitario(VetorTemp);

            double TempProdEsc = ProdEscalar(VetorTemp, VetorComparacao);
            //cout << "ProdEsc: " << TempProdEsc << " | "<< ProdEsc << endl;
            if(TempProdEsc > ProdEsc){
                ProdEsc = TempProdEsc;
                ProxPonto = getVertice(i);
                //cout << "ProxPont: " << ProxPonto.x << " | "<< ProxPonto.y << endl;
            }


        }
        convex.insereVertice(Temp);
        Temp = ProxPonto;
    } while( PontosIguais(Temp, Start) == FALSE); // enquanto nao der a volta

    return convex;
}


