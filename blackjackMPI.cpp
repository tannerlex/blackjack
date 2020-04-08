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

using namespace std;

int main(int argc, char **argv){
    int rank, size;
    Hand myHand;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MCW, &rank);
    MPI_Comm_size(MCW, &size);

    if(rank==0){ // dealer
        // shuffle deck
        CardDeck deck;
        Hand sendHand;
        int dealtCards[2];

        cout << "Shuffling deck... ";
        deck.shuffle();
        deck.printDeck();

        myHand.m_hand.push_back(deck.deal());
        myHand.m_hand.push_back(deck.deal());
        sendHand.m_hand.push_back(deck.deal());
        sendHand.m_hand.push_back(deck.deal());

        // deal to all player
        for(int i=1; i<size; i++){
            dealtCards[0] = sendHand.m_hand[0].getUCV();
            dealtCards[1] = sendHand.m_hand[1].getUCV();
            MPI_Send(dealtCards,2,MPI_INT,i,DEALTAG,MCW);
        }
    }

    else {
        int recvHand[2];
        MPI_Recv(&recvHand,2,MPI_INT,0,DEALTAG,MCW,MPI_STATUS_IGNORE); // each processor receives a split of the data

        cout << recvHand[0] << " " << recvHand[1] << "\n";
        Card firstCard(recvHand[0]);
        Card secondCard(recvHand[1]);
        myHand.m_hand.push_back(firstCard);
        myHand.m_hand.push_back(secondCard);
        myHand.update();

        cout << "Rank: " << rank << " recieved hand: " << endl;
        cout << myHand.m_hand[0].printCard() << " " << myHand.m_hand[1].printCard() << endl;
    }

    MPI_Finalize();
    return 0;
}