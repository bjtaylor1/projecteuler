using System;
using System.Collections.Generic;
using System.Linq;
using NLog;

namespace euler579
{
    class TripleFinder
    {
        static bool IsIntegral(double d)
        {
            return Math.Abs(d - Math.Round(d, 0)) < 1e-9;
        }

        static void MakeTriples(int[] triple, List<Triple> triples, int maxSide)
        {
            if (triple.Length == 3)
            {
                var sumSquares = triple.Sum(i => Math.Pow(i, 2));
                var squareRoot = Math.Pow(sumSquares, 0.5);
                if (squareRoot <= maxSide + 1e-9 && IsIntegral(squareRoot))
                {
                    var square = (int) squareRoot;
                    if (triple.Sum(i => i*i) == square*square)
                    {
                        triples.Add(new Triple(triple, square));
                    }
                }
            }
            else
            {
                for (int i = triple.Any() ? triple.Max() : 0; i <= maxSide; i++)
                {
                    if(triple.Length == 0) Console.Out.Write($"\r{(double)i/maxSide:0.00%}");
                    MakeTriples(triple.Concat(new [] {i}).ToArray(), triples,maxSide);
                }
            }
        }

        public static Triple[] FindTriplesSlow(int maxSide)
        {
            var triples = new List<Triple>();
            MakeTriples(new int[] {} , triples, maxSide);
            return triples.Distinct().Where(t => t.Dimensions > 0).OrderBy(t => t.Square).ThenByDescending(t => t.Dimensions).ToArray();
        }

        public static Triple[] FindTriples(int maxSide)
        {
            var triples = Enumerable.Range(1, 3).SelectMany(n => FindTriplesND(maxSide, n))
                .OrderBy(t => t.Square).ThenByDescending(t => t.Dimensions).ToArray();
            return triples;
        }

        private static Triple[] FindTriplesND(int maxSide, int dimensions)
        {
            var a = Enumerable.Range(1, maxSide).Select(n => n*n).ToArray();
            var triples = new List<Triple>();
            Permutations.Get(a, dimensions + 1, true, true, i =>
            {
                var sum = i.Take(dimensions).Sum();
                if (sum == i.Last())
                {
                    var roots = i.Select(sq => (int)Math.Sqrt(sq)).ToArray();
                    var triple = new Triple(roots.Take(dimensions).Concat(new[] { 0, 0, 0 }).Take(3).ToArray(), roots[dimensions]);
                    LogManager.GetCurrentClassLogger().Debug(triple);
                    triples.Add(triple);
                }
                return sum >= i.Last(); //last is increasing
            });
            var triplesnd = triples.Where(t => t.Dimensions > 0).ToArray();
            return triplesnd;
        }
        
        
    }
}
