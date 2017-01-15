using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Helpers;

namespace euler579cs2
{
    class Program
    {
        public static int N;
        private static HashSet<cube> cubesDone = new HashSet<cube>();
        private static massiveinteger S = new massiveinteger();
        static void Main(string[] args)
        {
            N = int.Parse(args[0]);
            for (long m = 0; m <= Math.Sqrt(N); m++)
            {
                long nmax = (long) Math.Ceiling(Math.Sqrt(N - m*m));
                for (long n = 0; n <= nmax; n++)
                {
                    long pmax = (long) Math.Ceiling(Math.Sqrt(N - m*m - n*n));
                    for (long p = 0; p <= pmax; p++)
                    {
                        long qmax = (long) Math.Ceiling(Math.Sqrt(N - m*m - n*n - p*p));
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
            Console.Out.WriteLine($"S = {S}");
        }

        private static void process_mnpq(mnpq item)
        {
            abcd baseAbcd = item.get_abcd();
            if (new[] {baseAbcd.a, baseAbcd.b, baseAbcd.c /*NOT d (although doesn't seem to make much difference in C++*/}.gcd() == 1)
            {
                HashSet<cube> thisCubes = new HashSet<cube>();

                vectortriple baseTriple = get_triple(baseAbcd, item);
                long sumgcd = baseTriple.u.gcd() + baseTriple.v.gcd() + baseTriple.n.gcd();
                long l = baseTriple.u.length;
                var abcds = baseAbcd.get_permutations();
                foreach (var abcd in abcds)
                {
                    var triple = get_triple(abcd, item);
                    for (int f = 0; f < 8; f++)
                    {
                        bool flipX = (f & 1) != 0,
                            flipY = (f & 2) != 0,
                            flipZ = (f & 4) != 0;
                        int []order = { 0, 1, 2 };
                        foreach (var orderPermutation in Permutations.Of(order,3,false, false))
                        {
                            var cube = new cube(triple.u, triple.v, triple.n, flipX, flipY, flipZ, orderPermutation);
                            thisCubes.Add(cube);
                        }
                    }
                }

                massiveinteger thisS = new massiveinteger();
                foreach (var thisCube in thisCubes)
                {
                    if (!thisCube.is_oversize() && cubesDone.Add(thisCube))
                    {
                        long maxSize = new[] {thisCube.width, thisCube.height, thisCube.depth}.Max();
                        long tmax = N / maxSize;
                        if (tmax * maxSize > N) throw new InvalidOperationException("tmax is too lenient - would produce oversize cubes!");
                        if ((tmax + 1) * maxSize <= N) throw new InvalidOperationException("tmax is not lenient enough - could squeeze another one out!");
                        if (tmax <= 0) throw new InvalidOperationException("tmax <= 0");

                        for (long t = 1; t <= tmax; t++)
                        {
                            long repeatability = (N + 1L - thisCube.width*t)*
                                                 (N + 1L - thisCube.height*t)*
                                                 (N + 1L - thisCube.depth*t);
                            if (repeatability <= 0) throw new InvalidOperationException("Repeatability <= 0");
                            massiveinteger ehp = new massiveinteger(l*l*l)*new massiveinteger(t*t*t) +
                                                 new massiveinteger(l*sumgcd)*new massiveinteger(t*t) + new massiveinteger(sumgcd*t + 1);
                            massiveinteger contributionS = ehp*new massiveinteger(repeatability);
                            var line = $"{item},{t},{thisS},{contributionS},";
                            thisS += contributionS;
                            line += thisS;
                            //File.AppendAllLines("s5_verbose_csh.csv", new [] {line});
                        }
                    }
                }
                S += thisS;
                S.truncate(9);
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
