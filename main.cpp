#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include <map>
#include <fstream>

std::ifstream infile;
std::ofstream outfile;
std::ofstream outfile_decoder;
char *IFPATH = "infile";
char *OFPATH = "outfile";
std::vector<int> compressed;

template <typename Iterator> compress(char *IFPATH, Iterator result) {
/******Build the dictionary******************************/
  int dictSize = 256;
  std::map<std::string,int> dictionary;
  for (int i = 0; i < 256; i++)
    dictionary[std::string(1, i)] = i;
/********************************************************/
  std::string w;

  infile.open(FPATH,std::ios_base::binary);

 if(!infile.is_open())
  {
      std::cout << "File compression can not be found" << std::endl;
      throw("Error");
  }

 outfile.open("infile.lzw",std::ios_base::binary);
 if(!outfile.is_open())
 {
     std::cout << "It is not possible to create a outfile coder" << std::endl;
     throw("Error");
 }

  std::istream_iterator <char>it(infile);
  for (;it != std::istream_iterator <char>(); ++it)
  {
    char c = *it;
    std::string wc = w + c;
    if (dictionary.count(wc))
      w = wc;
    else {
      *result++ = dictionary[w];
      // Add wc to the dictionary.
      dictionary[wc] = dictSize++;
      w = std::string(1, c);
    }
  }

  // Output the code for w.
  if (!w.empty())
    *result++ = dictionary[w];
  copy(compressed.begin(), compressed.end(), std::ostream_iterator<int>(outfile));
  std::cout << "Compress done" << std::endl;
}


template <typename Iterator> decompress(Iterator begin, Iterator end) {
  int dictSize = 256;
  std::map<int,std::string> dictionary;
  for (int i = 0; i < 256; i++)
    dictionary[i] = std::string(1, i);

  outfile_decoder.open("infile_out",std::ios_base::binary);
  if(!outfile_decoder.is_open())
  {
      std::cout << "It is not possible to create a file decoder" << std::endl;
      throw("Error");
  }

  std::string w(1, *begin++);
  std::string result = w;
  std::string entry;
  for ( ; begin != end; begin++)
  {
    int k = *begin;
    if (dictionary.count(k))
      entry = dictionary[k];
    else if (k == dictSize)
      entry = w + w[0];
    else
      throw "Error";

    result += entry;

    dictionary[dictSize++] = w + entry[0];

    w = entry;
  }
  outfile_decoder << result;
  std::cout << "Decompress done" << std::endl;
}

int main(int argc,char **argv)
{
   if(argc > 0)
   {

   }
   compress(IFPATH, std::back_inserter(compressed));
   decompress(compressed.begin(), compressed.end());

    infile.close();
    outfile.close();
  return 0;
}

