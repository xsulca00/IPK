#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>

#include "bitstream.h"

using namespace std;

static vector<char> get_bytes(ifstream& ifs)
{
   vector<char> v;

   for (char c; ifs.read(&c, sizeof(c));)
       v.push_back(c);

   return v;
}

vector<char> get_bytes_from(const string& name)
{
   ifstream ifs {name, ios_base::binary};

   if (!ifs)
   {
       cerr << __func__ << ": Error opening '" << name << "' file to read!\n";
   }

   return get_bytes(ifs);
}

void write_bytes_to(const string& name, const vector<char>& bytes)
{
   ofstream ofs {name, ios_base::binary};

   if (!ofs)
   {
       cerr << __func__ << ": Error opening '" << name << "' file to write!\n";
   }

   for (auto c : bytes)
       ofs.write(&c, sizeof(c));
}

/*
int main()
{
   vector<char> bytes {get_bytes_from("s/homes/xsulca00/Stažené/elfutils-0.168.zip")};
   size_t size {bytes.size()};

   cout << "File size: " << size << '\n';

   write_bytes_to("out", bytes);

   exit(EXIT_SUCCESS);
}
*/
