#include <iostream>
#include <mpi.h>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <time.h>
#include <cstdlib>
#include <deque>

#include "deck.hpp"

#define MCW MPI_COMM_WORLD
#define DEALTAG 0
#define HITTAG 1
#define DLHOLD 16

using namespace std;

int dealCard(int player, CardDeck &deck){
    int dealtCard;
    Card sendCard(deck.deal());

    dealtCard = sendCard.getUCV();
    MPI_Send(&dealtCard,1,MPI_INT,player,DEALTAG,MCW);

    return dealtCard;
}

void recvCard(Hand &curHand){
    int card;
    MPI_Recv(&card,1,MPI_INT,0,DEALTAG,MCW,MPI_STATUS_IGNORE); // each processor receives a split of the data
    Card recvCard(card);
    curHand.m_hand.push_back(recvCard);
    curHand.update();
}

int main(int argc, char **argv){
    int rank, size;
    srand(time(0));

    // hold[rank] = true: player is holding
    bool hold[size]; // hold[0] = true means all players are holding
    Hand myHand;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MCW, &rank);
    MPI_Comm_size(MCW, &size);

    if(rank==0){ // dealer
        // shuffle deck
        CardDeck deck;

        cout << "Shuffling deck... ";
        deck.shuffle();
        deck.printDeck();

        myHand.m_hand.push_back(deck.deal());
        myHand.m_hand.push_back(deck.deal());

        /* initial deal to all players */
        for(int i=1; i<size; i++){
            dealCard(i,deck);
            dealCard(i,deck);
        }

        for(int i=0;i<size;i++){ // set all players to hit
            hold[i] = false;
        }

        while(!hold[0]){
            hold[0] = true;
            for(int i=1;i<size;i++){
                if(!hold[i]){
                    cout << "check player " << i << " for hit\n";
                    int hit;
                    MPI_Recv(&hit,1,MPI_INT,i,HITTAG,MCW,MPI_STATUS_IGNORE); // each processor receives a split of the data
                    hold[i] = !hit;
                    if(!hold[i]){
                        cout << "dealing card to player: " << i << "\n";
                        dealCard(i,deck);
                        hold[0] = false;
                    }
                }
            }
            cout << "Hold[0] while loop: " << hold[0] << endl;
        }

        /* dealer play */
        while(myHand.hit(DLHOLD)) {
            /* deals a card to itself */
            myHand.m_hand.push_back(deck.deal());
            myHand.update();
        }
        cout << "Dealer's cards: ";
        myHand.printHand();
        cout << "value: " << myHand.getVal() << "\n";
    }

    else {
        recvCard(myHand);
        recvCard(myHand);

        myHand.printHand();
        
        int hit = myHand.hit(16);

        while(hit){
            MPI_Send(&hit,1,MPI_INT,0,HITTAG,MCW);
            recvCard(myHand);
            hit = myHand.hit(16);
        }

        cout << "Rank: " << rank << " recieved hand: " << endl;
        myHand.printHand();
        cout << "value: " << myHand.getVal() << "\n";
    }

    cout << rank << " about to finalize \n";

    MPI_Finalize();
    return 0;
}