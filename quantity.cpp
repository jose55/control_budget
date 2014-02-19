#include <iostream>
#include <set>
#include <vector>
#include <sstream>
using namespace std;


class partidas
{
  string strNumberPartida;
  string unity;
  string description;
  int measurement;
  int price;
  int ammount;

public:
 int intNumberPartida;
  partidas(string snp,string u,string d,int m,int p,int a):strNumberPartida(snp),unity(u),description(d),measurement(m),price(p),ammount(a){};
  partidas(){};
  void strToIntPartida();
 
  //this decides how to order the objects int the set.
  

};

struct order
{
  bool operator() (const partidas& el,const partidas& er)
  {
    return el.intNumberPartida < er.intNumberPartida;
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

	  //stores the int of the partida
	  currentPartida.strToIntPartida();

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

void partidas::strToIntPartida()
{
  const char* cstyle = strNumberPartida.c_str();

  //  string part(parti);//part contains "1.12\000T"
  
  size_t dot=strNumberPartida.find('.',0);
  
   if(dot >= strNumberPartida.size())
     throw "\nYou didn't write the dot !!!!\n";
  
  
      string   integer(cstyle,dot);//copies the integer part
      stringstream conv;
      conv << integer;
      int part_aux;
      
      conv >> part_aux;
      if(!part_aux)
	throw "\nLetters or chapter 0 are NOT permitted!\n";

      intNumberPartida = part_aux;
      intNumberPartida *= 100;

    
  //****  ok  integer  ***
  
  stringstream pass;
  string decimal(strNumberPartida,dot);
  float n;
  pass << decimal;
  pass >> n;
  size_t sss = decimal.size();
  --sss;
  int factor = 1;
  for(int x = 0 ; x < sss ; ++x)factor *= 10;
  n *= factor;
  int num=n;//gets the integer side
  intNumberPartida += num;


}
