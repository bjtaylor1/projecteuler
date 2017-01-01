using System;
using System.Linq;

namespace euler579
{
    class TripleFinder
    {
        static bool IsIntegral(double d)
        {
            return Math.Abs(d - Math.Round(d, 0)) < 1e-9;
        }

        static void MakeTriples(int[] sides, int maxSide, Action<Triple> process)
        {
            if (sides.Length == 3)
            {
                var sumSquares = sides.Sum(i => Math.Pow(i, 2));
                var squareRoot = Math.Pow(sumSquares, 0.5);
                if (squareRoot <= maxSide + 1e-9 && IsIntegral(squareRoot))
                {
                    var square = (int) squareRoot;
                    if (sides.Sum(i => i*i) == square*square)
                    {
                        var triple = new Triple(sides, square);
                        process(triple);
                    }
                }
            }
            else
            {
                for (int i = sides.Any() ? sides.Max() : 0; i <= maxSide; i++)
                {
                    if(sides.Length == 0) Console.Out.Write($"\r{(double)i/maxSide:0.00%}");
                    MakeTriples(sides.Concat(new [] {i}).ToArray(),maxSide, process);
                }
            }
        }

        public static void FindTriples(int maxSide, Action<Triple> process)
        {
            MakeTriples(new int[] {}, maxSide, process);
        }
    }
}
