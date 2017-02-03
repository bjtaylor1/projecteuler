// euler586.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "mpz_w.h"
#include "euler586.h"

using namespace std;

#define N 800000
//#define R 10
long long K[N + 1];

vector<pair<long long, long long>> factorize(long long n)
{
	vector<pair<long long, long long>> result;
	for (long f = 2; f < n; f++)
	{
		if (n%f == 0)
		{
			long long t = 0, ft = f;
			while (n % f == 0)
			{
				t++;
				n /= f;
			}
			result.push_back(pair<long long, long long>(f, t));
		}
	}
	if (n > 1) result.push_back(pair<long, long>(n, 1));
	return result;
}

void factorize_actual_answers()
{
	set<vector<long long>> contfactorsset;

	//for (long long k : {10253399761, 14754892339, 15255058181, 17755887391, 19514535029, 19756550759, 20176044691, 20860365031, 21567496049, 22257379969, 23483593001, 25103151139, 25258375021, 26129631649, 27259038389, 27931675211, 29033820409, 29437179959, 31036152851, 31467330301, 31839504521, 32760862651, 32918809759, 33406237931, 33793463099, 34761526019, 34939004221, 35710116409, 36052276579, 36124046761, 37262355229, 37348590719, 37601177251, 37762521071, 38315335949, 38538640481, 38875793429, 40194361889, 41013599044, 41047815061, 41556882631, 42360819941, 42632556901, 43328882861, 43796779939, 44764842859, 45248874319, 45265008701, 45282255799, 45974921509, 46317081679, 46817247521, 47370970141, 47765837911, 47776965071, 48029083091, 48072391169, 48369486341, 49145605751, 49282469819, 49396523209, 49701963751, 49766501279, 49943979481, 50976579929, 51266998805, 51387728491, 51880105321, 52681260841, 52767496331, 52838519129, 53129685389, 53160285079, 53388391859, 53638752959, 54492206131, 54504914519, 54962328641, 55136702951, 55458043619, 56720419921, 57005743729, 57268988909, 57337977301, 57849826661, 58822135471, 59019569356, 59205114749, 59769818119, 59866624411, 59889435089, 60269983961, 60632173019, 61020232724, 61349289199, 61839469879, 61919585431, 62351319239, 62431991149, 62651928251, 62770813171, 63174172721, 63288226111, 63428926121, 63972546451, 63995357129, 64368116989, 64464923281, 65820211369, 66159033391, 66651410221, 66737645711, 66972750889, 67272305749, 67531012219, 67772471591, 67964415101, 68401712489, 68807297471, 68910780059, 69115022009, 69466581701, 69789269341, 70268293579, 70273300801, 70359536291, 70694493089, 70721725349, 70918676081, 71023549564, 71082801691, 71457904111, 71870604619, 72070337141, 72515979899, 72922736161, 73119071971, 73322698999, 73347735109, 73774461695, 73827110731, 74257380439, 74306896301, 74602322399, 75011714041, 75032943491, 75394693319, 75742856299, 75809619259, 75834099011, 76035917771, 76275290905, 76827198041, 77517081961, 77775788431, 77951890379, 78058140116, 78259819891, 78379436861, 78433901381, 78613406509, 79026203036, 79050404609, 79092131459, 79431509839, 79615107979, 79711914271, 80190938509, 80207629249, 80408240231, 80594327341, 80704178764, 80966501561, 81092677699, 81139382489, 81487545469, 81787685969, 82777446851, 83019462581, 83172314621, 83441460124, 83487359659, 83657048849, 83889870031, 84502156651, 84646487629, 85105805081, 85197604151, 85209287669, 85342813589, 86149532689, 86269984196, 86488354711, 86735201971, 87278939429, 87515860091, 88085658529, 88151865131, 88265918521, 88585824371, 88744942759, 88779436955, 88811705719, 88973049539, 89029519876, 89069855831, 89103793669, 89245108601, 89634559201, 89675378309, 89779768639, 90279934481, 90909175379, 91003199881, 91073300989, 91140063949, 91228524871, 91410190399, 91500850199, 92058054649, 92090879771, 92280597849, 92453068829, 92543843029, 92942107511, 93535185139, 93934372004, 93990842341, 94055379869, 94160531531, 94386559289, 94508840921, 94695191569, 94716889531, 94781427059, 94936650941, 94958905261, 95109121921, 95178666671, 95212604509, 95816252939, 96233521439, 96597818801, 96705869381, 97178773681, 97282256269, 97572675145, 97642307741, 97677124039, 97714341461, 97927631549, 98211374129, 98782753795, 99351908029, 99799219861, 35303027881, 61752747001, 70564078321})
	for (long long k : {209, 2299, 6061, 278179})
	{
		auto factors = factorize(k);
		vector<long long> contributingfactors;
		for (auto f : factors)
		{
			if (K[f.first] == 1)
			{
				contributingfactors.push_back(f.second);
			}
			else if (K[f.first] > 1)
			{
				cout << "error - " << K[f.first] << " contributes " << K[f.first] << endl;
				exit(1);
			}
		}
		sort(contributingfactors.begin(), contributingfactors.end());
		contfactorsset.insert(contributingfactors);
	}
	for (auto cf : contfactorsset)
	{
		cout << "  ";
		for (auto l : cf)
		{
			cout << l << "  ";
		}
		cout << endl;
	}

}

int main()
{
	memset(K, 0, N * sizeof(long long));
	mpz_w w;
	cout << w.val;

	for (long long b = 1; b <= N; b++)
	{
		for (long long a = 2 * b + 1; a <= N; a++)
		{
			long long res = a*a + a*b - b*b;
			if (res > N)
				break;
			else if (res >= 0)
				K[res]++;
			else
			{
				cout << a << "," << b << endl;
			}
		}
	}

	factorize_actual_answers();
	return 0;

	//long long R = 6;
	for (long long R = 5; R <= 20; R++)
	{

		//long long tot = 0;

		set<vector<long long>> contfactorsset;
		long long max1 = 0;
		for (long long ki = 1; ki <= N; ki++)
		{
			if (K[ki] == R)
			{
				auto factors = factorize(ki);
				vector<long long> contributingfactors;
				for (auto f : factors)
				{
					if (K[f.first] == 1)
					{
						contributingfactors.push_back(f.second);
					}
					else if (K[f.first] > 1)
					{
						cout << "error - " << K[f.first] << " contributes " << K[f.first] << endl;
						exit(1);
					}
				}
				sort(contributingfactors.begin(), contributingfactors.end());
				contfactorsset.insert(contributingfactors);
			}
			if (K[ki] == 1 && ki > max1) max1 = ki;
		}

		cout << R << ":" << endl;
		for (auto cf : contfactorsset)
		{
			cout << "  ";
			for (auto l : cf)
			{
				cout << l << "  ";
			}
			cout << endl;
		}
	}
	return 0;

}
