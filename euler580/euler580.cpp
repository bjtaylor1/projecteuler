// euler580.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;

string get_representation(long long hs, long long hsm)
{
	stringstream ss;
	ss << hs << " x " << (hsm / hs);
	return ss.str();
}

int main(int argc, char** argv)
{
	if (argc < 2) return 1;
	long long limit = stoll(argv[1]);
	string** is_hs = new string*[limit];
	memset(is_hs, 0, limit * sizeof(string*));
	long long h;
	for (long long k = 1; (h = (4LL * k + 1LL)) < limit; k++)
	{
		long long hs = h*h;
		for (long long hsm = hs; hsm < limit; hsm += hs)
		{
			string representation = get_representation(hs, hsm);
			if (is_hs[hsm])
			{
				is_hs[hsm]->append(" = " + representation);
				cout << *is_hs[hsm] << endl;
			}
			else
			{
				is_hs[hsm] = new string(representation);
			}
		}
	}
	long long tot = 0;
	for (long long k = 0; (h = (4 * k + 1)) < limit; k++)
	{
		if (is_hs[h])
		{
			tot++;
			delete (is_hs[h]);
		}
	}
	delete[] is_hs;
	cout << tot << endl;
    return 0;
}

