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
            try
            {
//                FindTripleDuplicates();
                //var cube0 = new Triple(new[] {1,2,2}).GetCube(50);
                //            var c1 = new Triple(new[] {1,4,8}).GetCube(50);
                //            var c2 = new Triple(new[] {4,4, 7}).GetCube(50);
                //            var cubes = c1.Variants.Concat(c1.Variants).Distinct().ToArray();


                //var allVariants = cube1.Variants.Concat(cube2.Variants).Distinct().ToArray();
                CalculateResult(50);
            }
            finally { DatabaseHelper.Instance.Dispose();}
        }

        private static void FindTripleDuplicates()
        {
            using(var output = File.OpenWrite("primitivetripleduplicates.csv"))
            using (var file = File.OpenRead("primitivetriplessorted.csv"))
            {
                using (var sr = new StreamReader(file))
                    using(var sw = new StreamWriter(output))

                {
                    string line;
                    int count = 0;
                    while ((line = sr.ReadLine()) != null)
                    {
                        Console.Write($"\r{count}, {(double)(count++) / 853831:0.000%}");

                        var ints = line.Split(',').Select(int.Parse).ToArray();
                        var baseTripleSides = ints.Take(3).ToArray();
                        var tripleSquare = ints.Last();
                        var triple = new Triple(baseTripleSides, tripleSquare);
                        var cube = triple.GetCube(5000);
                        var duplicateTriples = cube.GetDuplicateDefinitionPoints()
                            .Select(ps => new Triple(ps))
                            .Where(t => t.IsPrimitive)
                            .ToArray();
                        //if (duplicateTriples.Length > 1) throw new InvalidOperationException("Did not expect to find more than one duplicate");
                        foreach (var duplicateTriple in duplicateTriples)
                        {
                            if (duplicateTriple.IsPrimitive)
                            {
                                var outputLine = string.Join(",",triple.Sides.Select(s => s.ToString())) + "," + string.Join(",", duplicateTriple.Sides.Select(s => s.ToString()));
                                sw.WriteLine(outputLine);
                            }
                        }

                    }
                }
            }
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

            using (var file = File.OpenRead("primitivetriplessorted.csv"))
            {
                using (var sr = new StreamReader(file))

                {
                    string line;
                    int count = 0;
                    DatabaseHelper.Instance.ResetDone();
                    while ((line = sr.ReadLine()) != null)
                    {
                        Console.Write($"\r{(double)(count++) / 853831:0.000%}");
                        var ints = line.Split(',').Select(int.Parse).ToArray();
                        if (!DatabaseHelper.Instance.IsDone(ints))
                        {

                            var baseTripleSides = ints.Take(3).ToArray();
                            var tripleSquare = ints.Last();
                            var triple = new Triple(baseTripleSides, tripleSquare);

                            if (triple.Square <= n)
                            {
                                //if (!triple.IsPrimitive) throw new InvalidOperationException("Should be primitive!");
                                //(should all be primitive - reading from the primitives-only file)
                                {
                                    var baseCube = triple.GetCube(n);
                                    if (baseCube != null)
                                    {
                                        var allCubes = new List<Cube> {baseCube};
                                        var maxFactor = (int) ((double) n/triple.Square);
                                        for (int factor = 2; factor <= maxFactor; factor++)
                                        {
                                            var factoredTriple = new Triple(triple.Sides.Select(i => i*factor).ToArray(), triple.Square*factor);
                                            var factoredCube = factoredTriple.GetCube(n);
                                            if (factoredCube != null) allCubes.Add(factoredCube);
                                            else break;
                                        }
                                        var contributions = allCubes.Select(c =>
                                        {
                                            var contribution = new Contribution(c.LatticePoints, c.GetRepeatability(n), c.GetCombinations());
                                            return contribution;
                                        }).ToArray();
                                        var totalContribution = contributions.Sum(c => c.Total);
                                        Console.Out.WriteLine();
                                        LogManager.GetCurrentClassLogger().Debug($"{triple}: Bounds: {baseCube.MaxBounds}, Combs: {baseCube.GetCombinations()}: Multiples: {contributions.Length}, contributions: {totalContribution} (first: {contributions.First()}, {string.Join(",", contributions.Select(c => c.ToString()))})");
                                        result += totalContribution;
                                        if (n == 5000 && result > 1e9) result -= (long) 1e9;

                                        DatabaseHelper.Instance.SetDone(baseTripleSides);
                                        foreach (var duplicate in baseCube.GetDuplicateDefinitionPoints())
                                        {
                                            DatabaseHelper.Instance.SetDone(duplicate);
                                        }
                                    }
                                }
                            }
                            else break;
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
        
    }

    internal class Contribution
    {
        public long LatticePoints { get; set; }
        public long Repeatability { get; set; }
        public long Combinations { get; set; }

        public Contribution(long latticePoints, long repeatability, long combinations)
        {
            LatticePoints = latticePoints;
            Repeatability = repeatability;
            Combinations = combinations;
            Total = LatticePoints*Repeatability*Combinations;
        }

        public long Total { get; set; }

        public override string ToString()
        {
            return $"{LatticePoints}x{Repeatability}x{Combinations}={Total}";
        }
    }
}
