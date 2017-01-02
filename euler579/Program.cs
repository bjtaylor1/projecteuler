using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media.Animation;
using System.Windows.Media.Media3D;
using NLog;

namespace euler579
{
    class Program
    {
        static void Main(string[] args)
        {
/*
            var cubes = new Triple(new [] {4,4,7}).GetCubes(20);
            Console.Out.WriteLine(cubes.First().LatticePoints);
*/
/*
            new Triple(new[] { 1, 0, 0 }, 5).GetCubes(100);
            new Triple(new[] { 3, 4, 0 }, 5).GetCubes(100);
            new Triple(new[] { 4, 4, 7 }, 9).GetCubes(100);
            new Triple(new[] { 2, 3, 6 }, 7).GetCubes(100);
*/

            //            for(int n = 1; n <= 10; n++)
            const int n = 5000;
            CalculateResult(n);
            //FindTriples();
        }

        private static void FindTriples()
        {
            using (var sw = new StreamWriter("triples.csv"))
            {
                var n = 5000;
                TripleFinder.FindTriples(n, triple =>
                {
                    if (triple.Square > n) throw new InvalidOperationException("too big.");
                    // ReSharper disable once AccessToDisposedClosure
                    sw.WriteLine($"{string.Join(",", triple.Sides.Select(i => i.ToString()))},{triple.Square}");
                });
            }
        }

        private static void CalculateResult(int n) //correct
        {
            long result = 0;
            var contributionByCombination = new Dictionary<int, long>();
            using (var file = File.OpenRead("triples.csv"))
            {
                using (var sr = new StreamReader(file))
                {
                    string line;
                    int count = 0;
                    while ((line = sr.ReadLine()) != null)
                    {
                        Console.Write($"\r{(double)(count++)/1414662:0.000%}");
                        var ints = line.Split(',').Select(int.Parse).ToArray();
                        var baseTripleSides = ints.Take(3).ToArray();
                        var tripleSquare = ints.Last();
                        var triple = new Triple(baseTripleSides, tripleSquare);
                        if (triple.Square <= n)
                        {
                            var cubes = triple.GetCubes(n).ToArray();
                            var s = cubes.Sum(c =>
                            {
                                var combinations = c.GetCombinations();
                                long i = (long)c.LatticePoints * c.GetRepeatability(n) * combinations;
                                if (combinations >= 8)
                                {
                                    long contribution ;
                                    if(!contributionByCombination.TryGetValue(combinations, out contribution)) contribution = 0;
                                    contributionByCombination[combinations] = contribution + i;
                                }
                                return i;
                            });
                            result += s;
                            if (result > 1e9) result -= (long)1e9;
                        }
                    }
                }
            }
            foreach (var kvp in contributionByCombination)
            {
                LogManager.GetCurrentClassLogger().Info($"Combinations of {kvp.Key} contributed {kvp.Value:0,000}");
            }
            LogManager.GetCurrentClassLogger().Info($"total by high combinations = {contributionByCombination.Values.Sum():0,000}");
            
            LogManager.GetCurrentClassLogger().Info($"S({n}) = {result:0,000}");
        }


        public static bool IsIntegral(Vector3D v)
        {
            var isIntegral = IsIntegral(v.X) &&
                             IsIntegral(v.Y) &&
                             IsIntegral(v.Z);
            return isIntegral;
        }

        public static bool IsIntegral(double d)
        {
            return Math.Abs(d - Math.Round(d, 0)) < 1e-9;
        }

        static bool IsInBounds(int n, Vector3D vector)
        {
            return IsInBounds(n, vector.X) &&
                IsInBounds(n, vector.Y) &&
                IsInBounds(n, vector.Z);
        }

        static bool IsInBounds(int n, double coord)
        {
            return coord <= n && coord >= 0;
        }
    }
}
