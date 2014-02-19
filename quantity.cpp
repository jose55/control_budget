#include <iostream>
#include <set>
#include <vector>
using namespace std;


class partidas
{
  string strNumberPartida;
  int intNumberPartida;
  string unity;
  string description;
  int measurement;
  int price;
  int ammount;

public:
  partidas(string snp,string u,string d,int m,int p,int a):strNumberPartida(snp),unity(u),description(d),measurement(m),price(p),ammount(a){};
  partidas(){};
  void strToIntPartida();

};


struct order
{

  bool operator()(const partidas& el,const partidas& er)
  {
    return true;
  }

};


void menu();


set<partidas,order> SetOfPartidas;
vector<set<partidas,order>> ContainerOfMonths;


int main()
{

  while(1)
    {
      menu();
      int option;
      cin>>option;

      switch(option)
	{
	case 1:
	  {
	  //creates a new object(partida) and adds to the containers
	  cout<<"Number Partida: ";
	  string np;
	  cin>>np;
	  cout<<"\nUnity: ";
	  string u;
	  cin>>u;
  	  cout<<"\nDescription:";
	  string d;
	  cin.ignore();
	  getline(cin,d);
	  cout<<"\nMeasurement: ";
	  int m;
	  cin>>m;
	  cout<<"Price: ";
	  float p;
	  cin>>p;
	  float ammount=m*p;

	  partidas currentPartida(np,u,d,m,p,ammount);

	  //stores the partida in the 2 containers  (they are global)
	  SetOfPartidas.insert(currentPartida);
	  ContainerOfMonths.push_back(SetOfPartidas);

	  //currentPartida desapear because is local to this block
	  }
	  break;
	case 2:

	  break;


	}



    }

  return 0;
}

void menu()
{
  cout << "menu" << endl;



}
