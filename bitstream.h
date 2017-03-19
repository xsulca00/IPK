#ifndef _BITSTREAM_H
#define _BITSTREAM_H

#include <vector>
#include <string>

using namespace std;

vector<char> get_bytes_from(const string& name);
void write_bytes_to(const string& name, const vector<char>& bytes);

#endif
