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
    public class Program
    {
        static void Main(string[] args)
        {
            try
            {
                int count = 0;
                //WriteCombinationsInfoLine(5000, "1,70,182,195", ref count);

                WriteCombinationsInfoForFile(5000, "primitivetriplessorted.csv");


                //var allVariants = cube1.Variants.Concat(cube2.Variants).Distinct().ToArray();
                //CalculateResult(50);
            }
            finally { DatabaseHelper.Instance.Dispose(); }
        }


        private static void WriteCombinationsInfoForFile(int n, string fileName)
        {
            using (var file = File.OpenRead(fileName))
            {
                using (var sr = new StreamReader(file))

                {
                    string line;
                    int count = 0;
                    DatabaseHelper.Instance.ResetDone();

                    while ((line = sr.ReadLine()) != null)
                    {
                        if (WriteCombinationsInfoLine(n, line, ref count)) break;
                    }
                }
            }
        }

        private static bool WriteCombinationsInfoLine(int n, string line, ref int count)
        {
            Console.Write($"\r{(double) (count++)/853831:0.000%}");
            var ints = line.Split(',').Select(Int32.Parse).ToArray();
            if (!DatabaseHelper.Instance.IsDone(ints))
            {
                if (OutputCubeInfoForTriple(n, ints, false)) return true;
            } //else LogManager.GetCurrentClassLogger().Warn($"Done: {string.Join(",", ints.Select(i => i.ToString()))}");
            return false;
        }


        private static bool OutputCubeInfoForTriple(int n, int[] ints, bool isDuplicate)
        {
            var baseTripleSides = ints.Take(3).ToArray();
            var triple = new Triple(baseTripleSides);
            if (triple.Square <= n)
            {
                var baseCube = triple.GetCube(n);
                if (baseCube != null)
                {
                    LogManager.GetCurrentClassLogger().Info($"{baseCube} Side {baseCube.A.Length}, Combinations: {baseCube.GetCombinations()}");
                    foreach (var variant in baseCube.Variants)
                    {
                        LogManager.GetCurrentClassLogger().Trace($"   {variant}");
                    }
                    DatabaseHelper.Instance.SetDone(baseTripleSides);
                    if (!isDuplicate)
                    {
                        foreach (var duplicate in baseCube.GetDuplicateDefinitionPoints())
                        {
                            DatabaseHelper.Instance.SetDone(duplicate);
                        }
                    }

                }
                return false;
            }
            else return true;
        }


        private static ulong GetMinHsf(Vector3D v)
        {
            var hsfAll = Numerics.GetHighestSquareFactorOfAll(
                new[] {(ulong) Math.Abs(Math.Round(v.X)), (ulong) Math.Abs(Math.Round(v.Y)), (ulong) Math.Abs(Math.Round(v.Z))}
                    .Where(h => h > 0).ToArray());
            return hsfAll;
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
                    sw.WriteLine($"{String.Join(",", triple.Sides.Select(i => i.ToString()))},{triple.Square}");
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
                        var ints = line.Split(',').Select(Int32.Parse).ToArray();
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
                                        var allCubes = new List<Cube> { baseCube };
                                        var maxFactor = (int)((double)n / triple.Square);
                                        for (int factor = 2; factor <= maxFactor; factor++)
                                        {
                                            var factoredTriple = new Triple(triple.Sides.Select(i => i * factor).ToArray(), triple.Square * factor);
                                            var factoredCube = factoredTriple.GetCube(n);
                                            if (factoredCube != null) allCubes.Add(factoredCube);
                                            else break;
                                        }
                                        var contributions = allCubes.Select(c =>
                                        {
                                            var contribution = new Contribution(c.LatticePoints, c.GetRepeatability(n), c.GetCombinations());
                                            return contribution;
                                        }).ToArray();
                                        var totalContribution = contributions.Sum(c => (long)(c.Total % (ulong)1e9));
                                        Console.Out.WriteLine();
                                        LogManager.GetCurrentClassLogger().Debug($"{triple}: Bounds: {baseCube.MaxBounds}, Combs: {baseCube.GetCombinations()}: Multiples: {contributions.Length}, contributions: {totalContribution} : {String.Join(",", contributions.Select(c => c.ToString()))})");
                                        result += totalContribution;
                                        if (result > 1e9) result -= (long)1e9;

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
            var isIntegral = Numerics.IsIntegral(v.X) && Numerics.IsIntegral(v.Y) && Numerics.IsIntegral(v.Z);
            return isIntegral;
        }
    }

    public static class PointsCalculatorFast
    {
        public static ulong GetSurfacePoints(Cube c)
        {
            var s = (ulong)c.A.Length;
            var s3 = s * s * s;
            var hsf = Numerics.GetHighestSquareFactor(s);
            var surfacePoints = s3 - (hsf + 2) * s + hsf + 1;
            return surfacePoints;
        }
    }

    internal class Contribution
    {
        public ulong LatticePoints { get; set; }
        public ulong Repeatability { get; set; }
        public ulong Combinations { get; set; }

        public Contribution(ulong latticePoints, ulong repeatability, ulong combinations)
        {
            LatticePoints = latticePoints;
            Repeatability = repeatability;
            Combinations = combinations;
            Total = (LatticePoints * Repeatability * Combinations) % (ulong)1e9;
        }

        public ulong Total { get; set; }

        public override string ToString()
        {
            return $"{LatticePoints}x{Repeatability}x{Combinations}={Total}";
        }
    }
}
