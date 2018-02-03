using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Mpir.NET;

namespace _266
{
    class Program
    {
        private static readonly HashSet<mpz_t> primes = new HashSet<mpz_t>();
        private static readonly mpz_t[] primesArray;
        private static readonly mpz_t m = new mpz_t(10).Power(16);
        private static object bestLock = new object();

        static Program()
        {
            for (mpz_t p = 2; p < 190; p = p.NextPrimeGMP())
                primes.Add(p);
            primesArray = primes.ToArray();
        }

        static mpz_t n, nr, best;
        static void Main(string[] args)
        {
            n = primes.Aggregate(new mpz_t(1), (st, e) => st.Multiply(e));
            nr = n.Sqrt();
            best = new mpz_t(2); // new mpz_t("225031728346737915090028065720049");
            MakePsr(1, primesArray.Length - 1);
            Console.WriteLine(best);
            Console.WriteLine(best.Mod(m));
        }

        static void MakePsr(mpz_t current, int pi)
        {
            if (current > best)
            {
                Console.WriteLine($"Best = {best}  {best.Mod(m)}");
                var div = n.Divide(best, out mpz_t rem);
                Console.WriteLine($"       {div}");
                if (rem != 0) throw new InvalidOperationException("Incorrect factor");
                if(div <= nr)
                {
                    Console.WriteLine("Improvement");
                    best = div;
                }
                Console.WriteLine();

            }
            var wi = current * primesArray[pi];
            if (pi > 0)
            {
                if (wi < nr) MakePsr(wi, pi - 1);
                MakePsr(current, pi - 1);
            }
        }
    }
}
