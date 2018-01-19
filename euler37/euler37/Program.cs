using System;
using System.Text.RegularExpressions;
using Mpir.NET;

namespace euler37
{
    class Program
    {
        static readonly Regex[] truncates = new[]
        {
            new Regex(@"\d$", RegexOptions.Compiled),
            new Regex(@"^\d", RegexOptions.Compiled)
        };
        static bool IsTruncatable(mpz_t p)
        {
            if (p <= 7) return false;
            var ps = p.ToString();
            foreach(var truncate in truncates)
            {
                for(string pstrunc = truncate.Replace(ps, string.Empty);
                    pstrunc.Length > 0;
                    pstrunc = truncate.Replace(pstrunc, string.Empty))
                {
                    var ptrunc = new mpz_t(pstrunc);
                    if (!ptrunc.IsProbablyPrimeRabinMiller(10))
                        return false;
                }
            }
            return true;
        }

        static void Main(string[] args)
        {
            mpz_t sum = 0, p = 7 ;
            int found = 0;
            while(found < 11)
            {
                do
                {
                    p = p.NextPrimeGMP();
                } while (!IsTruncatable(p));
                Console.WriteLine(p);
                sum += p;
                found++;
            }
            Console.WriteLine($"Sum = {sum}");
        }
    }
}
