#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <string.h>
#include <algorithm>

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

  void saveBudget();

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
  static  void loadPartida(char);//load the budget
  const void edit(char);

};

struct budget
{
  char c_strNumberPartida[5];
  char c_unity[5];
  char c_description[200];
  int intNumberPartida;
};

typedef budget theBudget;//both are the same

struct orderBudget
{

  bool operator()(const budget& left,const budget& right)
  {

    return left.intNumberPartida < right.intNumberPartida;
  }
  
  bool operator== ( const budget& a    )
  {
    return true;
  }
  

};
struct order
{
  bool operator() (const partidas& el,const partidas& er)
  {
    return el.intNumberPartida < er.intNumberPartida;
  }

  
};

void menu();//we are declaring a global function here


//char partidas::c_strNumberPartida[5];
//char partidas::c_unity[5];
//char partidas::c_description[200];
partidas auxPartida;
set<partidas,order> SetOfPartidas;
set<partidas,order>::iterator iSetOfPartidas;
vector<set<partidas,order>> ContainerOfMonths;
vector<set<partidas,order>>::iterator iContainerOfMonths;

multiset<budget,orderBudget> SetOfDescriptions;
multiset<budget,orderBudget>::iterator iSetOfDescriptions;
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
	    
	    //currentPartida desappear because is local to this block
	  }
	  break;

	case 2:

	  break;

	case 3:
	  {
	    //print single partida completely and edit it
	    cout<<"Which partida do you want to print? ";
	    string partida;
	    cin>>partida;

	    partidas partidaToCompare(partida);

	    //finds out the value of the int
	    //needed to order ... or to find
	    partidaToCompare.strToIntPartida();

	    iSetOfPartidas=SetOfPartidas.find(partidaToCompare);

	    partidaOnlyDescription.intNumberPartida=partidaToCompare.intNumberPartida;

	    iSetOfDescriptions=SetOfDescriptions.find(partidaOnlyDescription);
	 
	    if(iSetOfPartidas!=SetOfPartidas.end())
	      {
		cout << iSetOfDescriptions -> c_strNumberPartida << " ";
		cout << iSetOfDescriptions -> c_unity << " ";
		cout << iSetOfDescriptions -> c_description << endl;

		iSetOfPartidas->printPartida();// for it is CONST !!!!
		//		partidaToCompare=*iSetOfPartidas;
		//		partidaToCompare.printPartida();
		//****** VERY IMPORTANT *******
	      }
	      
	    else
	      cout << "There's not such a partida"<<endl;
	    
	    cout << "Edit anything...? " << endl;
	    
	    cout << "<u> unity <d> description <m> measurement <p> price <a> amount ";

	    char result;
	    
	    cin >> result;

	    partidaToCompare.edit (result);



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
	  //and then push_back to the general container (vector)
	  
	    cout << "Main budget [b] or expecific month [m]? ";
	    char answer;
	    cin >> answer;

	    if(answer=='b')
	      
		//overload loadpartida for loading the  budget
		//load partida in partidaOnlyDescrition  from here (is global)
		//insert that partida in SetOfDescriptions after erasing everything
	      //the next method with any character is to load the budget.
		partidas::loadPartida('b');

	    else{ 
		string fileName;
		cin>>fileName;
		
		ifile.open(fileName,ios_base::in | ios_base::binary);

	
		if(ifile.is_open())
		  {

		    //empties the whole set
		    SetOfPartidas.erase(SetOfPartidas.begin(),SetOfPartidas.end());
		    
		    //we are going to load the data of one single partida
		    //in the next object
		    partidas loadingPartida(empty);
	      	
		    
		    //first..  load how many partidas:
		    int numberOfPartidas;
		    ifile.read((char*)&numberOfPartidas,4);
		    
		    //loop with partidas
		    while(numberOfPartidas--)
		       //while(ifile.good())
		      {
			//load a single partida...
				loadingPartida.loadPartida();
			
			//insert this loadingPartida in the SetOfPartidas
				SetOfPartidas.insert(loadingPartida);
		      }
		    	
		     // auto i=SetOfPartidas.begin();
		    
		    //push_back on the vector.. general container
		    ContainerOfMonths.push_back(SetOfPartidas);

		    ifile.close();
		   		
		  }//end of file is open
	    }//end else
		
	  
	  
	  break;
	  
	case 6:
	  //list
	  iSetOfPartidas=SetOfPartidas.cbegin();
	  iSetOfDescriptions=SetOfDescriptions.cbegin();

	  while(iSetOfPartidas!=SetOfPartidas.cend())
	    {
	      //how many partidas are there?? the same type
	      //we have to print the last one
	      //we have to code the binary predicate
	      //oveloaded operator == ,  if we want this funtion to work 

	      partidaOnlyDescription=*iSetOfDescriptions;	      

	      size_t times=0;
	      //  auto aux_iSetOfDescriptions=iSetOfDescriptions;
	      	      
	            
	      do
		{
		  int g=9;
		
		 times=count_if(   iSetOfDescriptions++   ,SetOfDescriptions.cend(),[](const budget& part){ return part.intNumberPartida==partidaOnlyDescription.intNumberPartida;});

		 int c=55;
			  
		}

	      while(times > 1);

	      --iSetOfDescriptions;
	      


	      cout << iSetOfDescriptions -> c_strNumberPartida << " ";
	      cout << iSetOfDescriptions -> c_unity << " ";
	      cout << iSetOfDescriptions -> c_description << endl;




	      ++iSetOfDescriptions;

	      iSetOfPartidas++ -> printPartida(single);
	    }
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

  strToIntPartida();
  //  strcpy( partidaOnlyDescription.c_strNumberPartida,strNumberPartida.c_str());
  partidaOnlyDescription.intNumberPartida=intNumberPartida;

  cout<<"\nMeasurement: ";
  cin>>measurement;
  cout<<"Price: ";
  cin>>price;
  ammount=measurement*price;
  cout<<"\nAmmount: " << ammount << endl;

  //now, we want to save the 3 strings (but c_style) in file: budget 

  SetOfDescriptions.insert(partidaOnlyDescription);

  saveBudget();
  
 

  /*
  file.open("budget",ios_base::out | ios_base::binary | ios_base::app);
  if(file.is_open())
    {
      save(strNumberPartida.c_str());
      save(unity.c_str());
      save(description.c_str());
     

      save(intNumberPartida);

      file.close();

          
    }
  */
 
}


partidas::partidas(string partida)
{
  strNumberPartida=partida;
}


void partidas::printPartida(print type) const
{
  // iSetOfDescriptions=SetOfDescriptions.cbegin();

  //one object of struct budget// one partida only descriptions.
  /*
  while(iSetOfDescriptions!=SetOfDescriptions.cend())
    {
      partidaOnlyDescription=*iSetOfDescriptions++;
      
      cout << partidaOnlyDescription.c_strNumberPartida << " ";
      cout << partidaOnlyDescription.c_unity << " ";
      cout << partidaOnlyDescription.c_description << endl << endl;
   
 
    }
  */
     cout<<measurement<<" "<<price<<" "<<ammount<<endl;
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
  //copy(aux.cbegin(),aux.cend(),field.begin());//only for a string

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

void partidas::loadPartida(char budget)
{
  //we have to call this method like this:   loadPartida('b')
  //b o any other letter

  //we are going to fill partidaOnly description
  //then insert in SetOfDescriptions

  vector<theBudget> auxBudget;  

  ifile.open("budget",ios_base::in | ios_base::binary);

    if(ifile.is_open())
      {
	int intOfPartida=0;
	

	while(ifile.good())
	  {
	    int amount;

	    ifile.read((char*)&amount,4);
	    ifile.read((char*)partidaOnlyDescription.c_strNumberPartida,amount);
	    
	    ifile.read((char*)&amount,4);
	    ifile.read((char*)partidaOnlyDescription.c_unity,amount);
	    
	    ifile.read((char*)&amount,4);
	    ifile.read((char*)partidaOnlyDescription.c_description,amount);

	    ifile.read((char*)&partidaOnlyDescription.intNumberPartida,4);

	    //	    partidaOnlyDescription.intNumberPartida=intOfPartida++;
	 
	    //now it is multiset so ... we are loading several of the same partida if there were   
	        SetOfDescriptions.insert(partidaOnlyDescription);
	    
	  }

	ifile.close();
      }
    
    }

const void partidas::edit(char c)
{
  //we pass what we are going to edit and change out of the container to an object
  partidaOnlyDescription=*iSetOfDescriptions;
  auxPartida=*iSetOfPartidas;

  //once they are in the object out of the container we can delete them in the container.
  //later we'll insert the object changed
  /*
  SetOfDescriptions.erase(*iSetOfDescriptions);
  SetOfPartidas.erase(*iSetOfPartidas);
  */

  switch(c)
    {
      
    case 'u':
      	char n[5];
	cin >> n;
	
	strcpy(partidaOnlyDescription.c_unity,n);


	SetOfDescriptions.erase(*iSetOfDescriptions);
	SetOfDescriptions.insert(partidaOnlyDescription);

	//now we have to save in hd
	saveBudget();//maybe this partida is repited
	
      
      break;
      
    case 'd':
      {
	cin.clear();
	cin.ignore();
	string aux;
	getline(cin,aux);
	strcpy(partidaOnlyDescription.c_description,aux.c_str());

	SetOfDescriptions.erase(*iSetOfDescriptions);
	SetOfDescriptions.insert(partidaOnlyDescription);
	

	

 file.open("budget",ios_base::out | ios_base::binary | ios_base::app);
  if(file.is_open())
    {
      save(strNumberPartida.c_str());
      save(partidaOnlyDescription.c_unity);
      save(partidaOnlyDescription.c_description);
      
      
      save(intNumberPartida);
      
      file.close();
      
      
    }
	

	//	saveBudget();//now maybe the partida is repited

      }
      break;

    case 'm':
      //measurement
      float m;
      cin >> m;
      auxPartida.measurement=m;

      auxPartida.ammount=m * auxPartida.price;

      SetOfPartidas.erase(*iSetOfPartidas);
      SetOfPartidas.insert(auxPartida);

      break;

    case 'p':
      //price
      float p;
      cin >> p;
      auxPartida.price=p;

      auxPartida.ammount=auxPartida.measurement * p;

      SetOfPartidas.erase(*iSetOfPartidas);

      SetOfPartidas.insert(auxPartida);
    break;

    case 'a':
      //amount

      float a;
      cin >> a;

      auxPartida.ammount=a;


      auxPartida.price = a / auxPartida.measurement;

      SetOfPartidas.erase(*iSetOfPartidas);
      SetOfPartidas.insert(auxPartida);

      break;

    }



  
}


void partidas::saveBudget()
{

  file.open("budget",ios_base::out | ios_base::binary | ios_base::app);
  if(file.is_open())
    {
      save(strNumberPartida.c_str());
      save(unity.c_str());
      save(description.c_str());
      
      
      save(intNumberPartida);
      
      file.close();
      
      
    }

}
