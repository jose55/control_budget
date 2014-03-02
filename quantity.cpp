#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <string.h>
using namespace std;

enum print{detailed,single};
ofstream file;
ifstream ifile;

const int empty=0;




class partidas
{
  
  string strNumberPartida;
  string unity;
  string description;


  //we'll save in in a file apart from month..  file named budget
  // char c_strNumberPartida[5];
  //char c_unity[5];
  //char c_description[200];

  float measurement;
  float price;
  float ammount;
  int size(const char* chain)const
  {
    int count=0;
    char a='k';//to initialize
    while(a)
      {
	a=*(chain+count++);
	int l=0;
      };
     return count;
  }
    
  inline  void save(const char* reader)const
  {
    int s=size(reader);
    file.write(reinterpret_cast<const char*>(&s),4);
    file.write(reinterpret_cast<const char*>(reader),size(reader));
  }

inline  void save(float reader)const{  file.write(reinterpret_cast<const char*>(&reader),sizeof(reader));}

  inline  void save(int reader)const{  file.write(reinterpret_cast<const char*>(&reader),sizeof(reader));}

public:

 void strToIntPartida();

  int intNumberPartida;
  void load(string&);
  partidas(string);
  partidas(char);
  partidas()
  {
    strNumberPartida=10;//probably reserves 10 characters
    unity=10;
    description=100;

  }
  partidas(int empty)
  {
    strNumberPartida=10;//probably reserves 10 characters
    unity=10;
    description=100;

  }
  
 

  //2 types detailed when you see only one partida completely.
  //single you don't see the full descrition; (you want to list)
  void printPartida(print type=detailed)const;
 
  void savePartida()const;  
  void loadPartida();

};

struct budget
{
  char c_strNumberPartida[5];
  char c_unity[5];
  char c_description[200];
  int intNumberPartida;
};

struct orderBudget
{

  bool operator()(const budget& left,const budget& right)
  {

    return left.intNumberPartida < right.intNumberPartida;
  }


};
struct order
{
  bool operator() (const partidas& el,const partidas& er)
  {
    return el.intNumberPartida < er.intNumberPartida;
  }
};

void menu();


//char partidas::c_strNumberPartida[5];
//char partidas::c_unity[5];
//char partidas::c_description[200];

set<partidas,order> SetOfPartidas;
set<partidas,order>::iterator iSetOfPartidas;
vector<set<partidas,order>> ContainerOfMonths;
vector<set<partidas,order>>::iterator iContainerOfMonths;
set<budget,orderBudget> SetOfDescriptions;
set<budget,orderBudget>::iterator iSetOfDescriptions;
	  budget partidaOnlyDescription;    

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
	    //we pass as parameter any character
	    partidas currentPartida('c');
	    SetOfDescriptions.insert(partidaOnlyDescription);

	    //stores the int of the partida
	    currentPartida.strToIntPartida();
	    partidaOnlyDescription.intNumberPartida=currentPartida.intNumberPartida;
	    
	    //stores the partida in the 2 containers  (they are global)
	    SetOfPartidas.insert(currentPartida);
	    
	    //we've modify the set of partidas that there was so we delete wich
	    //was so far..
	    if(ContainerOfMonths.size())
	      ContainerOfMonths.pop_back();
	    
	    //... and push the new one
	    ContainerOfMonths.push_back(SetOfPartidas);
	    
	    //currentPartida desapear because is local to this block
	  }
	  break;
	case 2:

	  break;

	case 3:
	  {
	    //print single partida completely
	    cout<<"Which partida do you want to print? ";
	    string partida;
	    cin>>partida;

	    partidas partidaToCompare(partida);

	    //finds out the value of the int
	    //needed to order ... or to find
	    partidaToCompare.strToIntPartida();

	    iSetOfPartidas=SetOfPartidas.find(partidaToCompare);
	 
	    if(iSetOfPartidas!=SetOfPartidas.end())
	      iSetOfPartidas->printPartida();// for it is CONST !!!!
		//		partidaToCompare=*iSetOfPartidas;
		//		partidaToCompare.printPartida();
		//****** VERY IMPORTANT *******
	      
	      
	    else
	      cout << "There's not such a partida"<<endl;

	  }

	  break;
	case 4:
	  //save month
	  {
	    size_t numberPartidas=SetOfPartidas.size();
	    string nameFile;
	    cin >> nameFile;
	    file.open(nameFile,ios_base::out | ios_base::binary);

	    if(file.is_open())
	      {
		//first.. the number of partidas
		file.write(reinterpret_cast<const char*>(&numberPartidas),sizeof(numberPartidas));

		  iSetOfPartidas=SetOfPartidas.cbegin();

		  while(iSetOfPartidas!=SetOfPartidas.cend())
		    {
		      iSetOfPartidas -> savePartida(); 
		      ++iSetOfPartidas;
		    }
		      }
	    file.close();

	  }
	  break;

	case 5:
	  //load month to SetOfPartidas 
	  //and the push_back to the general container (vector)
	  {
	    string fileName;
	    cin>>fileName;
	    
	    ifile.open(fileName, ios_base::in | ios_base::binary);

	    //empties the whole set
	    SetOfPartidas.erase(SetOfPartidas.begin(),SetOfPartidas.end());

	    //we are going to load the data of one single partida
	    //in the next object
	    partidas loadingPartida(empty);

	    if(ifile.is_open())
	      {
		//first..  load how many partidas:
		int numberOfPartidas;
		ifile.read((char*)&numberOfPartidas,4);

		//loop with partidas
		while(numberOfPartidas--)
		  {
		    //load a single partida...
		    loadingPartida.loadPartida();

		    //insert this loadingPartida in the SetOfPartidas
		    SetOfPartidas.insert(loadingPartida);
		  }

		auto i=SetOfPartidas.begin();



		//push_back on the vector.. general container
		ContainerOfMonths.push_back(SetOfPartidas);
	      }

	    ifile.close();
	  }
	  break;

	case 6:
	  //list
	  iSetOfPartidas=SetOfPartidas.cbegin();

	  while(iSetOfPartidas!=SetOfPartidas.cend())
	    iSetOfPartidas++ -> printPartida(single);

	  break;

	}
    }

  return 0;
}

void menu()
{
  cout << "1 Add new partida" << endl;
  cout << "2 Delete Partida" << endl;
  cout << "3 Print on screen partida" << endl;
  cout << "4 Save current budget of the current month" << endl;
  cout << "5 Load a budget" << endl;
  cout << "6 List partidas of the current budget" << endl;
  cout << "7 Comparing list of two budgets" << endl;
  cout << "8 Change current month" << endl;

  cout << "\nChoose: " << endl;


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


partidas::partidas(char)
{


  cout<<"Number Partida: ";
  cin>>strNumberPartida;
  //  strcpy(c_strNumberPartida,strNumberPartida.c_str());
  strcpy(partidaOnlyDescription.c_strNumberPartida,strNumberPartida.c_str());


  cout<<"\nUnity: ";
  cin>>unity;
  //strcpy(c_unity,unity.c_str());
  strcpy(partidaOnlyDescription.c_unity,unity.c_str());


  cout<<"\nDescription:";
  cin.ignore();
  getline(cin,description);
  //  strcpy(c_description,description.c_str());
  strcpy(partidaOnlyDescription.c_description,description.c_str());

  cout<<"\nMeasurement: ";
  cin>>measurement;
  cout<<"Price: ";
  cin>>price;
  ammount=measurement*price;
  cout<<"\nAmmount: "<<ammount<<endl;

  //now, we want to save the 3 strings (but c_style) in file: budget 

  file.open("budget",ios_base::out | ios_base::binary | ios_base::app);
  if(file.is_open())
    {
      save(strNumberPartida.c_str());
      save(unity.c_str());
      save(description.c_str());
    }

  file.close();
}



partidas::partidas(string partida)
{
  strNumberPartida=partida;
}


void partidas::printPartida(print type) const
{
  iSetOfDescriptions=SetOfDescriptions.cbegin();

  //one object of struct budget// one partida only descriptions.
  partidaOnlyDescription=*iSetOfDescriptions;

  cout << partidaOnlyDescription.c_strNumberPartida << " ";
  cout << partidaOnlyDescription.c_unity << " ";
  cout << partidaOnlyDescription.c_description << endl;


  /*
  cout<<endl;
  cout<<strNumberPartida;
  if(type==single)
    {
      cout<<"  ";
    }
  else cout<<endl;


  cout<<unity;
  if(type==single)
    {
      cout<<"  ";
    }
  else cout<<endl;

  string shortDescription(description,1);
  cout<<shortDescription;
  if(type==single)
    {
      cout<<"  ";
    }
  else cout<<endl;
  */
  cout<<measurement<<" "<<price<<" "<<ammount<<endl;

}


void partidas::savePartida()const
{

  //we don't have to save strings
  //we have to save c_style char !!!!!! c_str().. !!!!!!!!!!!!!!!!!!!!!

  string reader;
  const char* c_reader;

  reader=iSetOfPartidas -> strNumberPartida;
  c_reader=reader.c_str();
  save(c_reader);

  reader=iSetOfPartidas -> unity;
  c_reader=reader.c_str();
  save(c_reader);


  reader=iSetOfPartidas -> description;
  c_reader=reader.c_str();
  save(c_reader);
		

  float fReader;

  fReader=iSetOfPartidas -> measurement;
  save(fReader);

  fReader=iSetOfPartidas -> price;
  save(fReader);

  fReader=iSetOfPartidas -> ammount;
  save(fReader);	
	
  int iReader=iSetOfPartidas -> intNumberPartida;
  save(iReader);


}


void partidas::load(string& field)
{
  const char* c_reader=NULL;
  
  int numberOfCharacters;
  ifile.read((char*)&numberOfCharacters,4);
  c_reader=new char[numberOfCharacters]; 
  ifile.read((char*)c_reader,numberOfCharacters);
  string aux(c_reader);
  delete c_reader;
  copy(aux.cbegin(),aux.cend(),field.begin());//only for a string

}


void partidas::loadPartida()
{

  load(strNumberPartida);//passes the value by reference
  load(unity);
  load(description);
  
  ifile.read((char*)&measurement,4);
  ifile.read((char*)&price,4);
  ifile.read((char*)&ammount,4);
  ifile.read((char*)&intNumberPartida,4);
  
}
