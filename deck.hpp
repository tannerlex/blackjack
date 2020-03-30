#include <iostream>
#include <time.h> /* used to seed rand with current time */
#include <vector>
using namespace std;

// #define DECKSIZE 52

/* unicode values for playing card characters */
#define BACK "🂠"
#define SPADE 0x1F0A0
#define HEART 0x1F0B0
#define DIAMOND 0x1F0C0
#define CLUB 0x1F0D0
#define ACE 0x1
#define JACK 0xB
#define KNIGHT 0xC
#define QUEEN 0xD
#define KING 0xE
#define SUITROW 0x10

// enum Suit {
//   spades,
//   hearts,
//   diamonds,
//   clubs,
//   maxSuit
// };

// enum Face {
//   ace,
//   two,
//   three,
//   four,
//   five,
//   six,
//   seven,
//   eight,
//   nine,
//   ten,
//   jack,
//   queen,
//   king,
//   maxFace
// };

class Card {
  public:
  Card();
  Card(int unicodeVal)
    : m_unicodeVal(unicodeVal)
  {}

  string printCard();
  string printBack();
  int m_unicodeVal;

  private:
  string m_face;
  string m_suit;
};

class CardDeck {
  public:
  CardDeck(){
    int i = 0;
    for(int suit = SPADE; suit <= CLUB; suit += SUITROW){
      for(int face = ACE; face <= KING; ++face){
        if(face == KNIGHT){++face;}
        Card card(suit + face);
        m_deck.push_back(card);
        cout << m_deck[i].printCard() << endl;
      }
    }
  }
  void shuffle();
  Card deal();
  vector<Card> m_deck;

  private:
};
