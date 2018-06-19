#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"


#include <iostream>
#include <cmath>
#include <set>
#include <queue>
#include <list>
#include <limits>
#include <cassert>
using namespace std;



/*

///////////
//
//		////////  \\   //    \\  //
/////	//	  //  \\   //     \\//  
//		////////  \\   //	  //\\	
//		//    //  \\   //    //  \\
//      //    //   /////    //    \\

*/
estado ComportamientoJugador::avanza(estado pos){
	estado sig;
	if(pos.orientacion==0){	//Norte
		sig.fila=pos.fila - 1;
		sig.columna=pos.columna;

	}
	else if(pos.orientacion==1){ //Este
		sig.fila=pos.fila;
		sig.columna=pos.columna +1;
	}
	else if(pos.orientacion==2){	//sur
		sig.fila=pos.fila+1;
		sig.columna=pos.columna;
	}
	else if(pos.orientacion==3){	//Oeste
		sig.fila=pos.fila;
		sig.columna=pos.columna-1;

	}
	sig.orientacion=pos.orientacion; ///la orientacion no cambia
	return sig;

}

bool ComportamientoJugador::puedoPasar(estado pos){
	bool encontrado = false;
			
	//Para comprobar que no se sale del mapa
	bool limiteFila = (pos.fila<mapaResultado.size())&&(pos.fila>0);
	bool limiteColumna = (pos.columna<mapaResultado[0].size())&&(pos.columna>0);
	
	bool sueloarenoso = (mapaResultado[pos.fila][pos.columna]=='T');
	bool suelopedregoso = (mapaResultado[pos.fila][pos.columna]=='S');
	bool PK = (mapaResultado[pos.fila][pos.columna]=='K');
	
	bool suelos = (sueloarenoso || suelopedregoso || PK);
	bool limites = (limiteFila || limiteColumna);
	
	if(suelos && limites){
		//cout<<"Tengo "<<mapaResultado[pos.fila][pos.columna]<<" algo delante puedoPasar\n";
		encontrado = true;
	}
	
	
	return encontrado;
}



estado ComportamientoJugador::girarIzq(estado pos){
	estado posfutura = pos;
	posfutura.orientacion =(posfutura.orientacion + 3)%4;
	posfutura.fila = pos.fila;
	posfutura.columna = pos.columna;
	return posfutura;
}
estado ComportamientoJugador::girarDch(estado pos){
	estado posfutura = pos;
	posfutura.orientacion =(posfutura.orientacion + 1)%4;
	posfutura.fila = pos.fila;
	posfutura.columna = pos.columna;
	return posfutura;
}



//Funcion para calcular la heuristica de manhattan
int ComportamientoJugador::heuristica_manhattan(estado posInicio, estado posDestino){
	int fil_posInicio=posInicio.fila;
	int col_posInicio=posInicio.columna;

	int fil_posDestino=posDestino.fila;
	int col_posDestino=posDestino.columna;

	int distanciafila=abs(fil_posDestino - fil_posInicio);
	int distanciacolumna=abs(col_posDestino - col_posInicio);


	return distanciafila+distanciacolumna;

}
bool ComportamientoJugador::puedoAvanzar(char valor){
	bool encontrado = false;
	if(valor == 'S' || valor == 'T' || valor == 'K'){
		//cout<<"Tengo "<<valor<<" algo delante puedoAvanzar\n";
		encontrado = true;
	} 
	else{
		//cout<<"No puedo avanzar "<<valor<<endl;
	}
	return encontrado;
					
}

bool ComportamientoJugador::obstaculo_sensor(unsigned char casilla){
	bool solucion = false;
	if (casilla == 'A' && casilla == 'B' && casilla == 'D' && casilla == 'P' && casilla == 'M'){
		///cout<<"Tengo algo delante obstaculo_sensor\n";
		solucion = true;
	}
	return solucion;
}

bool ComportamientoJugador::puedoAvanzar2(char casilla){
	bool solucion = false;
	if (casilla == 'A' || casilla == 'B' || casilla == 'D' || casilla == 'P' || casilla == 'M'){
		//cout<<"Tengo algo delante puedoAvanzar2\n";
		solucion = true;
	}
	return solucion;
}


void ComportamientoJugador::delante(Movimiento &sigMovimiento){
		sigMovimiento.h = heuristica_manhattan(sigMovimiento.posActual,destino);
		sigMovimiento.g = sigMovimiento.plan.size();
		sigMovimiento.f = sigMovimiento.h + sigMovimiento.g;
		sigMovimiento.plan.push_back(actFORWARD);
}
void ComportamientoJugador::izquierda(Movimiento &sigMovimiento){
	sigMovimiento.h = heuristica_manhattan(sigMovimiento.posActual,destino);
	sigMovimiento.g = sigMovimiento.plan.size();
	sigMovimiento.f = sigMovimiento.h + sigMovimiento.g;
	sigMovimiento.plan.push_back(actTURN_L);
}
void ComportamientoJugador::derecha(Movimiento &sigMovimiento){
	sigMovimiento.h = heuristica_manhattan(sigMovimiento.posActual,destino);
	sigMovimiento.g = sigMovimiento.plan.size();
	sigMovimiento.f = sigMovimiento.h + sigMovimiento.g;
	sigMovimiento.plan.push_back(actTURN_R);
}



/*

       
****** **** ******  *******
*    * *  * *       *     *
*    * **** ******  *******
*    * *    *       * *
****** *    ******  *  *
*/
bool operator<(const Movimiento &m1,const Movimiento &m2){//Para comparar el de menor costeF
	return m1.f>m2.f;
}
  bool operator<(const estado &e1, const estado &e2){
	 if(e1.fila < e2.fila) return true;
	 if(e1.fila == e2.fila && e1.columna < e2.columna) return true;
	 if(e1.fila == e2.fila && e1.columna == e2.columna && e1.orientacion < e2.orientacion) return true;
	 return false;

}




    


/*
Algoritmo de búsqueda A*

-ABIERTOS contiene el nodo inicial, CERRADOS está vacío
-Comienza un ciclo que se repite hasta que se encuentra 
solucion o hasta que ABIERTOS queda vacío.
	*Seleccionar el mejor nodo de ABIERTOS
	*Si es un nodo objetivo(destino) terminar
	*En otro caso se expande dicho nodo
	*Para cada uno de los nodos sucesores
		.Si está en ABIERTOS insertarlo manteniendo la información del mejor padre
		.Si está en CERRADOS insertarlo manteniendo la informacion del mejor padre y actualizar la informacion de los descendientes.
		.En otro caso, insertarlo como un nodo nuevo

Si nuestra heuristica(h(n)) es admisible, A* encontrará el óptimo
g(n)=coste hasta n
h(n)=heuristica(estimacion del coste desde el nodo en el que estamos hasta el final) del estado inicial(podremos usar la heuristica de manhattan calculo de distancias)
f(n)=g(n)+h(n)
Coste=cantidad de acciones que hemos tenido que realizar(sería el size del plan)

Para elegir el siguiente nodo miraremos el coste y además el que tenga mejor h(n)
*/


 
	

  bool ComportamientoJugador::pathFinding(const estado &origen, const estado &destino, list<Action> &plan) {
	Movimiento actual,sigMovimiento;
	
	
	priority_queue<Movimiento> abiertos;//Se puede usar una priority_queue
	set<estado>cerrados;
	actual.posActual = origen;
	
	
	

	plan.clear();
	
	
	
	

	//ABIERTOS Contiene el nodo inicial
	abiertos.push(actual);
	
	//Comenzamos ciclo
	while(!abiertos.empty() ){
		actual = abiertos.top();
		abiertos.pop();
			
			//Avanzar
			sigMovimiento = actual;
			sigMovimiento.posActual = avanza(sigMovimiento.posActual);
						
			if(cerrados.find(sigMovimiento.posActual) == cerrados.end()){  //miramos si está en cerrados
					
					

					
					if(puedoAvanzar(mapaResultado[sigMovimiento.posActual.fila][sigMovimiento.posActual.columna])){
						delante(sigMovimiento);

						if((sigMovimiento.posActual.fila == destino.fila) && (sigMovimiento.posActual.columna == destino.columna)){
							cout<<"He encontrado un plan"<<endl;
							plan=sigMovimiento.plan;
							return true;//Si hemos llegado al destino, hemos encontrado un camino o plan
						}
						
						abiertos.push(sigMovimiento);
						
						}
				
					

				}

				//Izquierda
				sigMovimiento = actual;//Hacemos una copia
				sigMovimiento.posActual = girarIzq(sigMovimiento.posActual);
					
				if(cerrados.find(sigMovimiento.posActual)==cerrados.end()){    //miramos si está en cerrados
						izquierda(sigMovimiento);
						abiertos.push(sigMovimiento);
							

						
						
					
				}
				//Derecha
				sigMovimiento = actual;
				sigMovimiento.posActual = girarDch(sigMovimiento.posActual);
				
				if(cerrados.find(sigMovimiento.posActual) == cerrados.end()){  	
						derecha(sigMovimiento);
						abiertos.push(sigMovimiento);
						
						
				}
				cerrados.insert(actual.posActual);
							
			
		}
		
	
	
	
	cout<<"Me salgo del bucle y no funciono"<<endl;

	return false;
}


 
 
Action ComportamientoJugador::think(Sensores sensores) {
	Action sigAccion;
	encima = sensores.terreno[0];
	terrenodelante = sensores.terreno[2];
	superficiedelante = sensores.superficie[2];
	
	
	//cout<<"Comenzamos\n";
	//Para reiniciar/resetear
	
	if(sensores.reset){
		
		hayPlan = false;
		plan.clear();
		//veo_PK = false;
		
	}
	
	if (sensores.mensajeF != -1 ){

		//Reorientar
		//if(esta_situado){
		//Reorientar(sensores);
		
		//}
		
		fil = sensores.mensajeF;
		col = sensores.mensajeC;
		

		
		esta_situado = true;
		//veo_PK = true;
		//cout<<"Me coloco\n";
		//cout<<"fil "<<fil<<"col "<<col<<endl;
	}
	
	
	 // Actualizar el movimiento(posicion desde la ultima accion que se realizó)
	switch (ultimaAccion){

		case actTURN_R: 

		brujula = (brujula+1)%4;

		 
		
		break;
		case actTURN_L: 
		brujula = (brujula+3)%4;

		
		 
		break;
		case actFORWARD:
		if(!sensores.colision){
			switch (brujula){
				case 0: fil--; break;//Norte
				case 1: col++; break;//Este
				case 2: fil++; break;//Sur
				case 3: col--; break;//Oeste
			}
		}
		break;
			cout<<"--------------------------\n";
			cout<<"Movimiento jugador"<<endl;
			cout << "fil: " << fil << "  col: " << col << " Or: " << brujula << endl;
			cout<<"--------------------------\n";
			cout<<"-----DATOS DEL DESTINO-------\n";
			cout<<"Destino fila: "<<sensores.destinoF<<" Destino columna: "<<sensores.destinoC<<endl;
			cout<<"--------------------------\n";
			cout<<"-------Lo que veo delante------\n";
			cout<<"Veo esto : "<< superficiedelante <<endl;
	} 
	
	
 	actual.fila = fil;
	actual.columna = col;

	actual.orientacion = brujula;
	


	/* //Actualizaremos el mapa con la información de los sensores
	cout<<"Procedemos a actualizar el mapa con la información de los sensores dependiendo de si estamos orientados"<<endl;
	if(esta_situado){
		ActualizarMapa(sensores,mapaResultado,mapaPrioridades);
		
		//Pintamos el mapa
		PintarMapa(mapaResultado);
	}
	else{
		ActualizarMapa(sensores,mapaAuxiliar,mapaPrioridades);
		//Pintamos el mapa
		PintarMapa(mapaAuxiliar);
	} */

		

	// Determinar si ha cambiado el destino desde la ultima planificacion
	if (hayPlan && (sensores.destinoF != destino.fila or sensores.destinoC != destino.columna)){
		cout << "El destino ha cambiado\n";
		
		hayPlan = false;
		//plan.clear();
	}
	//cout<<"Pasamos a elaborar el plan"<<endl;
	
	// Determinar si tengo que construir un plan
	 

/*
	 if(!esta_situado && !hayPlan && !veo_PK){
		plan.clear();
		busquedaPK(sensores,plan);
		//hayPlan = true;
		//assert(plan.size()>0);
		
		//VisualizaPlan(actual, plan);
		 if(hayPlan){
			
			char c;
			cout << "Pulsa una tecla para pintar plan busquedaPK ...\n";
			PintaPlan(plan);
			cin >> c;
		}	 
	} 
	if(!esta_situado && !hayPlan){
			plan.clear();
			exploracion(brujula,sensores, mapaPrioridades,plan);
			//hayPlan = true;
			assert(plan.size()>0);
			 if(hayPlan){
				
				char c;
				cout << "Pulsa una tecla para pintar plan exploracion ...\n";
				PintaPlan(plan);
				cin >> c;
			} 	
	}
*/
if (!hayPlan && esta_situado ){

		destino.fila = sensores.destinoF;
		destino.columna = sensores.destinoC;
		
	
		hayPlan = pathFinding(actual,destino,plan);
		
		VisualizaPlan(actual,plan);
		//PintaPlan(plan);
			 
		//PintaPlan(plan);
		
		
	}

	
	
	
	
/* 	if(fil - 1 == destino.fila && col == destino.columna){
			plan.push_back(actFORWARD);
	} */
	

 	 if(hayPlan && plan.size()>0){
		//cout<<"Tengo plan y voy a ejecutar"<<endl;
		sigAccion = plan.front();
		if(sigAccion == actFORWARD && superficiedelante == 'a'){
			
			cout<<"OHH un ALDEANO me espero y me voy en cuanto no lo tenga delante"<<endl;
			sigAccion = actIDLE;
		}
		else{
			//cout<<"¿Podre?"<<endl;
			if(!puedoAvanzar(sensores.terreno[2]) && sigAccion == actFORWARD){
				
				sigAccion = actIDLE;
				
			}
			else{
			
			plan.erase(plan.begin());
			}
		}
	}  
		
		

	
	
	

	
	ultimaAccion = sigAccion;
	return sigAccion;
	

}

void AnularMatriz(vector<vector<unsigned char> > &m){
	for (int i=0; i<m[0].size(); i++){
		for (int j=0; j<m.size(); j++){
			m[i][j]=0;
		}
	}
}

void ComportamientoJugador::VisualizaPlan(const estado &st, const list<Action> &plan){
  AnularMatriz(mapaConPlan);
	estado cst = st;

	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			switch (cst.orientacion) {
				case 0: cst.fila--; break;
				case 1: cst.columna++; break;
				case 2: cst.fila++; break;
				case 3: cst.columna--; break;
			}
			mapaConPlan[cst.fila][cst.columna]=1;
		}
		else if (*it == actTURN_R){
			cst.orientacion = (cst.orientacion+1)%4;
		}
		else {
			cst.orientacion = (cst.orientacion+3)%4;
		}
		it++;
	}
}
void ComportamientoJugador::PintaPlan(list<Action> plan) {
	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			cout << "A ";
		}
		else if (*it == actTURN_R){
			cout << "D ";
		}
		else if (*it == actTURN_L){
			cout << "I ";
		}
		else {
			cout << "- ";
		}
		it++;
	}
	cout << endl;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}





/*
Actualizacion del mapa
Forma del vector sensores.terreno
       9
    4 10
  1 5 11
0 2 6 12
  3 7 13
    8 14
	  15
	  
estado actual, sig;
miguita[actual.fila][actual.col]++;
sig2 = avanzar(actual);
if(pos.fila>=0 && pos.fila<otromapa.size() && pos.columna>=0 && pos.columna<otromapa.size()){
	otromapa[pos2.fila][pos2.columna] = sensores.terreno[2];
}
if(otromapa[pos2.fila][pos2.columna]=='B' || .....){
	miguita[pos2.fila][pos2.columna]=numerics_limits<int>::max();
}
sig1 = avanzar(girarIzq(avanzar(sig2)));
sig3 = avanzar(girarDch(avanzar(sig2)))




mapaAux[sig1.fila][sig1.columna] = sensores.terreno[1];
mapaAux[sig2.fila][sig2.columna] = sensores.terreno[2];
mapaAux[sig1.fila][sig1.columna] = sensores.terreno[1];


*/



void ComportamientoJugador::ActualizarMapa(const Sensores &sensores, vector<vector <unsigned char> > &mapaNuevo, vector<vector <unsigned int> > &miguita) {

	
	mapaNuevo[fil][col] = sensores.terreno[0];
	miguita[fil][col]++;
	
	switch(brujula){
		case 0:
		if(fil-1 > 0 && col-1 > 0){
			// if (mapaNuevo[fil-1][col-1] == '?')
			mapaNuevo[fil-1][col-1] = sensores.terreno[1];
			if(!puedoAvanzar(mapaNuevo[fil-1][col-1])){
				miguita[fil-1][col-1] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil-1 > 0){
			// if (mapaNuevo[fil-1][col] == '?')
			mapaNuevo[fil-1][col] = sensores.terreno[2];
			if(!puedoAvanzar(mapaNuevo[fil-1][col])){
				miguita[fil-1][col] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil-1 > 0 && col+1 < mapaNuevo[0].size()){
			// if (mapaNuevo[fil-1][col+1] == '?')
			mapaNuevo[fil-1][col+1] = sensores.terreno[3];
			if(!puedoAvanzar(mapaNuevo[fil-1][col+1])){
				miguita[fil-1][col+1] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil-2 > 0 && col-2 > 0){
			// if (mapaNuevo[fil-2][col-2] == '?')
			mapaNuevo[fil-2][col-2] = sensores.terreno[4];
			if(!puedoAvanzar(mapaNuevo[fil-2][col-2])){
				miguita[fil-2][col-2] = std::numeric_limits<int>::max();
			}
				
		}
		if(fil-2 > 0 && col-1 > 0){
			// if (mapaNuevo[fil-2][col-1] == '?')
			mapaNuevo[fil-2][col-1] = sensores.terreno[5];
			if(!puedoAvanzar(mapaNuevo[fil-2][col-1])){
				miguita[fil-2][col-1] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil-2 > 0){
			// if (mapaNuevo[fil-2][col] == '?')
			mapaNuevo[fil-2][col] = sensores.terreno[6];
			if(!puedoAvanzar(mapaNuevo[fil-2][col])){
				miguita[fil-2][col] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil-2 > 0 && col+1 < mapaNuevo[0].size()){
			// if (mapaNuevo[fil-2][col+1] == '?')
			mapaNuevo[fil-2][col+1] = sensores.terreno[7];
			if(!puedoAvanzar(mapaNuevo[fil-2][col+1])){
				miguita[fil-2][col+1] = std::numeric_limits<int>::max();
			}
				
		}
		if(fil-2 > 0 && col+2 < mapaNuevo[0].size()){
			// if (mapaNuevo[fil-2][col+2] == '?')
			mapaNuevo[fil-2][col+2] = sensores.terreno[8];
			if(!puedoAvanzar(mapaNuevo[fil-2][col+2])){
				miguita[fil-2][col+2] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil-3 > 0 && col-3 > 0){
			// if (mapaNuevo[fil-3][col-3] == '?')
			mapaNuevo[fil-3][col-3] = sensores.terreno[9];
			if(!puedoAvanzar(mapaNuevo[fil-3][col-3])){
				miguita[fil-3][col-3] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil-3 > 0 && col-2 > 0){
			// if (mapaNuevo[fil-3][col-2] == '?')
			mapaNuevo[fil-3][col-2] = sensores.terreno[10];
			if(!puedoAvanzar(mapaNuevo[fil-3][col-2])){
				miguita[fil-3][col-2] = std::numeric_limits<int>::max();
		
			}
		}
		if(fil-3 > 0 && col-1 > 0){
			// if (mapaNuevo[fil-3][col-1] == '?')
			mapaNuevo[fil-3][col-1] = sensores.terreno[11];
			if(!puedoAvanzar(mapaNuevo[fil-3][col-1])){
				miguita[fil-3][col-1] = std::numeric_limits<int>::max();
		
			}
		}
		if(fil-3 > 0 ){
			// if (mapaNuevo[fil-3][col] == '?')
			mapaNuevo[fil-3][col] = sensores.terreno[12];
			if(!puedoAvanzar(mapaNuevo[fil-3][col])){
				miguita[fil-3][col] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil-3 > 0 && col+1 < mapaNuevo[0].size()){
			// if (mapaNuevo[fil-3][col+1] == '?')
			mapaNuevo[fil-3][col+1] = sensores.terreno[13];
			if(!puedoAvanzar(mapaNuevo[fil-3][col+1])){
				miguita[fil-3][col+1] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil-3 > 0 && col+2 < mapaNuevo[0].size()){
			// if (mapaNuevo[fil-3][col+2] == '?')
			mapaNuevo[fil-3][col+2] = sensores.terreno[14];
			if(!puedoAvanzar(mapaNuevo[fil-3][col+2])){
				miguita[fil-3][col+2] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil-3 > 0 && col+3 < mapaNuevo[0].size()){
			// if (mapaNuevo[fil-3][col+3] == '?')
			mapaNuevo[fil-3][col+3] = sensores.terreno[15];
			if(!puedoAvanzar(mapaNuevo[fil-3][col+3])){
				miguita[fil-3][col+3] = std::numeric_limits<int>::max();
				
			}
		}
		
		break;

		case 1:

		if(fil-1 > 0 && col+1 < mapaNuevo[0].size()){
			// if (mapaNuevo[fil-1][col+1] == '?')
			mapaNuevo[fil-1][col+1] = sensores.terreno[1];
			if(!puedoAvanzar(mapaNuevo[fil-1][col+1])){
				miguita[fil-1][col+1] = std::numeric_limits<int>::max();
				
			}
			
		}
		if(col+1 < mapaNuevo[0].size()){
			// if (mapaNuevo[fil][col+1] == '?')
			mapaNuevo[fil][col+1] = sensores.terreno[2];
			if(!puedoAvanzar(mapaNuevo[fil][col+1])){
					miguita[fil][col+1] = std::numeric_limits<int>::max();
					
				}
		}
		if(fil+1<mapaNuevo.size() && col+1<mapaNuevo[0].size()){
			// if (mapaNuevo[fil+1][col+1] == '?')
			mapaNuevo[fil+1][col+1] = sensores.terreno[3];
			if(!puedoAvanzar(mapaNuevo[fil+1][col+1])){
				miguita[fil+1][col+1] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil-2>0 && col+2<mapaNuevo[0].size() ){
			// if (mapaNuevo[fil-2][col+2] == '?')
			mapaNuevo[fil-2][col+2] = sensores.terreno[4];
			if(!puedoAvanzar(mapaNuevo[fil-2][col+2])){
				miguita[fil-2][col+2] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil-1>0 && col+2<mapaNuevo[0].size() ){
			// if (mapaNuevo[fil-1][col+2] == '?')
			mapaNuevo[fil-1][col+2] = sensores.terreno[5];
			if(!puedoAvanzar(mapaNuevo[fil-1][col+2])){
				miguita[fil-1][col+2] = std::numeric_limits<int>::max();
				
			}
		}
		if(col+2<mapaNuevo[0].size()){
			// if (mapaNuevo[fil][col+2] == '?')
			mapaNuevo[fil][col+2] = sensores.terreno[6];
			if(!puedoAvanzar(mapaNuevo[fil][col+2])){
				miguita[fil][col+2] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil+1<mapaNuevo.size() && col+2<mapaNuevo[0].size()){
			// if (mapaNuevo[fil+1][col+2] == '?')
			mapaNuevo[fil+1][col+2] = sensores.terreno[7];
			if(!puedoAvanzar(mapaNuevo[fil+1][col+2])){
				miguita[fil+1][col+2] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil+2<mapaNuevo.size() && col+2<mapaNuevo[0].size()){
			// if (mapaNuevo[fil+2][col+2] == '?')
			mapaNuevo[fil+2][col+2] = sensores.terreno[8];
			if(!puedoAvanzar(mapaNuevo[fil+2][col+2])){
				miguita[fil+2][col+2] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil-3>0 && col+3<mapaNuevo[0].size()){
			// if (mapaNuevo[fil-3][col+3] == '?')
			mapaNuevo[fil-3][col+3] = sensores.terreno[9];
			if(!puedoAvanzar(mapaNuevo[fil-3][col+3])){
				miguita[fil-3][col+3] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil-2>0 && col+3<mapaNuevo[0].size()){
			// if (mapaNuevo[fil-2][col+3] == '?')
			mapaNuevo[fil-2][col+3] = sensores.terreno[10];
			if(!puedoAvanzar(mapaNuevo[fil-2][col+3])){
				miguita[fil-2][col+3] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil-1>0 && col+3<mapaNuevo[0].size() ){
			// if (mapaNuevo[fil-1][col+3] == '?')
			mapaNuevo[fil-1][col+3] = sensores.terreno[11];
			if(!puedoAvanzar(mapaNuevo[fil-1][col+3])){
				miguita[fil-1][col+3] = std::numeric_limits<int>::max();
				
			}
		}
		if( col+3<mapaNuevo[0].size()){
			// if (mapaNuevo[fil][col+3] == '?')
			mapaNuevo[fil][col+3] = sensores.terreno[12];
			if(!puedoAvanzar(mapaNuevo[fil][col+3])){
				miguita[fil][col+3] = std::numeric_limits<int>::max();
				
			}
		}
		if( fil+1<mapaNuevo.size() && col+3<mapaNuevo[0].size()){
			// if (mapaNuevo[fil+1][col+3] == '?')
			mapaNuevo[fil+1][col+3] = sensores.terreno[13];
			if(!puedoAvanzar(mapaNuevo[fil+1][col+3])){
				miguita[fil+1][col+3] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil+2<mapaNuevo.size() && col+3<mapaNuevo[0].size()){
			// if (mapaNuevo[fil+2][col+3] == '?')
			mapaNuevo[fil+2][col+3] = sensores.terreno[14];
			if(!puedoAvanzar(mapaNuevo[fil+2][col+3])){
				miguita[fil+2][col+3] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil+3<mapaNuevo.size() && col+3<mapaNuevo[0].size()){
			// if (mapaNuevo[fil+3][col+3] == '?')
			mapaNuevo[fil+3][col+3] = sensores.terreno[15];
			if(!puedoAvanzar(mapaNuevo[fil+3][col+3])){
				miguita[fil+3][col+3] = std::numeric_limits<int>::max();
				
			}
		}
		
		break;

		case 2:
		if(fil+1<mapaNuevo.size() && col+1<mapaNuevo[0].size() ){
			// if (mapaNuevo[fil+1][col+1] == '?')
			mapaNuevo[fil+1][col+1] = sensores.terreno[1];
			if(!puedoAvanzar(mapaNuevo[fil+1][col+1])){
				miguita[fil+1][col+1] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil+1<mapaNuevo.size()){
			// if (mapaNuevo[fil+1][col] == '?')
			mapaNuevo[fil+1][col] = sensores.terreno[2];
			if(!puedoAvanzar(mapaNuevo[fil+1][col])){
				miguita[fil+1][col] = std::numeric_limits<int>::max();
				
			}
		}

		if(fil+1<mapaNuevo.size() && col-1>0 ){
			// if (mapaNuevo[fil+1][col-1] == '?')
			mapaNuevo[fil+1][col-1] = sensores.terreno[3];
			if(!puedoAvanzar(mapaNuevo[fil+1][col-1])){
				miguita[fil+1][col-1] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil+2<mapaNuevo.size() && col+2<mapaNuevo[0].size()){
			// if (mapaNuevo[fil+2][col+2] == '?')
			mapaNuevo[fil+2][col+2] = sensores.terreno[4];
			if(!puedoAvanzar(mapaNuevo[fil+2][col+2])){
				miguita[fil+2][col+2] = std::numeric_limits<int>::max();
				
			}
			
		}
		if(fil+2<mapaNuevo.size() && col+1<mapaNuevo[0].size() ){
			// if (mapaNuevo[fil+2][col+1] == '?')
			mapaNuevo[fil+2][col+1] = sensores.terreno[5];
			if(!puedoAvanzar(mapaNuevo[fil+2][col+1])){
					miguita[fil+2][col+1] = std::numeric_limits<int>::max();
					
				}
		}
		if(fil+2<mapaNuevo.size()){
			// if (mapaNuevo[fil+2][col] == '?')
			mapaNuevo[fil+2][col] = sensores.terreno[6];
			if(!puedoAvanzar(mapaNuevo[fil+2][col])){
				miguita[fil+2][col] = std::numeric_limits<int>::max();
				
			}
		
		}

		if(fil+2<mapaNuevo.size() && col-1>0){
			// if (mapaNuevo[fil+2][col-1] == '?')
			mapaNuevo[fil+2][col-1] = sensores.terreno[7];
			if(!puedoAvanzar(mapaNuevo[fil+2][col-1])){
				miguita[fil+2][col-1] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil+2<mapaNuevo.size() && col-2>0){
			// if (mapaNuevo[fil+2][col-2] == '?')
			mapaNuevo[fil+2][col-2] = sensores.terreno[8];
			if(!puedoAvanzar(mapaNuevo[fil+2][col-2])){
				miguita[fil+2][col-2] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil+3 < mapaNuevo.size() && col+3<mapaNuevo[0].size()){
			// if (mapaNuevo[fil+3][col+3] == '?')
			mapaNuevo[fil+3][col+3] = sensores.terreno[9];
			if(!puedoAvanzar(mapaNuevo[fil+3][col+3])){
				miguita[fil+3][col+3] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil+3<mapaNuevo.size() && col+2<mapaNuevo[0].size()){
			// if (mapaNuevo[fil+3][col+2] == '?')
			mapaNuevo[fil+3][col+2] = sensores.terreno[10];
			if(!puedoAvanzar(mapaNuevo[fil+3][col+2])){
					miguita[fil+3][col+2] = std::numeric_limits<int>::max();
					
				}
		}
		if(fil+3<mapaNuevo.size() && col+1<mapaNuevo[0].size()){
			// if (mapaNuevo[fil+3][col+1] == '?')
			mapaNuevo[fil+3][col+1] = sensores.terreno[11];
			if(!puedoAvanzar(mapaNuevo[fil+3][col+1])){
				miguita[fil+3][col+1] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil+3<mapaNuevo.size()){
			// if (mapaNuevo[fil+3][col] == '?')
			mapaNuevo[fil+3][col] = sensores.terreno[12];
			if(!puedoAvanzar(mapaNuevo[fil+3][col])){
				miguita[fil+3][col] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil+3<mapaNuevo.size() && col-1>0){
			// if (mapaNuevo[fil+3][col-1] == '?')
			mapaNuevo[fil+3][col-1] = sensores.terreno[13];
			if(!puedoAvanzar(mapaNuevo[fil+3][col-1])){
				miguita[fil+3][col-1] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil+3<mapaNuevo.size() && col-2>0){
			// if (mapaNuevo[fil+3][col-2] == '?')
			mapaNuevo[fil+3][col-2] = sensores.terreno[14];
			if(!puedoAvanzar(mapaNuevo[fil+3][col-2])){
				miguita[fil+3][col-2] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil+3<mapaNuevo.size() && col-3>0){
			// if (mapaNuevo[fil+3][col-3] == '?')
			mapaNuevo[fil+3][col-3] = sensores.terreno[15];
			if(!puedoAvanzar(mapaNuevo[fil+3][col-3])){
				miguita[fil+3][col-3] = std::numeric_limits<int>::max();
				
			}
		}
		
		break;

		case 3:
		if(fil+1<mapaNuevo.size() && col-1>0){
			// if (mapaNuevo[fil+1][col-1] == '?')
			mapaNuevo[fil+1][col-1] = sensores.terreno[1];
			if(!puedoAvanzar(mapaNuevo[fil+1][col-1])){
				miguita[fil+1][col-1] = std::numeric_limits<int>::max();
				
			}
		}
		if(col-1>0){
			// if (mapaNuevo[fil][col-1] == '?')
			mapaNuevo[fil][col-1] = sensores.terreno[2];
			if(!puedoAvanzar(mapaNuevo[fil][col-1])){
				miguita[fil][col-1] = std::numeric_limits<int>::max();
				
			}
		}
		if( fil-1>0 && col-1>0){
			// if (mapaNuevo[fil-1][col-1] == '?')
			mapaNuevo[fil-1][col-1] = sensores.terreno[3];
			if(!puedoAvanzar(mapaNuevo[fil+1][col-1])){
				miguita[fil-1][col-1] = std::numeric_limits<int>::max();
				
			}
		}
		if( fil+2<mapaNuevo.size() && col-2>0){
			// if (mapaNuevo[fil+2][col-2] == '?')
			mapaNuevo[fil+2][col-2] = sensores.terreno[4];
			if(!puedoAvanzar(mapaNuevo[fil+2][col-2])){
				miguita[fil+2][col-2] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil+1<mapaNuevo.size() && col-2>0){
			// if (mapaNuevo[fil+1][col-2] == '?')
			mapaNuevo[fil+1][col-2] = sensores.terreno[5];
			if(!puedoAvanzar(mapaNuevo[fil+1][col-2])){
				miguita[fil+1][col-2] = std::numeric_limits<int>::max();
				
			}
		}
		if(col-2>0){
			// if (mapaNuevo[fil][col-2] == '?')
			mapaNuevo[fil][col-2] = sensores.terreno[6];
			if(!puedoAvanzar(mapaNuevo[fil][col-2])){
				miguita[fil][col-2] = std::numeric_limits<int>::max();
				
			}
		}
		if( fil-1>0 && col-2>0){
			// if (mapaNuevo[fil-1][col-2] == '?')
			mapaNuevo[fil-1][col-2] = sensores.terreno[7];
			if(!puedoAvanzar(mapaNuevo[fil-1][col-2])){
				miguita[fil-1][col-2] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil-2>0 && col-2>0){
			// if (mapaNuevo[fil-2][col-2] == '?')
			mapaNuevo[fil-2][col-2] = sensores.terreno[8];
			if(!puedoAvanzar(mapaNuevo[fil-2][col-2])){
				miguita[fil-2][col-2] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil+3<mapaNuevo.size() && col-3>0){
			// if (mapaNuevo[fil+3][col-3] == '?')
			mapaNuevo[fil+3][col-3] = sensores.terreno[9];
			if(!puedoAvanzar(mapaNuevo[fil+3][col-3])){
				miguita[fil+3][col-3] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil+2<mapaNuevo.size() && col-3>0){
			// if (mapaNuevo[fil+2][col-3] == '?')
			mapaNuevo[fil+2][col-3] = sensores.terreno[10];
			if(!puedoAvanzar(mapaNuevo[fil+2][col-3])){
				miguita[fil+2][col-3] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil+1<mapaNuevo.size() && col-3>0){
			// if (mapaNuevo[fil+1][col-3] == '?')
			mapaNuevo[fil+1][col-3] = sensores.terreno[11];
			if(!puedoAvanzar(mapaNuevo[fil+1][col-3])){
				miguita[fil+1][col-3] = std::numeric_limits<int>::max();
				
			}
		}
		if(col-3>0){
			// if (mapaNuevo[fil][col-3] == '?')
			mapaNuevo[fil][col-3] = sensores.terreno[12];
			if(!puedoAvanzar(mapaNuevo[fil][col-3])){
				miguita[fil][col-3] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil-1>0 && col-3>0){
			// if (mapaNuevo[fil-1][col-3] == '?')
			mapaNuevo[fil-1][col-3] = sensores.terreno[13];
			if(!puedoAvanzar(mapaNuevo[fil-1][col-3])){
				miguita[fil-1][col-3] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil-2>0 && col-3>0){
			// if (mapaNuevo[fil-2][col-3] == '?')
			mapaNuevo[fil-2][col-3] = sensores.terreno[14];
			if(!puedoAvanzar(mapaNuevo[fil-2][col-3])){
				miguita[fil-2][col-3] = std::numeric_limits<int>::max();
				
			}
		}
		if(fil-3>0 && col-3>0){
			// if (mapaNuevo[fil-3][col-3] == '?')
			mapaNuevo[fil-3][col-3] = sensores.terreno[15];
			if(!puedoAvanzar(mapaNuevo[fil-3][col-3])){
				miguita[fil-3][col-3] = std::numeric_limits<int>::max();
				
			}
		}
	break;
	}

}


void ComportamientoJugador::PintarMapa(vector<vector <unsigned char> > mapaNuevo){
 	      for(int i=0; i<mapaNuevo.size(); i++){
            for(int j=0; j<mapaNuevo[0].size(); j++){
				if(mapaNuevo[i][j]=='?'){
					//cout<<"No encuentro nada, voy a pintar con mapaResultado\n";
					//mapaResultado[i][j] = mapaNuevo[]
					//cout<<"Muestro el mapaResultado[ "<<i<<" ][ "<<j<<" ]-->"<<mapaResultado[i][j]<<endl;


				} 
				else{
					if(!esta_situado)
					cout<<"Muestro el mapaAuxiliar[ "<<i<<" ][ "<<j<<" ]-->"<<mapaNuevo[i][j]<<endl;
					if(esta_situado)
					cout<<"Muestro el mapaResultado[ "<<i<<" ][ "<<j<<" ]-->"<<mapaNuevo[i][j]<<endl;
				}
                
			}
		  } 
}


 



void ComportamientoJugador::exploracion(int orientacion,Sensores sensores,vector< vector<unsigned int> > &mapaNuevo,list<Action> &plan){
	Action sigAccion;
	hayPlan = true;
switch (brujula) {
				case 0: // Norte
					if(mapaNuevo[fil-1][col] <= mapaNuevo[fil][col-1] && mapaNuevo[fil-1][col] <= mapaNuevo[fil][col+1] && puedoAvanzar(sensores.terreno[2])&& sensores.superficie[2]!='a'){
						sigAccion=actFORWARD;
						plan.push_back(sigAccion);
					}
					else if(mapaNuevo[fil][col-1] >= mapaNuevo[fil][col+1]){
						sigAccion=actTURN_R;
						plan.push_back(sigAccion);
					}
					else
						sigAccion=actTURN_L;
						plan.push_back(sigAccion);
					break;
				case 1: // Este
					if(mapaNuevo[fil][col+1]<=mapaNuevo[fil-1][col] && mapaNuevo[fil][col+1] <= mapaNuevo[fil+1][col] && puedoAvanzar(sensores.terreno[2])&& sensores.superficie[2]!='a'){
						sigAccion=actFORWARD;
						plan.push_back(sigAccion);
					}
					else if(mapaNuevo[fil-1][col] >= mapaNuevo[fil+1][col]){
						sigAccion=actTURN_R;
						plan.push_back(sigAccion);
					}
					else
						sigAccion=actTURN_L;
						plan.push_back(sigAccion);
					break;
				case 2: // Sur
						if(mapaNuevo[fil+1][col]<=mapaNuevo[fil][col-1] && mapaNuevo[fil+1][col]<=mapaNuevo[fil][col+1] && puedoAvanzar(sensores.terreno[2])&& sensores.superficie[2]!='a'){
							sigAccion=actFORWARD;
							plan.push_back(sigAccion);
						}
						else if(mapaNuevo[fil][col-1] >= mapaNuevo[fil][col+1]){
							sigAccion=actTURN_L;
							plan.push_back(sigAccion);
						}
						else
							sigAccion=actTURN_R;
							plan.push_back(sigAccion);
						break;

						break;
					case 3: // Oeste
						if(mapaNuevo[fil][col-1]<=mapaNuevo[fil+1][col] && mapaNuevo[fil][col-1]<=mapaNuevo[fil-1][col] && puedoAvanzar(sensores.terreno[2]) && sensores.superficie[2]!='a'){
							sigAccion=actFORWARD;
							plan.push_back(sigAccion);
						}
						else if(mapaNuevo[fil-1][col] >= mapaNuevo[fil+1][col]){
							sigAccion=actTURN_L;
							plan.push_back(sigAccion);
						}
						else
							sigAccion=actTURN_R;
							plan.push_back(sigAccion);
					break;
			}
	//return sigAccion;
}
void ComportamientoJugador::Reorientar(Sensores &sensores){
	veo_PK=true;
	vector<vector<unsigned int> > miguitasCopia(mapaPrioridades);
	int difF = fil - sensores.mensajeF;
	int difC = col - sensores.mensajeC;

	for(int i = 0; i < mapaResultado.size(); i++){
    		for(int j = 0; j < mapaResultado[i].size(); j++){
				if(!esta_situado){
            	mapaResultado[i][j] = mapaAuxiliar[i+difF][j+difC];
				
				mapaPrioridades[i][j] = miguitasCopia[i+difF][j+difC];
				}
			
			}
		}
	
}

void ComportamientoJugador::busquedaPK(Sensores sensores, list<Action> &plan){
	veo_PK = true;
	hayPlan = true;
	for(int i=1; i < sensores.terreno.size() && esta_situado == false ;i++){
			if(sensores.terreno[i] == 'K'){//Busco el PK dependiendo donde se situe
			
			/*
			
					Forma del vector sensores.terreno
						   9
						4 10
					  1 5 11
					0 2 6 12
					  3 7 13
						8 14
						  15
				*/
				
				
				if(i < 4 && !obstaculo_sensor(sensores.terreno[i])){
					cout<<"Avanzo1\n";
					plan.push_back(actFORWARD);
				}
				else if(i < 10 && !obstaculo_sensor(sensores.terreno[i])){
					cout<<"Avanzo2\n";
					plan.push_back(actFORWARD);
					plan.push_back(actFORWARD);
				}
				else{
					if(!obstaculo_sensor(sensores.terreno[i])){
					cout<<"Avanzo3\n";
						plan.push_back(actFORWARD);
						plan.push_back(actFORWARD);
						plan.push_back(actFORWARD);
					}
					
				}
				if( i == 9){
					cout<<"Giro1\n";
					plan.push_back(actTURN_L);
				}
				else if(i == 10 or i == 4){
					cout<<"Giro2\n";
					plan.push_back(actTURN_L);
				}
				else if(i == 11 or i == 5 or i == 1){
					cout<<"Giro3\n";
					plan.push_back(actTURN_L);
				}
				else if(i == 13 or i == 7 or i==3){
					cout<<"Giro4\n";
					plan.push_back(actTURN_R);
				}
				else if(i == 14 or i == 8){
					cout<<"Giro5\n";
					plan.push_back(actTURN_R);
				}
				else if(i == 15){
					cout<<"Giro6\n";
					plan.push_back(actTURN_R);
				}
			}
	}
}
