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
                //Console.Out.WriteLine(Prime.GetHighestPrimeFactor(35));
                //                var c = new Triple(1, 2, 2).GetCube(50);
                //                var latticePoints = c.LatticePoints;
                //                LogManager.GetCurrentClassLogger().Info($"{string.Join(", ", c.Vertices.Select(v => v.ToString()))} {latticePoints}");

                FindLatticePoints(5000);
                //CalculateResult(5000);
                //                FindTripleDuplicates();
                //var cube0 = new Triple(new[] {1,2,2}).GetCube(50);
                //            var c1 = new Triple(new[] {1,4,8}).GetCube(50);
                //            var c2 = new Triple(new[] {4,4, 7}).GetCube(50);
                //            var cubes = c1.Variants.Concat(c1.Variants).Distinct().ToArray();


                //var allVariants = cube1.Variants.Concat(cube2.Variants).Distinct().ToArray();
                //CalculateResult(50);
            }
            finally { DatabaseHelper.Instance.Dispose(); }
        }

        private static void FindTripleDuplicates()
        {
            using (var output = File.OpenWrite("primitivetripleduplicates.csv"))
            using (var file = File.OpenRead("primitivetriplessorted.csv"))
            {
                using (var sr = new StreamReader(file))
                using (var sw = new StreamWriter(output))

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
                                var outputLine = string.Join(",", triple.Sides.Select(s => s.ToString())) + "," + string.Join(",", duplicateTriple.Sides.Select(s => s.ToString()));
                                sw.WriteLine(outputLine);
                            }
                        }

                    }
                }
            }
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
                            if (OutputCubeInfoForTriple(n, ints, true)) break;
                        } //else LogManager.GetCurrentClassLogger().Warn($"Done: {string.Join(",", ints.Select(i => i.ToString()))}");
                    }
                }
            }
        }

        private static bool OutputCubeInfoForTriple(int n, int[] ints, bool analyzeDuplicates)
        {
            var baseTripleSides = ints.Take(3).ToArray();
            var tripleSquare = ints.Last();
            var triple = new Triple(baseTripleSides, tripleSquare);

            if (triple.Square <= n)
            {
                var baseCube = triple.GetCube(n);
                if (baseCube != null)
                {
                    var latticePoints = baseCube.LatticePoints;
                    var cbf = baseCube.GetCountsByFace(); //points on non-parallel face
                    var pointsOnEdges = baseCube.GetPointsOnEdges();
                    var pattern = GetPattern(baseCube);
                    var logLevel = analyzeDuplicates && pattern == "unknown" ? LogLevel.Warn : LogLevel.Info;
                    var factor = (cbf[0] + 1)/triple.Square;
                    var equation = $"{triple.Square*triple.Square*triple.Square} x + {triple.Square*triple.Square} y = {triple.Square} z + 1 t == {baseCube.LatticePointsInside}";
                    LogManager.GetCurrentClassLogger().Log(logLevel, $"Side: {triple.Square}, Factor: {factor}, Cube: {baseCube}, CountsByFace: {cbf.ToCsvString("/")}   {cbf.Sum()}+{pointsOnEdges}+8={cbf.Sum() + pointsOnEdges + 8}, LatticePoints: {baseCube.LatticePointsSurface - 8}+8+{baseCube.LatticePointsInside}={latticePoints}   {equation}");

                    DatabaseHelper.Instance.SetDone(baseTripleSides);
                    foreach (var duplicate in baseCube.GetDuplicateDefinitionPoints())
                    {
                        if (analyzeDuplicates && pattern == "unknown") OutputCubeInfoForTriple(n, duplicate, false);
                        DatabaseHelper.Instance.SetDone(duplicate);
                    }
                }
                return false;
            }
            else return true;
        }

        private static string GetPattern(Cube c)
        {
            var n = (ulong)c.A.Length;
            var n2 = n * n;
            if (c.LatticePointsSurface == 8 + 2 * (n - 1) * (n + 1) + 4 * (n - 1)) return "A";
            else if (c.LatticePointsSurface == 8 + 6 * (n - 1)) return "B";
            else return "unknown";
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
