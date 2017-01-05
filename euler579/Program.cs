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
                Console.Out.WriteLine(GetPattern(new Triple(35,38,70).GetCube(5000)));
                FindLatticePoints(5000);


                //var allVariants = cube1.Variants.Concat(cube2.Variants).Distinct().ToArray();
                //CalculateResult(50);
            }
            finally { DatabaseHelper.Instance.Dispose(); }
        }

        private static void FindLatticePoints(int n)
        {
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
                            var insideFormulaMatches = true;
                            if (OutputCubeInfoForTriple(n, ints, false, ref insideFormulaMatches)) break;
                        } //else LogManager.GetCurrentClassLogger().Warn($"Done: {string.Join(",", ints.Select(i => i.ToString()))}");
                    }
                }
            }
        }


        private static bool OutputCubeInfoForTriple(int n, int[] ints, bool isDuplicate, ref bool insideFormulaMatches)
        {
            var baseTripleSides = ints.Take(3).ToArray();
            var triple = new Triple(baseTripleSides);
            insideFormulaMatches = true;
            if (triple.Square <= n && triple.IsPrimitive)
            {
                var baseCube = triple.GetCube(n);
                if (baseCube != null && GetPattern(baseCube).StartsWith("D"))
                {
                    var latticePoints = baseCube.LatticePoints;
                    var cbf = baseCube.GetCountsByFace(); //points on non-parallel face
                    var pointsOnEdges = baseCube.GetPointsOnEdges();
                    var pattern = GetPattern(baseCube);

                    var equation = $"{triple.Square * triple.Square * triple.Square} x + {triple.Square * triple.Square} y = {triple.Square} z + 1 t == {baseCube.LatticePointsInside}";



                    DatabaseHelper.Instance.SetDone(baseTripleSides);
                    if (!isDuplicate)
                    {
                        foreach (var duplicate in baseCube.GetDuplicateDefinitionPoints())
                        {
                            OutputCubeInfoForTriple(n, duplicate, true, ref insideFormulaMatches);
                            DatabaseHelper.Instance.SetDone(duplicate);
                        }
                    }

                    var pointsInsideSlow = baseCube.LatticePointsInside;
                    var pointsInsideFast = PointsCalculatorFast.GetSurfacePoints(baseCube);
                    var factor = Numerics.IntegralFactor(pointsInsideSlow - pointsInsideFast, (ulong)triple.Square - 1);
                    var logLevel = pointsInsideFast == pointsInsideSlow ? LogLevel.Info : LogLevel.Warn;
                    if (pointsInsideFast != pointsInsideSlow|| !insideFormulaMatches/* for duplicates*/)
                    {
                        var diff = pointsInsideSlow - pointsInsideFast;
                        var logger = pattern + (isDuplicate ? "(dup)" : "") + (!insideFormulaMatches ? "(orig)" : "");
                        LogManager.GetLogger(logger).Log(logLevel, $"{baseCube.A.Length}: {baseCube} {diff}: {factor}");
                        LogManager.GetLogger(logger).Log(logLevel, $"Side: {triple.Square}, Cube: {baseCube}, CountsByFace: {cbf.ToCsvString("/")}   {cbf.Sum()}+{pointsOnEdges}+8={cbf.Sum() + pointsOnEdges + 8}, LatticePoints: {baseCube.LatticePointsSurface - 8}+8+{baseCube.LatticePointsInside}={latticePoints}   {equation}");
                        insideFormulaMatches = false;
                    }
                }
                return false;
            }
            else return true;
        }

        private static string GetPattern(Cube c)
        {
            var maxHsf = c.Definitions.Select(GetMinHsf).Max();
            if (maxHsf > 1) return "D" + maxHsf;

            var n = (ulong)c.A.Length;
            var n2 = n * n;
            if (c.LatticePointsSurface == 8 + 2 * (n - 1) * (n + 1) + 4 * (n - 1)) return "A";
            else if (c.LatticePointsSurface == 8 + 6 * (n - 1)) return "B";
            else return "unknown";
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
                                        LogManager.GetCurrentClassLogger().Debug($"{triple}: Bounds: {baseCube.MaxBounds}, Combs: {baseCube.GetCombinations()}: Multiples: {contributions.Length}, contributions: {totalContribution} : {string.Join(",", contributions.Select(c => c.ToString()))})");
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
