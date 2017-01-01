using System;
using System.Collections.Generic;
using System.Linq;
using euler579_findtriples;

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
                if (IsIntegral(squareRoot))
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
                for(int i = triple.Any() ? triple.Max() : 0; i < maxSide; i++)
                    MakeTriples(triple.Concat(new [] {i}).ToArray(), triples,maxSide);
            }
        }

        public static Triple[] FindTriples(int maxSide)
        {
            var triples = new List<Triple>();
            MakeTriples(new int[] {} , triples, maxSide);
            return triples.Where(t => t.Dimensions > 0).OrderBy(t => t.Square).ThenByDescending(t => t.Dimensions).ToArray();
        }
    }
}
