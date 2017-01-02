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
            PrimTriples();

//            Console.Out.WriteLine(new Triple(new [] {1,2,2}).IsPrimitive);
//            CalculateResult(10);
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

        private static void PrimTriples()
        {
            using (var input = File.OpenRead("triples.csv"))
            using (var output = File.OpenWrite("primitivetriples.csv"))
            using (var sr = new StreamReader(input))
            using(var sw = new StreamWriter(output))
            {
                string line;
                while ((line = sr.ReadLine()) != null)
                {
                var ints = line.Split(',').Select(int.Parse).ToArray();
                var baseTripleSides = ints.Take(3).ToArray();
                var tripleSquare = ints.Last();
                var triple = new Triple(baseTripleSides, tripleSquare);
                    if (triple.IsPrimitive)
                        sw.WriteLine(line);
                }
            }
        }

        private static void CalculateResult(int n) //correct
        {
            long result = 0;
            using (var file = File.OpenRead("triples.csv"))
            {
                using (var sr = new StreamReader(file))

                {
                    string line;
                    int count = 0;
                    while ((line = sr.ReadLine()) != null)
                    {
                        Console.Write($"\r{(double)(count++) / 1414662:0.000%}");
                        var ints = line.Split(',').Select(int.Parse).ToArray();
                        var baseTripleSides = ints.Take(3).ToArray();
                        var tripleSquare = ints.Last();
                        var triple = new Triple(baseTripleSides, tripleSquare);
                        if (triple.Square <= n)
                        {
                            if (triple.IsPrimitive)
                            {
                                var baseCube = triple.GetCube(n);
                                if (baseCube != null)
                                {
                                    var allCubes = new List<Cube> { baseCube };
                                    var maxFactor = (int)((double)n / triple.Square);
                                    for (int factor = 2; factor <= maxFactor; factor++)
                                    {
                                        var factoredTriple = new Triple(triple.Sides.Select(i => i * factor).ToArray(), triple.Square * factor);
                                        var factoredCube = factoredTriple.GetCube(n);
                                        if (factoredCube != null) allCubes.Add(factoredCube);
                                        else break;
                                    }
                                    var totalContribution = allCubes.Sum(c => (long)c.LatticePoints * c.GetRepeatability(n) * c.GetCombinations());
                                    LogManager.GetCurrentClassLogger().Debug($"{triple}: {totalContribution}");
                                    result += totalContribution;
                                    if (n == 5000 && result > 1e9) result -= (long)1e9;
                                }
                            }
                        }
                    }
                }
            }

            LogManager.GetCurrentClassLogger().Info($"S({n}) = {result}");
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
