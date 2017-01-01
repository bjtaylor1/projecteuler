using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NLog;

namespace euler579_findtriples
{
    class TripleFinder
    {
        static void Output(int[] triple, int square)
        {
            if (triple.Sum(i => i*i) == square*square)
            {
                LogManager.GetCurrentClassLogger().Info($"{string.Join(" + ", triple.Select(i => i.ToString()))} = {square}");
            }
        }
        static bool IsIntegral(double d)
        {
            return Math.Abs(d - Math.Round(d, 0)) < 1e-9;
        }

        static void PrintOut(int[] triple)
        {
            if (triple.Length == 2)
            {
                var sumSquares = triple.Sum(i => Math.Pow(i, 2));
                var squareRoot = Math.Pow(sumSquares, 0.5);
                if (IsIntegral(squareRoot))
                {
                    Output(triple, (int)squareRoot);
                }
            }
            else
            {
                for(int i = triple.Any() ? triple.Max() : 1; i < 100; i++)
                    PrintOut(triple.Concat(new [] {i}).ToArray());
            }
        }

        static void Main(string[] args)
        {
            PrintOut(new int[] {} );
        }
    }
}
