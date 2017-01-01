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
            var triplesAll = TripleFinder.FindTriples(500).ToArray();
            for (int index = 0; index < triplesAll.Length; index++)
            {
                var triple = triplesAll[index];
                if (!triplesAll.Take(index).Any(t => triple.IsMultipleOf(t)))
                {
                    triple.IsUnique = true;
                }
            }
            File.WriteAllLines("triples.csv", new [] {"Dimensions,A,B,C,Square,Unique"}.Concat(
                triplesAll.Select(t => $"{t.Dimensions},{string.Join(",", t.Sides.Select(s => s.ToString()))},{t.IsUnique}")));
            
//            var cubes = TripleFinder.FindTriplesSlow(n).Select(t => new { Triple = t, Cubes = t.GetCubes(n) }).ToArray();



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
