/*
 * chui274.h
 *
 *  Created on: 23 mayo. 2018
 *      Author: Sofía Fernández Moreno
 */

#include "Bot.h"

#ifndef MANUPCBOT_H_
#define MANUPCBOT_H_


struct CamposTablero {

    GameState state;
    Move movimiento;

};
struct Nodo{
	int valor;
	Move movimiento;
};
class chui274:Bot {
private:
	
public:
	chui274();
	~chui274();

	
	//Algoritmos implementados
	//Inicialmente MiniMax
	Move MinMax(const GameState &state, int limite, const Player &jugador, int & valor);
	//Algoritmo PodaAlfaBeta, implementado sobre Algoritmo MiniMax
	int PodaAlfaBetaRecursivo(const GameState &state, int limite,const  Player &jugador, Move & movimiento, int alfa, int beta);
	

	
	void initialize();
	string getName();
	Move nextMove(const vector<Move> &adversary, const GameState &state);



	//Pruebas en clase e iniciales
	int heuristicaRaul(const GameState &state,const Player & jugador);
	int JugadasSeguidas(const GameState &state,const Player & jugador);
	int evaluaTablero(const GameState &state, const Player &jugadorActual);
	



	int heuristicaFinal(const GameState &state, const Player & jugador,int mejorValor);
	


};

#endif /* MANUPCBOT_H_ */
