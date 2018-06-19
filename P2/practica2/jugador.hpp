#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <iostream>
#include <cmath>
#include <set>
#include <queue>
#include <list>
#include <algorithm>

struct estado {
  int fila;
  int columna;
  int orientacion;
  estado() : columna(0), fila(0), orientacion(0) {};
  estado(int y, int x, int brujula) : columna(x), fila(y), orientacion(brujula) {}


};
/*Para el A
Mientras queden acciones para evaluar saco y meto los que pueda desde el inicial
*/
struct Movimiento{
  
  estado posActual;
  list<Action> plan;//No tengo ninguna opcion, pero tendré una lista con los movimientos(con el inicial que acaba cuando esté vacio) y sacaré los más interesantes
  int h;//Para guardar la heuristica
  int g;//Para guardar plan.size
  int f;//suma de g+h
  
  
  
  Movimiento(int fila, int columna){
			posActual.fila = fila;
			posActual.columna = columna;
		}

		Movimiento(estado uno){
			posActual = uno;
		}

		Movimiento(){
			//Constructor vacío
		}
  
		void setFila(int fila){
			posActual.fila = fila;
		}

		void setColumna(int columna){
			posActual.columna = columna;
		}

		void setBrujula(int orientacion){
			posActual.orientacion = orientacion;
		}

		void setEstado(estado uno){
			posActual = uno;
		}

		
		estado getEstado() const{
			return posActual;
		}

		

		int getFila() const{
			return posActual.fila;
		}

		int getColumna() const{
			return posActual.columna;
		}

		int getBrujula() const{
			return posActual.orientacion;
		}

		void addAccion(Action una){
			plan.push_back(una);
		}

		list<Action> getPlan() const{
			return plan;
		}
    
};
	;


class ComportamientoJugador : public Comportamiento {
  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      // Inicializar Variables de Estado
      //fil = col = 99;
      brujula = 0; // 0: Norte, 1:Este, 2:Sur, 3:Oeste
      destino.fila = -1;
      destino.columna = -1;
      destino.orientacion = -1;
      ultimaAccion = actIDLE;
      hayPlan = false;
      esta_situado = false;
      rellenado = false;
      falloPlan = false;
      veo_PK = false;


/*       for(int i=0; i<200; i++)
            for(int j=0; j<200; j++)
                mapaAuxiliar[i][j]=0; */
      
      mapaAuxiliar.resize(mapaResultado.size()*2);
      fil = col = mapaAuxiliar.size()/2;
      for(int i=0; i<mapaAuxiliar.size(); i++){
            mapaAuxiliar[i].resize(mapaResultado.size()*2);
            for(int j=0; j<mapaAuxiliar[0].size(); j++)
                mapaAuxiliar[i][j]='?'; 
      }
      mapaPrioridades.resize(mapaResultado.size()*2);
      
      for(int i=0; i<mapaPrioridades.size(); i++){
            mapaPrioridades[i].resize(mapaResultado.size()*2);
            for(int j=0; j<mapaPrioridades[0].size(); j++)
                mapaPrioridades[i][j]=0; 
      }


      
      
    
    }
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      // Inicializar Variables de Estado
      //fil = col = 99;
      brujula = 0; // 0: Norte, 1:Este, 2:Sur, 3:Oeste
      destino.fila = -1;
      destino.columna = -1;
      destino.orientacion = -1;
      ultimaAccion = actIDLE;
      hayPlan = false;
      esta_situado = false;
      rellenado = false;
      falloPlan = false;
      veo_PK = false;


      /* for(int i=0; i<200; i++)
            for(int j=0; j<200; j++)
                mapaAuxiliar[i][j]=0; */
      mapaAuxiliar.resize(mapaResultado.size()*2);
      fil = col = mapaAuxiliar.size()/2;
      for(int i=0; i<mapaAuxiliar.size(); i++){
            mapaAuxiliar[i].resize(mapaResultado.size()*2);
            for(int j=0; j<mapaAuxiliar[0].size(); j++)
                mapaAuxiliar[i][j]='?'; 
      }

      mapaPrioridades.resize(mapaResultado.size()*2);
      
      for(int i=0; i<mapaPrioridades.size(); i++){
            mapaPrioridades[i].resize(mapaResultado.size()*2);
            for(int j=0; j<mapaPrioridades[0].size(); j++)
                mapaPrioridades[i][j]=0; 
      }
      
        

      

    }
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    void VisualizaPlan(const estado &st, const list<Action> &plan);

    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}

    
    

    void delante(Movimiento &sigMovimiento);
    void izquierda(Movimiento &sigMovimiento);
    void derecha(Movimiento &sigMovimiento);
    //Funciones para establecer la orientacion del estado
    estado avanza(estado pos);
    estado girarIzq(estado pos);
    estado girarDch(estado pos);
    
    bool puedoPasar(estado pos);
   
    bool puedoAvanzar(char valor);

    bool obstaculo_sensor(unsigned char casilla);

    bool puedoAvanzar2(char casilla);

    
    //Funcion que calcula la heuristica a resolver
    int heuristica_manhattan(estado posInicio, estado posDestino);
    
   

  private:
    // Declarar Variables de Estado
    int fil, col, brujula;
    //Filas y columnas auxiliares para el mapaResultado
    int fil_anterior, col_anterior;
    estado destino;
    list<Action> plan;
    estado actual;

    // Nuevas Variables de Estado
    Action ultimaAccion;
    bool hayPlan;
    bool esta_situado;
    int ultPosF,ultPosC;
    char superficiedelante;
    char terrenodelante;
    Sensores sensor;
    bool rellenado;
    bool falloPlan;
    bool giros_derecha;
    bool veo_PK;

    char encima;
    //MapaNuevos
    vector<vector <unsigned char> > mapaAuxiliar;
    //Mapa miguitas
    vector<vector <unsigned int> > mapaPrioridades;
    //Mapa
    
    
    

   
    


    

    bool pathFinding(const estado &origen, const estado &destino, list<Action> &plan);
    void PintaPlan(list<Action> plan);
   



/******************************ACTUALIZACION MAPA***********************/
void ActualizarMapa(const Sensores &sensores, vector<vector <unsigned char> > &mapaNuevo,vector<vector <unsigned int> > &miguita);
 
/******************************PINTADO DEL MAPA*************************/
void PintarMapa(vector<vector <unsigned char> > mapaNuevo);

/***********FUNCION PARA EXPLORAR NIVEL3**********/
void exploracion(int orientacion,Sensores sensores,vector< vector<unsigned int> > &mapaNuevo,list<Action> &plan);

/**************FUNCION PARA REORIENTAR***********/
void Reorientar(Sensores &sensores);

/*****************FUNCION PARA BUSCAR LOS PK******/
void busquedaPK(Sensores sensores, list<Action> &plan);

};

#endif
