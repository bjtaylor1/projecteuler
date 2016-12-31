using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NLog;

namespace euler579_findtriples
{
    class Program
    {
        static void Output(int[] triple, int square, List<Tuple<int[], int>> triples )
        {
            if (triple.Sum(i => i*i) == square*square)
            {
                triples.Add(Tuple.Create(triple, square));

            }
        }
        static bool IsIntegral(double d)
        {
            return Math.Abs(d - Math.Round(d, 0)) < 1e-9;
        }

        static void PrintOut(int[] triple, List<Tuple<int[], int>> triples)
        {
            if (triple.Length == 3)
            {
                if (triple.Count(i => i != 0) >= 2)
                {
                    var sumSquares = triple.Sum(i => Math.Pow(i, 2));
                    var squareRoot = Math.Pow(sumSquares, 0.5);
                    if (IsIntegral(squareRoot))
                    {

                        Output(triple, (int) squareRoot, triples);
                    }
                }
            }
            else
            {
                for(int i = triple.Any() ? triple.Max() : 1; i < 100; i++)
                    PrintOut(triple.Concat(new [] {i}).ToArray(), triples);
            }
        }


        static void Main(string[] args)
        {
            var triples = new List<Tuple<int[], int>>();
            PrintOut(new int[] {}, triples);
            foreach (var triple in triples
                .Where(t => t.Item1.Distinct().Count() == 3)
                .OrderBy(t => t.Item2))
            {
                LogManager.GetCurrentClassLogger().Info($"{string.Join(" + ", triple.Item1.Select(i => i.ToString()))} = {triple.Item2}");
            }
        }
    }
}
