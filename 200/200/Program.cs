using System;
using System.Collections.Generic;
using System.Linq;
using Mpir.NET;

namespace _200
{
    class Program
    {
        static bool IsPrimeProof(mpz_t n)
        {
            var ns = n.ToString();
            var digits = ns.ToCharArray();
            char[] digitsCopy = new char[digits.Length];
            for (int di = 0; di < digits.Length; di++)
            {
                int digit = digits[di];
                var low = di == 0 ? '1' : '0';
                for (char odigit = low; odigit <= '9'; odigit++)
                {
                    if (odigit != digit)
                    {
                        Array.Copy(digits, digitsCopy, digits.Length);
                        digitsCopy[di] = odigit;
                        var cand = new mpz_t(new string(digitsCopy));
                        if (cand.IsProbablyPrimeRabinMiller(10)) return false;
                    }
                }
            }
            return true;
        }
        static void Main(string[] args)
        {
            var primes = new List<mpz_t>();
            int c = 0;
            for (mpz_t p = 2; ++c < 1e6; p = p.NextPrimeGMP())
            {
                primes.Add(p);
            }
            mpz_t limit = 1e12;
            Console.WriteLine("Made primes");
            var squbes = new SortedSet<mpz_t>();
            for (int i = 0; i < primes.Count; i++)
            {
                if (i % 1000 == 0) Console.WriteLine(i);
                for (int j = 0; j < primes.Count; j++)
                {
                    if (i != j)
                    {
                        var sqube = primes[i].Power(2).Multiply(primes[j].Power(3));
                        if (sqube > limit) break;
                        //if (sqube.ToString().Contains("200") && IsPrimeProof(sqube))
                        //{
                            squbes.Add(sqube);
                        //}
                    }
                }
            }

            var pp200squbes = squbes.Where(sq => IsPrimeProof(sq) && sq.ToString().Contains("200"))
                .OrderBy(s => s).ToArray();

            var result = pp200squbes[199];
            Console.WriteLine(result);

        }
    }
}
