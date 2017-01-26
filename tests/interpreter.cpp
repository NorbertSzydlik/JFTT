/*
* Kod interpretera maszyny rejestrowej do projektu z JFTT2015
*
* Autor: Maciek Gębala
* http://mgc.im.pwr.wroc.pl/
* 2015-11-14
* (wersja CLN)
*/

#include<tuple>
#include<vector>
#include<map>

#include<cstdlib> 	// rand()
#include<ctime>

#include "interpreter.hpp"

using namespace std;
using namespace cln;

enum Instructions { READ, WRITE, LOAD, STORE, COPY, ADD, SUB, SHR, SHL, INC, DEC, RESET, JUMP, JZERO, JODD, HALT, ERROR };

cl_I interpret(std::istream& plik, std::ostream& out, std::istream& programStdIn, std::ostream& programStdOut)
{
  vector< tuple<Instructions,int,int> > program;
  map<cl_I,cl_I> pam;

  cl_I r[10];
  int lr;

  cl_I k=0;
  cl_I i;
  Instructions i1;
  int i2, i3;
  string com;

  out << "Czytanie programu." << endl;
  if( !plik )
  {
    out << "Błąd: Nie można otworzyć pliku " << endl;
    return -1;
  }
  while( !plik.eof() )
  {
    plik >> com;
    out << "wczytano komendę: " << com << std::endl;
    i1 = ERROR;
    i2 = 0;
    i3 = 0;

    if( com=="GET"  ) { i1 = READ; plik >> i2; }
    if( com=="PUT" ) { i1 = WRITE; plik >> i2; }

    if( com=="LOAD"  ) { i1 = LOAD; plik >> i2; }
    if( com=="STORE" ) { i1 = STORE; plik >> i2; }

    if( com=="COPY"  ) { i1 = COPY; plik >> i2; }
    if( com=="ADD"   ) { i1 = ADD; plik >> i2; }
    if( com=="SUB"   ) { i1 = SUB; plik >> i2; }
    if( com=="SHR"   ) { i1 = SHR; plik >> i2; }
    if( com=="SHL"   ) { i1 = SHL; plik >> i2; }
    if( com=="INC"   ) { i1 = INC; plik >> i2; }
    if( com=="DEC"   ) { i1 = DEC; plik >> i2; }
    if( com=="ZERO" ) { i1 = RESET; plik >> i2; }

    if(i2>9) { out << "Błąd: zły adress w instrukcji w linii " << k << endl; return -1; }
    if(i3>9) { out << "Błąd: zły adress w instrukcji w linii " << k << endl; return -1; }

    if( com=="JUMP"  ) { i1 = JUMP; plik >> i2; }
    if( com=="JZERO" ) { i1 = JZERO; plik >> i2;  plik >> i3; }
    if( com=="JODD"  ) { i1 = JODD; plik >> i2;  plik >> i3; }
    if( com=="HALT"  ) { i1 = HALT; }

    if(i1==ERROR) { out << "Błąd: Nieznana instrukcja w linii " << k << "." << endl; return -1; }
    if(i2<0) { out << "Błąd: Zły adress w instrukcji w linii " << k << endl; return -1; }
    if(i3<0) { out << "Błąd: Zły adress w instrukcji w linii " << k << endl; return -1; }

    if( plik.good() )
    {
      program.push_back( make_tuple(i1,i2,i3) );
    }
    k++;
  }
  out << "Skończono czytanie programu (linii: " << program.size() << ")." << endl;

  out << "Uruchamianie programu." << endl;
  lr = 0;
  srand(time(NULL));
  for(int i = 0; i<10; i++ ) r[i] = rand();
  i = 0;
  while( get<0>(program[lr])!=HALT )	// HALT
  {
    switch( get<0>(program[lr]) )
    {
      case READ:	programStdIn >> r[get<1>(program[lr])]; i+=100; lr++; break;
      case WRITE:	programStdOut << r[get<1>(program[lr])] << endl; i+=100; lr++; break;

      case LOAD:	r[get<1>(program[lr])] = pam[r[get<2>(program[lr])]]; i+=20; lr++; break;
      case STORE:	pam[r[get<2>(program[lr])]] = r[get<1>(program[lr])]; i+=20; lr++; break;

      case COPY:	r[get<1>(program[lr])] = r[get<2>(program[lr])] ; i+=1; lr++; break;
      case ADD:   r[get<1>(program[lr])] += r[get<2>(program[lr])] ; i+=5; lr++; break;
      case SUB:   if( r[get<1>(program[lr])] >= r[get<2>(program[lr])] )
      r[get<1>(program[lr])] -= r[get<2>(program[lr])];
      else
      r[get<1>(program[lr])] = 0;
      i+=5; lr++; break;
      case SHR:   r[get<1>(program[lr])] >>= 1; i+=1; lr++; break;
      case SHL:   r[get<1>(program[lr])] <<= 1; i+=1; lr++; break;
      case INC:   r[get<1>(program[lr])]++ ; i+=1; lr++; break;
      case DEC:   if( r[get<1>(program[lr])]>0 ) r[get<1>(program[lr])]--; i+=1; lr++; break;
      case RESET: r[get<1>(program[lr])] = 0; i+=1; lr++; break;

      case JUMP: 	lr = get<1>(program[lr]); i+=1; break;
      case JZERO:	if( r[get<1>(program[lr])]==0 ) lr = get<2>(program[lr]); else lr++; i+=1; break;
      case JODD:	if( oddp(r[get<1>(program[lr])]) ) lr = get<2>(program[lr]); else lr++; i+=1; break;
      default: break;
    }
    if( lr<0 || lr>=(int)program.size() )
    {
      out << "Błąd: Wywołanie nieistniejącej instrukcji nr " << lr << "." << endl;
      return -1;
    }
  }
  out << "Skończono program (czas: " << i << ")." << endl;

  return i;
}
