#include <iostream>
#include <iomanip>
#include <algorithm>
#include <set>
#include <vector>
#include <sstream>
#include <fstream>
#include <string.h>
#include <termios.h>

using namespace std;

class line
{
protected:
 
  static char month[20];
  char unity[5];
  char concept[50];
  float measurement;
  float price;
  float import;
public:

  void find_partida(char [5]);//work out the integer

  //char partida[4];
  int partida;
  char parti[5];
  virtual  void enter_data()=0;
  virtual void print_partida(const line&)=0;
  static char menu();
  virtual void  save_file()=0;
  virtual void load_partida()=0;
  virtual  void list()=0;
  static  void li();
  virtual  void operator()(const line& e)=0;//functor
  virtual void edit()=0;
   virtual void enterMonth(char m[20])=0;
  
};


class pepe_line:public line//implementation of the former class
{

public:
  void enter_data();
  void print_partida(const line&);
  void save_file();
  void load_partida();
  void list();
  void enterMonth(char m[20]);
  void operator()(const line& e)
  {
    cout<<e.parti<<"  ";
  
  };

  void edit();
};
 termios old_t,new_t;
void unbuffered()
{
tcsetattr(STDIN_FILENO,TCSANOW,&new_t);//set new settings unbuffered
}
void buffered()
{
// restore the former settings (keyboard no bufferd)
        tcsetattr(STDIN_FILENO,TCSANOW,&old_t);

}

struct order
{
  
  bool operator ()(const pepe_line& l,const pepe_line& r)const
  {

    return (l.partida<r.partida);  
  }
  

};
//global variables

char line::month[]="INITIAL BUDGET";
line* main1;
pepe_line main2;
set<pepe_line,order> cont_main2;
set<pepe_line,order>::iterator p;

vector<set<pepe_line,order>> project;//we're going to store monthly project
vector<set<pepe_line,order>>::iterator p_project; 
fstream file;

int main()
{
  
 
  //the next few lines are to set unbuffered cin 
  //it means you don't have to press [enter] in the keyboard
  //
  tcgetattr(STDIN_FILENO,&old_t);//gets the current terminal setings.

  new_t=old_t;

  new_t.c_lflag &=(~ICANON & ~ECHO);//disable ...

  tcsetattr(STDIN_FILENO,TCSANOW,&new_t);//set new settings unbuffered

  // *****************************************************



  size_t number_partidas;
  size_t number_projects;

  while(1)
    {
      main1=&main2;
      char option=main1->menu();
      bool ok;
      switch(option)
	{

	case 'w':
	  //write a new partida
	  /* restore the former settings */
	  tcsetattr(STDIN_FILENO,TCSANOW,&old_t);//buffered keyboard
	  ok = true;
	  try
	    {
	      main1 -> enter_data ();//we've filled a new object
	    }
	  catch(const char* e)
	    {
	      cout << endl << e << endl;
	      ok = false;
	    } 

	  if(ok) cont_main2.insert(main2);
	  else cout << "I have inserted no partida";
	  tcsetattr(STDIN_FILENO,TCSANOW,&new_t);//set new settings unbuffered
 	  break;



	case 'p':
	  //see a specific partida
	  cout << "\n\n";
	  //	  string part=main2.partida;
	  
	  /* restore the former settings */
	  tcsetattr(STDIN_FILENO,TCSANOW,&old_t);//keyboard buffered

	  cin>>main2.parti;
	      

	  tcsetattr(STDIN_FILENO,TCSANOW,&new_t);//set new settings unbuffered


	  //converses string to float
	  main2.find_partida(main2.parti);//find out the actual number
	  
	  cout << fixed << setprecision(2);//only 2 decimals
	  //	  main2.find_partida(main2.partida);

	  p=cont_main2.find(main2);
	  if(p != cont_main2.cend())
	    {
	      main2 = *p;	 
	      main1 = &main2;
	      
	      //main1->print_partida(main2);//p contains a object (partida
	      main1 -> print_partida(main2);//p contains a object (partida
	      cout << "\nEdit ? (y/n)";
	      char ed;
	      cin >> ed;
	      if(ed == 'y' || ed == 'Y')main1->edit();
	      
	    }
	  else cout << "\n\nNot found !!!!" << endl;
	  break;


	  
	case 'e':
	  //edit a specific partida
	  break;


	case 's':
	  //we want to save th whole set, not only one object

	  file.open("prevision.bin",ios_base::out | ios_base::binary);
	  if(file.is_open())
	    {
	      number_partidas = cont_main2.size();
	      //the first byte in disk will be the number of objects/partidas.. next the objects
	      file.write(reinterpret_cast<const char*>(&(number_partidas)),sizeof(number_partidas));

	      p=cont_main2.cbegin();//point to the begining of the vector (dinamic array)
	      
	      for(size_t x = 0 ; x < number_partidas ; ++x)
		{
		  main2 = *p;
		  main1 -> save_file();//p contains an object a partida...saves ONE object/partida
		  ++p;//next object ... next partida
		}
	    }
	  file.close();
	  cout << "\n*** FILE SAVED ***\n";
	  break;
	 
	case 'l':
	  file.open("prevision.bin",ios_base::in | ios_base::binary);
	  if(file.is_open())
	    {
	      
	       cont_main2.erase(cont_main2.begin(),cont_main2.end());
	       int n;//we read the number of objects/partidas
	       file.read((char*)&n,sizeof(n) );

	      for(int x = 0;x < n; ++x)
		main1 -> load_partida();
	
	    }

	    file.close();

	    project.push_back(cont_main2);
	    cout << "\n*** FILE LOADED ***\n";
	  break;

	case 'i':

	   number_partidas = cont_main2.size();
	   p_project = project.begin();
	   //	  	  p=cont_main2.begin();

		  cout << endl << "List:" << endl;
		  number_projects = project.size();

		  for(size_t proj = 0 ; proj < number_projects ; ++proj)
		    {
		      p = (*p_project).begin();

		      for(size_t x = 0 ; x<number_partidas ; ++x)
			{
			  main1 -> list();//writes one line/partida
			  ++p;
			}
		      cout << endl << endl;
		      ++p_project;
		    }

	  cout << "\n\033[0;0mWould you like to create a monthly prevision from this list?" << "\033[?25h";

	  char a;
	  cin.clear();
	  cout << "\033[?25l";
	  cin >> a;

	  if(a =='y' || a =='Y')
	    {

	      project.push_back(cont_main2);
	      //now we should save this container of containers on HDD
	      p_project=--project.end();//points past end so --
	      //no it point to the last set (the last budget)

	      cont_main2 = *p_project;//a contains a set a month
	      

	      //we go to the begin(). The firs partida of this new monthly
	      //budget.. Because the variable "month" is static.
	      //only storing in the first line we are storing the month of
	      //every partida.

	      main2 = *cont_main2.begin();//main 2 contains a line a single partida
	      main1 = &main2;
	   
	      cout << "\033[?25h";

	      char monthLastPrevision[] = "MArch";

	      buffered();

	      cout << "\nWhat month? ";

	      cin >> monthLastPrevision;

	      //we enter this month to every line , every partida.It's static!!
	      main1 -> enterMonth (monthLastPrevision);

	      unbuffered();//cin unbuffered  (keyboard)

	      cout << "\033[?25l";//hide cursor
	    }
	    
	  break;
	case 't'://experimental lisT LAMBDA FUNCTION
	  for_each(p = cont_main2.cbegin(),cont_main2.cend(),[](const line& e){e.li();});
	  break;
	
    case 'y'://experimental with function object (functor)

      cout << "\nPartidas:" << endl;
      for_each(p = cont_main2.cbegin(),cont_main2.cend(),pepe_line());

      break;
	}
    }

  // restore the former settings (keyboard no bufferd)
        tcsetattr(STDIN_FILENO,TCSANOW,&old_t);

  return 0;
}


void pepe_line::enter_data()
{
  

  string clear;

  cout << "\nPartida: ";
 ;
  cin >> parti;

  find_partida(parti);//work out the integer
  
  cout << "\nMeasurement unity: ";
  
  cin >> unity;
  cin.clear();   
  cout << "\nConcept: ";
  cin.ignore();
  getline(cin,clear);
  strcpy(concept,clear.c_str());

  float aux_measurement = 0.0f;
  while(!aux_measurement)
    {
      cout << "\nMeasurement: ";
      cin.clear();
      cin.ignore();
      cin >> aux_measurement;
      cout << "\033[1A";
    
    }
  cout << endl;
  measurement = aux_measurement;

  cout << "\nprice: ";
  cin.clear();
  cin >> price;

  cout << "\nImport: ";
  
  import = measurement*price;
  cout<<import<<endl<<endl<<"*******"<<endl;
  cin.clear();
  //  getline(cin,clear);

  
}

char line::menu()
{

  static int count = 0;//this is as if it were global
  cout << "\033[?25l";//hide cursor
  cout << "\033[0;0m";
  if(count)
    {
      cout << "\n...PRESS ANY KEY...";
      char a;
      cin >> a;
     
    }


  if(!count)
    {
      cout << "\033[2J"<<"\033[1;1H";

    }
  ++count;
  //  cout<<"\033[2J"<<"\033[1;1H";
  char option;
  cout << "\n\n*** MENU ***"<<endl<<endl;
  cout << "<w> Write a new partida"<<endl;
  cout << "<p> Print a specific partida"<<endl;
  cout << "<s> Save file"<<endl;
  cout << "<l> Load file"<<endl;
  cout << "<i> List partidas"<<endl;
  cout << "<y> Only partidas' number"<<endl;
  cin.clear();
  cin >> option;
 cout << "\033[?25h";//show cursor
  return option;
}

void pepe_line::print_partida(const line& partida)
{
  
 cout << "\033[0;33m";
  cout << endl << this -> parti << " " << unity << " " << concept << endl;
  cout << "MEDICION PRECIO IMPORTE" << endl;
  cout << setw(8) << measurement << setw(7) << price << setw(8) << import << endl;

}

void line::find_partida(char p[5])//"1.23\000t"
{

  const char* cstyle = p;
  string part(parti);//part contains "1.12\000T"
  
  size_t dot=part.find('.',0);
  
   if(dot >= part.size())
     throw "\nYou didn't write the dot !!!!\n";
  
  
      string   integer(cstyle,dot);//copies the integer part
      stringstream conv;
      conv << integer;
      int part_aux;
      
      conv >> part_aux;
      if(!part_aux)
	throw "\nLetters or chapter 0 are NOT permitted!\n";

      partida = part_aux;
      partida *= 100;

    
  //****  ok  integer  ***
  
  stringstream pass;
  string decimal(part,dot);
  float n;
  pass << decimal;
  pass >> n;
  size_t sss = decimal.size();
  --sss;
  int factor = 1;
  for(int x = 0 ; x < sss ; ++x)factor *= 10;
  n *= factor;
  int num=n;//gets the integer side
  partida += num;

}


void pepe_line::save_file()
{
  file.write(reinterpret_cast<const char*>(&(*p)),sizeof(*p));

}

 void pepe_line::load_partida()
 {

   //easier ...we save in disk the number of partidas 
   //and we'll know how many we have to get
   pepe_line aux;
   file.read((char*)&aux,sizeof(aux) );
   cont_main2.insert(aux);

 }


void pepe_line::list()
{
 cout << "\033[0;33m";
  //cout<<setiosflags(ios::fixed)<<setprecision(2)<<setw(25)<<(*p).measurement<<setw(8)<<(*p).price<<setw(8)<<(*p).import<<endl;
 cout << fixed << setprecision(2) << setw(25) << (*p).measurement << setw(8) << (*p).price << setw(8) << (*p).import << endl;

   cout << "\033[A";
   cout << (*p).parti << "  " << (*p).unity << "  " << setprecision(10);

   string aux((*p).concept,10);// only write the first 10 words of a string... jiji

cout << aux << "  "<< endl;
//cout<<(*p).concept<<"  "<<endl;
  }
void line::li()
{
cout << fixed << setprecision(2) << setw(25) << (*p).measurement << setw(8) << (*p).price << setw(8) << (*p).import << endl;

   cout << "\033[A";
  cout << (*p).parti << " " << (*p).unity << " " << setprecision(10) << (*p).concept << "  " << endl;
  ++p;
}


void pepe_line::edit()
{
 cout << "\033[0;31m";
  //we are in  object main2 
  //main2=*p;	 
  //main1=&main2;
  //iterator p points to our partida;
  string aux;
  cont_main2.erase(p);//because we are going to modify it
  //we have a copy in the object main2;
  cout << "\n<u> Unity <c> Concept <m> Measurement" << endl;
  cout << "<p> Price <i> Import" << endl;

  char ed;
  cout << "\033[?25l";//hide cursor
  unbuffered();
  cout << "Which?";

  cin.clear();
  //  cin.ignore();
  cin >> ed;
   cout << "\033[?25h";//show cursor
  buffered();
  switch(ed)
    {
    case 'u':

      cout << "\nNew unity:";
      
      cin >> unity;//
      
      //we have written in one object main2
      //now we have to put it in the container!!!!!
      break;

    case 'c':
      cout << "\nRewrite the concept again:" << endl;
      getline(cin,aux);
      strcpy(concept,aux.c_str());
      
      break;

    case 'm':
      cout << "\nNew measurement:" << endl;
      
      cin >> measurement;


      break;
    case 'p':
      cout << "\nNew price:" << endl;
      cin>>price;
    break;
    case 'i':
      cout << "\nNew import:" << endl;
      cin >> import;
      break;
    }

  unbuffered();
  cont_main2.insert(main2);//inserts the partida
  project.push_back(cont_main2);//adds a new object!!!

}

void pepe_line::enterMonth(char m[20])
{
  //it has to be like that because 'month' is protected
  strcpy(month,m);
}
