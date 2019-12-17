#include "tabuleiro.hpp"


/**************************************************
*	Estrutura tabuleiro:                          *
* 		-Armazena um vetor de vetor de char       *
* 		que representa as posicoes do tabuleiro   *
*                                                 *
* 		- Uso:                                    *
*			- Tabuleiro exemplo (<vetor>);        *
*                                                 *
***************************************************/
Tabuleiro::Tabuleiro(vector<vector<char>> posicoes)
{
	this->posicoes = posicoes;
}
