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
            
            var a = new Vector3D(4,4,7);
            var bs = VectorVariantFinder.FindAllVariantsAtRightAnglesTo(a);
            foreach (var b in bs)
            {
                Vector3D c = Vector3D.CrossProduct(a, b);
                c *= (a.Length)/c.Length;
                Cube cube;
                if(Cube.TryMakeCubeFrom(new Vector3D(0,0,0), a, b, c, out cube ))
                {
                    LogManager.GetCurrentClassLogger().Info(cube);
                }
            }
            /*
                        int n = 10;
                        var triples = new[]
            {
                            new Triple(new[] {0,0,1}, 7),
                            new Triple(new[] {0,1,0}, 7),
                            new Triple(new[] {1,0,0}, 7),
                        };
                        var cubes = triples.SelectMany(t => t.GetCubes(7)).Distinct().ToArray();
            */

            /*
                        var triples = new[]
                        {
                                        new Triple(new[] {0,3,4}, 5),
                                        new Triple(new[] {0,4,3}, 5),
                                        new Triple(new[] {3,0,4}, 5),
                                        new Triple(new[] {3,4,0}, 5),
                                        new Triple(new[] {4,0,3}, 5),
                                        new Triple(new[] {4,3,0}, 5),
                                    };
                        var cubes = triples.SelectMany(t => t.GetCubes(7)).Distinct().ToArray();
                        // = 6
            */


            //var cubes = TripleFinder.FindTriples(n).Select(t => new { Triple = t, Cubes = t.GetCubes(n) }).ToArray();



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
