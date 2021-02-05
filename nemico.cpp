#include "oggetto.h"
#include "nemico.h"

Nemico::Nemico(int x, int y, TipoDiOggetto tipo, ConvertiAsciiArt *asciiArt, Map *mappa)
	:Oggetto(x, y, tipo, asciiArt)
{
	_mappa = mappa;
	_direction = true;
}

bool Nemico::minore_coppia(int x1, int y1,int x2, int y2)
{
	bool return_value = false; /* returna true se la prima coppia è minore della seconda */
	if(y1 == y2 && x1 < x2){
		 return_value = true;
	}
	else{
		if(y1 > y2) return_value = true;
	}

	return return_value;
		

}

bool Nemico::maggiore_coppia(int x1, int y1,int x2, int y2)
{
	bool return_value = false; /* returna true se la prima coppia è maggiore della seconda */
	if(y1 == y2 && x1 > x2){
		return_value = true;
	}
	else{
		if(y1 > y2) return_value = true;
	}
	return return_value;
		

}

int *Nemico::getZampaDestra()
{
	figura buff_figura = ottieniFigura();	
	int maxX;
	int maxY;	
	int *ret = new int(2);
	if(buff_figura != NULL){
		maxX = buff_figura -> x;
		maxY = buff_figura -> y; 
		while(buff_figura != NULL){
			if(maggiore_coppia(buff_figura -> x, buff_figura -> y, maxX, maxY)){
				maxY = buff_figura -> y; 
				maxX = buff_figura -> x;
			}
			buff_figura = buff_figura -> next;
		}
	}
	ret[0] = maxY;
	ret[1] = maxX;

	return ret;
};

int *Nemico::getZampaSinistra()
{
	
	figura buff_figura = ottieniFigura();	
	int minX;
	int minY;	
	int *ret = new int(2);
	if(buff_figura != NULL){
		minX = buff_figura -> x;
		minY = buff_figura -> y; 
		while(buff_figura != NULL){
			if(minore_coppia(buff_figura -> x, buff_figura -> y, minX, minY)){
				minY = buff_figura -> y; 
				minX = buff_figura -> x;
			}
			buff_figura = buff_figura -> next;
		}
	}
	ret[0] = minY;
	ret[1] = minX;

	return ret;
}

bool Nemico::getDirection()
{
	return _direction;
}

bool Nemico::changeDirection()
{
	_direction = !_direction;
	return _direction;
}

void Nemico::vaiASinistra(int numeroPassi)
{
	figura buff_figura = ottieniFigura();	
	while(buff_figura != NULL){
		buff_figura -> x = buff_figura -> x - numeroPassi;
		buff_figura = buff_figura -> next;
	}
}

void Nemico::vaiADestra(int numeroPassi)
{
	figura buff_figura = ottieniFigura();	
	while(buff_figura != NULL){
		buff_figura -> x = buff_figura -> x + numeroPassi;
		buff_figura = buff_figura -> next;
	}
}

void Nemico::muoviNemico()
{
	if(getDirection()){//true se sta andando a sinistra
		if(_mappa->controllaCollisionePiattaforme(ottieniFigura(), -1, 1) == -1){
			vaiASinistra(1);	
		}
		else{
			
			changeDirection();
		}

	}
	else{
		if(_mappa->controllaCollisionePiattaforme(ottieniFigura(), 1, 1) == -1){
			vaiADestra(1);	
		}
		else{
			
			changeDirection();
		}
	}
}

abisso Nemico::morte()
{
	
}

void Nemico::impostaFigura(TipoDiOggetto tipo, ConvertiAsciiArt *asciiArt) {
	Oggetto::impostaFigura(tipo, asciiArt);
	switch (tipo)
	{
	
	case OS_NEMICO1:
		_vita = 1;
		_attacco = 1;
	break;

	case OS_NEMICO2:
		_vita = 2;
		_attacco = 1;
	break;
	
	default:
		break;
	}
}
