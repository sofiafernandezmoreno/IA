/*
 * chui274.cpp
 *
 *  Created on: 23 mayo. 2018
 *      Author: Sofía Fernández Moreno
 */

#include "chui274.h"

#include <string>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <math.h>
#include <stdlib.h>
#include <algorithm>
using namespace std;


chui274::chui274() {
	// Inicializar las variables necesarias para ejecutar la partida
	

}

chui274::~chui274() {
	// Liberar los recursos reservados (memoria, ficheros, etc.)
}

void chui274::initialize() {
	// Inicializar el bot antes de jugar una partida
}

string chui274::getName() {
	return "chui274"; // Sustituir por el nombre del bot
}

/******************************************************************************/

/*

			PRUEBAS INICIALES (HEURISTICAS Y ALGORITMOS)
*/

/**
Función que evalua el tablero y asigna una puntuación a cada jugador, dependiendo las semillas recogidas
PRIMERAS PRUEBAS BASICAS
**/

int chui274::evaluaTablero(const GameState &state,const Player &jugadorActual){
	int misSemillas, semillasContrario;
	Player jugadorContrincante;
	misSemillas =  semillasContrario = 0;
	if(jugadorActual == J1){//Evaluamos que el jugadorActual de verdad sea el que se encuentra jugador, es decir, el jugadorActual
		jugadorContrincante = J2;
	}
	else{
		jugadorContrincante = J1;
	}
	
	
	
	for (int i= 1; i<=6; i++) {
		misSemillas += state.getSeedsAt(jugadorActual, (Position) i);
		semillasContrario += state.getSeedsAt(jugadorContrincante, (Position) i);

	}
	
	
	return misSemillas - semillasContrario;

}





int chui274::JugadasSeguidas(const GameState &state,const Player & jugador){
	int h=0;
	if(state.isFinalState() || state.getCurrentPlayer()!= jugador){
		return h;
	}
	for (int i= 1; i<=6; i++){
		if(state.getSeedsAt(jugador, (Position) i)>0){
			GameState next = state.simulateMove((Move) i);
			if(next.getCurrentPlayer()==jugador){
				h = JugadasSeguidas(state,jugador)+1;
			}
		}

	}
	return h;
}






/*
	Función MiniMax
	Función inicial, desde esta sacaré el PodaAlfaBeta, además de probar la heurística inicial
*/
Move chui274::MinMax(const GameState &state, int limite,const Player &jugador, int & valor){
	//El limite será el maximo de profundidad de la hoja del arbol
	//Valor -1 evalúa todo
	
	Move movimiento = M_NONE;
	int valorSigMovimiento;
	Move sigMovimiento;
	if((limite > 0 || limite <= -1 ) && !state.isFinalState()){//Comprobamos que estamos en un nodo max
		if(state.getCurrentPlayer() == jugador){ //Estamos en un nodo MAX
			//Buscamos el valor más grande
			valor = std::numeric_limits<int>::min();//Lo vamos a inicilizar a -infinito
			
			for(int i = 0; i <7; i++){
				GameState sigEstado = state.simulateMove((Move) i);
				sigMovimiento = MinMax(sigEstado, limite - 1, jugador, valorSigMovimiento);
				if(valor < valorSigMovimiento){
					valor = valorSigMovimiento;
					sigMovimiento = (Move) i;
					movimiento = sigMovimiento;
				}
			}


		}
		else{
			//No estamos en un nodo MAX, por lo que estamos en un nodo MIN
			//Buscamos el valor más pequeño
			//Por lo que lo vamos a inicializar a infinito
			valor = std::numeric_limits<int>::max();//Lo vamos a inicilizar a infinito
			
			for(int i = 0; i <7; i++){
				GameState sigEstado = state.simulateMove((Move) i);
				sigMovimiento = MinMax(sigEstado, limite - 1, jugador, valorSigMovimiento);
				if(valor > valorSigMovimiento){
					valor = valorSigMovimiento;
					sigMovimiento = (Move) i;
					movimiento = sigMovimiento;
				}
			}

		}
	}//Caso general
	else{//Evaluamos el tablero
		valor = evaluaTablero(state,jugador);

	}
	return movimiento;

}
int chui274::heuristicaRaul(const GameState &state,const Player &jugador){
		return state.getScore(jugador);
	
}
/******************************************************************************/
/*

	FIN PRUEBAS INICIALES
*/
/******************************************************************************/

/******************************************************************************/
/*
	PRACTICA 3 : MANCALABOT CHUI274 
*/

/******************************************************************************/
/*********************************************************************/
/*
	HEURISTICA FINAL: Explicada en memoria
*/
/*
	Función donde calculamos la heuristica de nuestro Mancala
*/
int chui274::heuristicaFinal(const GameState &state,const Player & jugador,int mejorValor) {
	
	int movimientos[6] = {0};
	int puntuacionJugador = state.getScore(jugador);
	int i = 1;
	while(i<=6){//Recorro los posibles movimientos de cada jugador
	
		if (state.getSeedsAt(jugador, (Position) i) == i)//Si cada valor i coincide con las semillas
			movimientos[i-1] = puntuacionJugador + 1 + i;
		i++;
	}
	int j = 0;
	while(j<6){//Recorro las posiciones del tablero,incluyendo el granero
	
		if (movimientos[j] > mejorValor)//Calculamos el mejor valor, ese mejor valor se le asignará el numero de movimientos obtenidos
			mejorValor = movimientos[j];
		j++;
	}

	if (mejorValor == 0){//devolvemos el granero
		
		return puntuacionJugador;
	}
	else
		return mejorValor;//Valor actualizado 
	
	
}

/*
Poda alfa-beta

Se aplica sobre el MiniMax.
-No afecta al resultado del juego, pero nos permite con el mismo esfuerzo explorar un árbol de mayor profundidad
alfa = valor de la mejor opcion encontrada para el jugador MAX
"MAX evitará cualquier movimiento que tenga un valor "v" peor que alfa"
beta = valor de la mejor opcion encontrada para el jugador MIN
"MIN evitará cualquier movimiento que tenga un valor "v" peor que beta"


*/

int chui274::PodaAlfaBetaRecursivo(const GameState &state, int limite,const Player &jugador, Move & movimiento, int alfa, int beta){
	//El limite será el maximo de profundidad de la hoja del arbol
	//Valor -1 evalúa todo
	
	Move bestMove;
	int valor;
	
	//if((limite > 0 || limite <= -1 ) && !state.isFinalState()){//Comprobamos que estamos en un nodo max
		if(limite == 0 || state.isFinalState()){//Evaluamos el tablero para cuando la profundidad es 0, es decir, el ultimo nodo
		//Player player = this->getPlayer();
			int mejorValor = heuristicaFinal(state,jugador,0);
			//cerr<<"Valor heuristica"<<mejorValor<<endl;
			return mejorValor;
			//return heuristicaFinal(state,jugador);
			
			
			
		
		}
		if (jugador == state.getCurrentPlayer()) {//Me encuentro en un nodo MAX
			//Maximizo soy el jugador actual y quiero maximizar el resultado
    		// Si es la segunda, se evalua al rival y quiero minimizar el resultado
			//Buscamos el valor más grande
			
			
			
			for(int i = 1; i <=6; i++){
				//hay que calcular todos los posibles movimientos que se pueden hacer.
				// para cada hijo de nodo
				// Se recorren todos los posibles movimientos, teniendo en cuenta
				// que ahora la profundidad valdrá profundidad-1 al bajar un nivel
				// y que el jugador quiere maximizar y lo que se busca es minimizar
				if(state.getSeedsAt(jugador, (Position)i)!=0){//Comprobar si hay semillas
					//assert(state.getSeedsAt(jugador,(Position)i)>0);
					GameState sigEstado = state.simulateMove((Move) i);
					
                	valor = PodaAlfaBetaRecursivo(sigEstado, limite - 1, jugador, bestMove,alfa,beta);
					
					// alpha valdrá el mayor valor entre la alpha que ya se tiene o el
            		// resultado de evaluar el movimiento
					if(alfa < valor){
						//cerr<<"alfa<valor"<<endl;
						alfa = valor;
						movimiento = (Move) i;
						
					}
					if(beta <= alfa){//Podamos
						//cerr<<"Podamos"<<endl;
						//(* Poda β *)
						return beta;
						
					}
				

					
				}
			}
			return alfa;

		}
		else{//Estamos en un nodo MIN
			//No estamos en un nodo MAX, por lo que estamos en un nodo MIN
			//Buscamos el valor más pequeño
			
			
			for(int i = 1; i <=6; i++){
				// para cada hijo de nodo
				// Se recorren todos los posibles movimientos, teniendo en cuenta
				// que ahora la profundidad valdrá profundidad-1 al bajar un nivel
				// y que el jugador quiere minimizar y lo que se busca es maximizar
				
					GameState sigEstado = state.simulateMove((Move) i);
					
                	valor = PodaAlfaBetaRecursivo(sigEstado, limite - 1, jugador, bestMove,alfa,beta);
					
					// beta valdrá el menor valor entre el beta que ya se tiene o el
            		// resultado de evaluar el movimiento
					if(beta > valor){
						//cerr<<"beta > valor"<<endl;
						beta = valor;
						movimiento = (Move) i;
						
					}
					if(beta<=alfa){//Podamos
					// si β≤α
					// beta siempre tiene que ser mayor que alpha. Si es menor o igual,
					// ya se sabe que no se debe seguir por esta rama y se poda alpha
						//cerr<<"Podamos"<<endl;
						return alfa;
						
					}
				

					
				
			}
			return beta;

		}
	//}//Caso general
	
	
	
	
}
 
    
	
Move chui274::nextMove(const vector<Move> &adversary, const GameState &state) {
	
	
	
	//Aquí plantearemos la poda alfa-beta
	int alfa,beta;
	alfa = std::numeric_limits<int>::min();
	beta = std::numeric_limits<int>::max();
	//Gamestate-->Representacion del tablero
	//Move-->Movimientos posibles
	 Player turno= this->getPlayer();//Jugador yo
	 long timeout= this->getTimeOut();//Tiempo limite
	 //int valor;
	 Move movimiento = M_NONE;


	//Pruebas para comprobar que funciona evaluarTablero
	//Vamos a meter una semilla y evaluamos cual es el resultado

	
	GameState estado1;

	//Jugador2
	//25
	// estado1.piezas[1][0]=7;
	// estado1.piezas[1][1]=0;
	// estado1.piezas[1][2]=1;
	// estado1.piezas[1][3]=3;
	// estado1.piezas[1][4]=5;
	// estado1.piezas[1][5]=7;
	// estado1.piezas[1][6]=2;


	//Jugador1
	//25
	// estado1.piezas[0][0]=7;
	// estado1.piezas[0][1]=0;
	// estado1.piezas[0][2]=1;
	// estado1.piezas[0][3]=3;
	// estado1.piezas[0][4]=5;
	// estado1.piezas[0][5]=7;
	// estado1.piezas[0][6]=2;


	int prof = 10;
	



	int valor = PodaAlfaBetaRecursivo(state,prof, turno,movimiento,alfa,beta);
	cerr<<"Profundidad--> "<<prof<<" Valor Poda Alfa-Beta--> "<<valor<<endl; 
	return movimiento;
	

}











  

