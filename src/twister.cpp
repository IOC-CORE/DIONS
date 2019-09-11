#include "twister.h"

const unsigned CYCLE = 0x100;

const unsigned C_TRANS_ = 0xff;
int block_units[][0x10] = {
	0x74, 0x63, 0x69, 0x67, 0x72, 0x68, 0x6e, 0x6c, 0x61, 0x67, 0x74, 0x6c, 0x69, 0x73, 0x61, 0x61, 0x6f, 0x6d, 0x6f, 0x72, 0x6e, 0x72, 0x69, 0x6d, 0x6e, 0x6e, 0x65, 0x74, 0x72, 0x6e, 0x6b, 0x73, 0x61, 0x6f, 0x65, 0x69, 0x73, 0x64, 0x6c, 0x65, 0x69, 0x6b, 0x68, 0x68, 0x68, 0x65, 0x72, 0x64, 0x66, 0x74, 0x61, 0x73, 0x6f, 0x69, 0x65, 0x74, 0x69, 0x68, 0x6e, 0x65, 0x62, 0x74, 0x6b, 0x65
  };
int triad_units[][0x03] = {
	0x74, 0x68, 0x65, 0x74, 0x68, 0x61, 0x65, 0x74, 0x68, 0x66, 0x6f, 0x72, 0x65, 0x73, 0x74, 0x69, 0x6e, 0x67, 0x65, 0x72, 0x65, 0x68, 0x61, 0x74, 0x68, 0x69, 0x73
};
const int transition_seq[] = { 0xce, 0xab, 0xdf, 0xcf, 0xee,
                               0xcf, 0xad, 0xdf, 0xff, 0xce, 
                               0x32, 0x40, 0xd3, 0x27, 0x82, 
                               0xda, 0xee, 0xff, 0xfc, 0xbf, 
                               0x1c, 0x90, 0x13, 0x4a, 0xa5, 
                               0xe0, 0x21, 0x9f, 0xe1, 0xc6, 
                               0xe4, 0x38, 0x1f, 0x60, 0x24, 
                               0x12, 0x9a, 0x30, 0x44, 0x72, 
                               0xf2, 0xca, 0xa2, 0x51, 0xc6, 
                               0xcb, 0xbd, 0xba, 0xac, 0xdf, 
                               0xdf, 0xde, 0xcd, 0xfd, 0xca };

unsigned char base(unsigned char a, unsigned char (*s)(int), int pos)
{
	return (*s)(pos)^a;
}

void trans(vector<unsigned char>& data, unsigned char (*f)(unsigned char))
{
  for(unsigned i = 0; i<data.size(); i++)
  {
    unsigned char d = data[i];
    for(unsigned j=0; j<CYCLE; j++)
    {
      d = (*f)(d);
    }
    data[i] = d;
  }
}

vector<double> f_dist(vector<unsigned char>& in)
{
  vector<double> fdist;
  fdist.resize(256);
  for(unsigned i = 0; i<in.size(); i++)
  {
    fdist[in[i]]++;
  }

  for(unsigned i=0; i<fdist.size(); i++)
  {
    fdist[i] = (fdist[i] / in.size()) * 100;
  }

  return fdist;
}

double s_entropy(vector<double> v) 
{
  double entropy = 0;
  double p;

  for(unsigned i = 0; i < v.size(); i++) 
  {
    p = v[i] / 100;
    if (p == 0) continue;
    entropy += p * std::log(p) / std::log(2);
  }
  return -entropy;
}
void rms(const string& s, string& r)
{
  for(unsigned int i=0; i<s.size(); i++)
  {
    if(::isspace(s[i])) continue;
    r+= s[i];
  }
}

double sw(double weight, int i, int j, int (*inv)(int, int))
{
  return weight*(*inv)(i, j); 
}
void hPerm(int s, int n, void (*p)(int), void (*inv)(int, int))
{
  if(s == 1)
  {
    (*p)(n);
    return;
  }
  for(int i=0; i< s; i++)
  {
    hPerm(s-1, n, p, inv);
    if(s%2 == 1)
      (*inv)(0, s-1);
    else 
      (*inv)(i, s-1);
  }
}

double ic(const string& t)
{
  string text; rms(t, text);
  vector<double> freq(256,0);
  for(unsigned int i=0; i<text.size(); i++)
  {
    if(text[i] == ' ') continue;
    freq[text[i]] ++;
  }
  double sum=0;
  for(unsigned int i=0; i<freq.size(); i++)
  {
    if(freq[i] != 0)
    {
      double c = freq[i];
      if(c != 0)
        sum += c * (c - 1);
    }
  }
  double ic = 26 * sum / (text.size() * (text.size() - 1));
  return ic;
}
