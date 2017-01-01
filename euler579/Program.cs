using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.ComponentModel;
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
            using(var sw =new StreamWriter("triples.csv"))
            {
                var n = 5000;
                TripleFinder.FindTriples(n, triple =>
                {
                    if(triple.Square > n) throw new InvalidOperationException("too big.");
                    // ReSharper disable once AccessToDisposedClosure
                    sw.WriteLine($"{string.Join(",", triple.Sides.Select(i => i.ToString()))},{triple.Square}");
                });
            }
        }

        private static void CalculateResult() //correct
        {
            var allTriples = File.ReadAllLines("triples.csv")
                .Select(line => line.Split(',').Select(int.Parse).ToArray())
                .Select(ints => new Triple(ints.Take(3).ToArray(), ints.Last()))
                .ToArray();
            for (int n = 1; n <= 10; n++)
            {
                var triples = allTriples
                    .Where(t => t.Square <= n)
                    .ToArray();
                var cubes = triples.SelectMany(t => t.GetCubes(n)).ToArray();
                var s = cubes.Sum(c =>
                {
                    var repeatability = c.GetRepeatability(n);
                    var combinations = c.GetCombinations();
                    var i = c.LatticePoints*repeatability*combinations;
                    return i;
                });
                LogManager.GetCurrentClassLogger().Info($"n = {n}, S = {s}");
            }
        }

        static long GetNumStraightLatticePoints(long n)
        {
            long result = 0;
            long a = n, b = 2;
            while (a >= 1)
            {
                result += (long)(Math.Pow(a, 3) * Math.Pow(b, 3));
                a--;
                b++;
            }
            return result;
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
