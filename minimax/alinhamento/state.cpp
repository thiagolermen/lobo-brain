#include "state.hpp"
#include "siphasher.hpp"
#include "evaluations.hpp"
#include "tree.hpp"
#include "agent.hpp"
#include <vector>
#include <iostream>

using namespace std;
typedef vector<vector<char>> Tabuleiro;


/**************************************************
*	Estrutura tabuleiro:                          *
* 		-Armazena um vetor de vetor de char       *
* 		que representa as posicoes do tabuleiro   *
*                                                 *
* 		- Uso:                                    *
*			- Tabuleiro exemplo (<vetor>);        *
*                                                 *
***************************************************/


// referência
State start(
			{	{'2', '1', '2'},
				{'0', '0', '0'},
				{'1', '2', '1'}
			});

Tabuleiro initialBoard() {
		return start.getBoard();
}



/*
	=================
	Funções da Classe
	=================
/*


/* Class Contructor */
State::State(Agent p1, Agent p2, char player_symbol, bool finished, Tabuleiro board)
{
	this->p1 = Agent(p1);
	this->p2 = Agent(p2);
	this->playing_symbol = player_symbol;
	this->finished = finished;
	this->board = board;
	this->hash = this->getHash();
}

/* hash related */
unsigned int State::getHash() {

	/* só pra nao encurtar linha */
	vector<vector<char>> b = this->board;

	/* inicializando hasher */
	SipHasher hasher;

	/* varre as linhas */
	for(int i = 0; i < b.size(); i++) {

			/* varre cada linha jogando o char pro hasher */
			for(int k = 0; k < b[i].size(); k++) {
				hasher << b[i][k];
			}

	}

	return hasher.finish();
}


/* positons related */

Tabuleiro State::getBoard() {
	return this->board;
}

void State::setBoard(Tabuleiro board) {
	this->board = board;
	this->hash = this->getHash();
}

void State::setPosition(int i, int j, char player) {
	this->board[i][j] = player;
	this->hash = this->getHash();
}

void State::print() {

	vector<vector<char>> t = this->getBoard();

	cout << "\t=============" << endl ;

	for(int i=0; i<3; i++) {
		cout << '\t';
		for(int j=0; j<3; j++) {
			cout << "| " << t[i][j] << " ";
		}
                cout << "|" << endl;
	}

	cout << "\t=============" << endl ;

}
/*
Essa funcão vai ser usada pra saber quais hashes devem
ser procurados na Q-table
*/
vector<unsigned int> State::getPossibleMovesHashes(char player, bool jump) {

	Tree* obj = new Tree();
	vector<State> states = obj->generateStates(*this, player, jump);
	vector<unsigned int> hashes;

	for(vector<State>::iterator it = states.begin(); it != states.end(); it++) {
		hashes.push_back((*it).getHash());
	}

	// Testing results
	cout << "Hashes:" << endl;
	for(int i = 0; i < hashes.size(); i++) {
		cout << "\t#" << hashes[i] << endl;
	}

	return hashes;

}

// da pra embutir o contador de repeticoes dentro
//	da propria classe e tirar o parametro

int State::winner(int *rep){
	Tabuleiro t = this->getBoard();
	Tabuleiro t_ini = initialBoard();

	int cont = 0;
	char jogador = this->playing_symbol;
	char oponente;

	if(playing_symbol == '1'){
		oponente = '2';
	}else{
		oponente = '1';
	}
	/*
		===============================
								TIE
		===============================
	*/
	for (int i=0; i<3; i++)
	{
			for(int j=0; j<3; j++)
			{

					if(t[i][j] == t_ini[i][j])
					{
							cont++;
							// cout << "cont = " << cont << endl;
					}
			}
	}

	// Tabuleiro igual ao do comeco -> empate
	// Retorna 0 e incrementa repeticoes do tab inicial
	if (cont == 9)
	{
			(*rep)++;
			if (*rep >= 3) {
					this->finished = true;
					return 0;
			}
	}

	/*
		===============================
								WIN
		===============================
	*/

	if (t[0][0] == jogador)
	{
		if (t[0][1] == jogador && t[0][2] == jogador ||
					t[1][0] == jogador && t[2][0] == jogador ||
					t[1][1] == jogador && t[2][2] == jogador)
			{
					this->finished = true;
					return 1;
			}
	}
	else if (t[0][2] == jogador)
	{
			if (t[1][1] == jogador && t[2][0] == jogador ||
					t[1][2] == jogador && t[2][2] == jogador)
			{
					this->finished = true;
					return 1;
			}
	}
	else if (t[1][1] == jogador)
	{
			if(t[0][1] == jogador && t[2][1] == jogador ||
					t[1][0] == jogador && t[1][2] == jogador) {
					this->finished = true;
					return 1;
			}
	}
	else if (t[2][0] == jogador)
	{
			if(t[2][1] == jogador && t[2][2] == jogador) {
					this->finished = true;
					return 1;
			}
	}

	/*
		===============================
								LOSE
		===============================
	*/

if (t[0][0] == oponente)
	{
		if (t[0][1] == oponente && t[0][2] == oponente ||
					t[1][0] == oponente && t[2][0] == oponente ||
					t[1][1] == oponente && t[2][2] == oponente)
			{
					this->finished = true;
					return 2;
			}
	}
	else if (t[0][2] == oponente)
	{
			if (t[1][1] == oponente && t[2][0] == oponente ||
					t[1][2] == oponente && t[2][2] == oponente)
			{
					this->finished = true;
					return 2;
			}
	}
	else if (t[1][1] == oponente)
	{
			if(t[0][1] == oponente && t[2][1] == oponente ||
					t[1][0] == oponente && t[1][2] == oponente) {
					this->finished = true;
					return 2;
			}
	}
	else if (t[2][0] == oponente)
	{
			if(t[2][1] == oponente && t[2][2] == oponente) {
					this->finished = true;
					return 2;
			}
	}

	/*
		===============================
							NÃO TERMINAL
		===============================
		acho q é aqui que entraria o
		Q-learning, pra poder retornar
		um valor
	*/

	this->finished = false;
	return 0;
}

vector<vector<int>> possibleMoves(int player_pos){
	vector<vector<int>> possible_mov;
    vector<int> ij;

    switch(player_pos)
    {
		case 0://(0,0)
			ij.push_back(0); ij.push_back(1);
			possible_mov.push_back(ij);

			ij[0] = 1; ij[1] = 0;
			possible_mov.push_back(ij);

			ij[0] = 1; ij[1] = 1;
			possible_mov.push_back(ij);
		return possible_mov;

		case 1://(0,1)
			ij.push_back(0); ij.push_back(0);
			possible_mov.push_back(ij);

			ij[0] = 1; ij[1] = 1;
			possible_mov.push_back(ij);

			ij[0] = 0; ij[1] = 2;
			possible_mov.push_back(ij);
		return possible_mov;

		case 2://(0,2)
			ij.push_back(0); ij.push_back(1);
			possible_mov.push_back(ij);

			ij[0] = 1; ij[1] = 1;
			possible_mov.push_back(ij);

			ij[0] = 1; ij[1] = 2;
			possible_mov.push_back(ij);
		return possible_mov;

		case 3://(1,0)
			ij.push_back(0); ij.push_back(0);
			possible_mov.push_back(ij);

			ij[0] = 1; ij[1] = 1;
			possible_mov.push_back(ij);

			ij[0] = 2; ij[1] = 0;
			possible_mov.push_back(ij);
		return possible_mov;

		case 4://(1,1)
			ij.push_back(0); ij.push_back(0);
			possible_mov.push_back(ij);

			ij[0] = 0; ij[1] = 1;
			possible_mov.push_back(ij);

			ij[0] = 0; ij[1] = 2;
			possible_mov.push_back(ij);

			ij[0] = 1; ij[1] = 0;
			possible_mov.push_back(ij);

			ij[0] = 1; ij[1] = 2;
			possible_mov.push_back(ij);

			ij[0] = 2; ij[1] = 0;
			possible_mov.push_back(ij);

			ij[0] = 2; ij[1] = 1;
			possible_mov.push_back(ij);

			ij[0] = 2; ij[1] = 2;
			possible_mov.push_back(ij);
		return possible_mov;

		case 5://(1,2)
			ij.push_back(0); ij.push_back(2);
			possible_mov.push_back(ij);

			ij[0] = 1; ij[1] = 1;
			possible_mov.push_back(ij);

			ij[0] = 2; ij[1] = 2;
			possible_mov.push_back(ij);
		return possible_mov;

		case 6://(2,0)
			ij.push_back(2); ij.push_back(1);
			possible_mov.push_back(ij);

			ij[0] = 1; ij[1] = 0;
			possible_mov.push_back(ij);

			ij[0] = 1; ij[1] = 1;
			possible_mov.push_back(ij);
		return possible_mov;

		case 7://(2,1)
			ij.push_back(2); ij.push_back(0);
			possible_mov.push_back(ij);

			ij[0] = 1; ij[1] = 1;
			possible_mov.push_back(ij);

			ij[0] = 2; ij[1] = 2;
			possible_mov.push_back(ij);
		return possible_mov;

		case 8://(2,2)
			ij.push_back(2); ij.push_back(1);
			possible_mov.push_back(ij);

			ij[0] = 1; ij[1] = 1;
			possible_mov.push_back(ij);

			ij[0] = 1; ij[1] = 2;
			possible_mov.push_back(ij);
		return possible_mov;

		default: return possible_mov;
   }
}



vector<vector<int>> possibleJumpMoves(int player_pos, State state) {
      vector<vector<int>> possible_mov;
      vector<int> ij;

      vector<vector<char>> t = state.getBoard();

      switch (player_pos) {
          case 0:
            if(t[0][1] != '0') {
              ij.push_back(0); ij.push_back(2);
              possible_mov.push_back(ij);
            }else if(t[1][0] != '0') {
              ij.push_back(2); ij.push_back(0);
              possible_mov.push_back(ij);
            }else if(t[1][1] != '0') {
              ij.push_back(2); ij.push_back(2);
              possible_mov.push_back(ij);
            }
            return possible_mov;

          case 1:
              if(t[1][1] != '0') {
                ij.push_back(2); ij.push_back(1);
                possible_mov.push_back(ij);
              }
              return possible_mov;

          case 2:
            if(t[0][1] != '0') {
              ij.push_back(0); ij.push_back(0);
              possible_mov.push_back(ij);
            }else if(t[1][2] != '0') {
              ij.push_back(2); ij.push_back(2);
              possible_mov.push_back(ij);
            }else if(t[1][1] != '0') {
              ij.push_back(2); ij.push_back(0);
              possible_mov.push_back(ij);
            }
            return possible_mov;

          case 3:
            if(t[1][1] != '0') {
              ij.push_back(1); ij.push_back(2);
              possible_mov.push_back(ij);
            }
            return possible_mov;

          case 5:
            if(t[1][1] != '0') {
              ij.push_back(1); ij.push_back(0);
              possible_mov.push_back(ij);
            }
            return possible_mov;

          case 6:
            if(t[1][0] != '0') {
              ij.push_back(0); ij.push_back(0);
              possible_mov.push_back(ij);
            }else if(t[2][1] != '0') {
              ij.push_back(2); ij.push_back(2);
              possible_mov.push_back(ij);
            }else if(t[1][1] != '0') {
              ij.push_back(0); ij.push_back(2);
              possible_mov.push_back(ij);
            }
            return possible_mov;

          case 7:
            if(t[1][1] != '0') {
              ij.push_back(0); ij.push_back(1);
              possible_mov.push_back(ij);
            }
            return possible_mov;

          case 8:
            if(t[2][1] != '0') {
              ij.push_back(2); ij.push_back(0);
              possible_mov.push_back(ij);
            }else if(t[1][2] != '0') {
              ij.push_back(0); ij.push_back(2);
              possible_mov.push_back(ij);
            }else if(t[1][1] != '0') {
              ij.push_back(0); ij.push_back(0);
              possible_mov.push_back(ij);
            }
            return possible_mov;

          default: return possible_mov;

      }
}



vector<Tabuleiro> State::possibleBoards(bool jump) {
    int i,j, player_pos;
    vector<vector<int>> possible_mov;//matriz com as possiveis posicoes geradas a partir do tabuleiro passado como parametro
    vector<vector<int>> aux_vet;
    vector<Tabuleiro> possibleBoards;//vetor com os possiveis tabuleiros gerados a partir da posicao atual

		char player = this->playing_symbol;

		Tabuleiro t = this->getBoard();

    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            if(t[i][j] == player)//verifica se a posicao do tabuleiro esta sendo ocupada pelo jogador
            {
                player_pos = 3*i+j;//realiza uma aritmetica para determinar a posicao do jogador no tabuleiro

                possible_mov = possibleMoves(player_pos);

								// appending possible moves from the jump engine, if using
                if(jump) {
                  aux_vet = possibleJumpMoves(player_pos, *this);
                  possible_mov.insert(possible_mov.end(), aux_vet.begin(), aux_vet.end());
                }

                for(vector<vector<int>>::iterator it = possible_mov.begin(); it != possible_mov.end(); it++)//varre todas as posicoes do tabuleiro
                {
                    if(t[it->at(0)][it->at(1)] == '0')//verifica se a posicao do tabuleiro esta vazia para poder realizar a possivel movimentacao
                    {

                        Tabuleiro new_board = this->getBoard();
                        //gera um possivel tabuleiro com a possivel movimentacao a partir da posicao atual do jogo
                        new_board[it->at(0)] [it->at(1)] = player;
                        //quer dizer que o jogador se movimentou, ou seja, tem que zerar a posicao anterior dele
                        new_board[i][j] = '0';

                        possibleBoards.push_back(new_board);
                    }

                }
            }
        }
    }

    return possibleBoards;
}


//talvez aqui esteja faltando algo
void State::updateState(){
	if(this->playing_symbol == '1'){
		this->playing_symbol = '2';
	}else{
		this->playing_symbol = '1';
	}
}


// talvez tenha que mudar
void State::giveReward(int *rep){
	int result = this->winner(rep);

	if(this.finished) {
		if(result == 1){
			this->p1.feedReward(1.0);
			this->p2.feedReward(0.0);
		}else if(result == 2){
			this->p1.feedReward(0.0);
			this->p1.feedReward(1.0);
		}else if(result == 0){
			this->p1.feedReward(0.1);
			this->p2.feedReward(0.5);
		}
	}
	return;
}

void State::reset(){
	this->setBoard(initialBoard());
	this->finished = false;
	this->playing_symbol = '1';
}

void play_train(int rounds){

}

void play_human(){

}

boolean State::isFinished() {
	return this.finished;
}
