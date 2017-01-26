#pragma once

#include<iostream>
#include<fstream>

#include<cln/cln.h>

cln::cl_I interpret(std::istream& plik, std::ostream& out, std::istream& programStdIn, std::ostream& programStdOut);
