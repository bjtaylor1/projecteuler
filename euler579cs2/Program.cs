using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Helpers;

namespace euler579cs2
{
    class Program
    {

        static void Main(string[] args)
        {
            int maxSide = int.Parse(args[1]);
            for (long m = 0; m <= Math.Sqrt(maxSide); m++)
            {
                long nmax = (long) Math.Ceiling(Math.Sqrt(maxSide - m*m));
                for (long n = 0; n <= nmax; n++)
                {
                    long pmax = (long) Math.Ceiling(Math.Sqrt(maxSide - m*m - n*n));
                    for (long p = 0; p <= pmax; p++)
                    {
                        long qmax = (long) Math.Ceiling(Math.Sqrt(maxSide - m*m - n*n - p*p));
                        for (long q = 0; q <= qmax; q++)
                        {
                            if (((m + n + p + q)%2) == 1)
                            {
                                mnpq it = new mnpq(m, n, p, q);
                                try
                                {
                                    process_mnpq(it);
                                }
                                catch (InvalidOperationException e)
                                {
                                    throw new InvalidOperationException($"{e.Message} while processing {it}");
                                }
                            }
                        }
                    }
                }
            }
        }

        private static void process_mnpq(mnpq item)
        {
            abcd baseAbcd = item.get_abcd();
            if (new[] {baseAbcd.a, baseAbcd.b, baseAbcd.c /*NOT d (although doesn't seem to make much difference in C++*/}.gcd() == 1)
            {
                vectortriple baseTriple = get_triple(baseAbcd, item);
                long sumgcd = baseTriple.u.gcd() + baseTriple.v.gcd() + baseTriple.n.gcd();
                long l = baseTriple.u.length;
                var abcds = baseAbcd.get_permutations();
                
            }
        }

        private static vectortriple get_triple(abcd baseAbcd, mnpq item)
        {
            var itemArray = new[] {item.m, item.n, item.p, item.q};
            Array.Sort(itemArray);
            long[][] perms = Permutations.Of(itemArray, 4, false, false);
            vector[] allVectors = perms.SelectMany(p => new mnpq(p[0], p[1], p[2], p[3]).get_abcd().get_vectors()).ToArray();
            var u = new vector(baseAbcd.a, baseAbcd.b, baseAbcd.c);
            foreach (var v in allVectors)
            {
                if (v.is_orthogonal_to(u))
                {
                    var n = v.cross_product(u);
                    return new vectortriple(u, v, n);
                }
            }
            throw new InvalidOperationException("No triple found!");
        }
    }
}
