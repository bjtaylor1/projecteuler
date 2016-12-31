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
        static long GetNumStraightLatticePoints(long n)
        {
            long result = 0;
            long a = n, b = 2;
            while (a >= 1)
            {
                result += (long)(Math.Pow(a, 3)*Math.Pow(b, 3));
                a--;
                b++;
            }
            return result;
        }


        static void Main(string[] args)
        {
            foreach (var line in File.ReadAllLines("info.log"))
            {
                var m = Regex.Match(line, @"^C\((\d+)\)(.+)$");
                var n = int.Parse(m.Groups[1].Value);
                var rest = m.Groups[2].Value;
                var products = Regex.Matches(rest, @"(\d+) x (\d+)").Cast<Match>().Select(m1 => int.Parse(m1.Groups[2].Value))
                    .Where(i => !IsIntegral(Math.Pow(i, 1d/3)))
                    .Distinct();
                LogManager.GetCurrentClassLogger().Info($"{n}, {string.Join(", ", products.Select(s => s.ToString()))}");
            }
            return;
            for (long i = 1; i <= 14; i++)
            {
                LogManager.GetCurrentClassLogger().Info($"{i}: {GetNumStraightLatticePoints(i)}");
            }
            return;

            var cube1 = new Cube(new[]
            {
                new Vector3D(0, 0, 0),
                new Vector3D(3, 0, 0),
                new Vector3D(0, 3, 0),
                new Vector3D(0, 0, 3),
                new Vector3D(0, 3, 3),
                new Vector3D(3, 0, 3),
                new Vector3D(3, 3, 0),
                new Vector3D(3, 3, 3)
            });
            Console.Out.WriteLine(cube1.LatticePoints);

            var cube2 = new Cube(new[]
            {
                new Vector3D(0, 2, 2),
                new Vector3D(1, 4, 4),
                new Vector3D(2, 0, 3),
                new Vector3D(2, 3, 0),
                new Vector3D(3, 2, 5),
                new Vector3D(3, 5, 2),
                new Vector3D(4, 1, 1),
                new Vector3D(5, 3, 3)
            });
            Console.Out.WriteLine(cube2.LatticePoints);
            Console.WindowWidth = 200;
            for (int i = 1; i <= 30; i++)
            {
                var cubes = GetCubes(i);
                var cubesByLatticePoints = cubes.GroupBy(c => c.LatticePoints);
                var latticePointsDist = string.Join(", ", cubesByLatticePoints.OrderBy(g => g.Key).Select(g => $"{g.Count()} x {g.Key}"));
                var totalLatticePoints = cubes.Sum(c => c.LatticePoints);
                LogManager.GetCurrentClassLogger().Info($"C({i}) = {cubes.Length}, S = {latticePointsDist}, total: {totalLatticePoints}");
            }
        }

        static void AddCubesFrom(int n, Vector3D[] vertices, List<Cube> cubes )
        {
            System.Windows.Media.Media3D.Geometry3D g;
            for (int x = 0; x <= n; x++)
            {
                for (int y = 0; y <= n; y++)
                {
                    for (int z = 0; z <= n; z++)
                    {
                        var v = new Vector3D(x, y, z);
                        if (!vertices.Contains(v))
                        {
                            var newVertices = vertices.Concat(new[] {v}).ToArray();
                            if (newVertices.Length == 3)
                            {
                                var v0 = newVertices[0];
                                var v1 = newVertices[1]; //the origin of the cube
                                var v2 = newVertices[2];
                                var sideA = v0 - v1;
                                var sideB = v2 - v1;
                                if (Math.Abs(sideA.LengthSquared - sideB.LengthSquared) < 1e-9 && Math.Abs(Vector3D.AngleBetween(sideA, sideB) - 90) < 1e-9)
                                {
                                    var crossProduct = Vector3D.CrossProduct(sideA, sideB);
                                    //make it the same length, and we have a choice of plus or minus
                                    var sideC1 = crossProduct*(sideA.Length/crossProduct.Length);
                                    var sideC2 = crossProduct*-(sideA.Length/crossProduct.Length);
                                    Cube cube;
                                    if (TryMakeCubeFrom(n, v1, sideA, sideB, sideC1, out cube) && !cubes.Contains(cube)) cubes.Add(cube);
                                    if (TryMakeCubeFrom(n, v1, sideA, sideB, sideC2, out cube) && !cubes.Contains(cube)) cubes.Add(cube);
                                }
                            }
                            else
                            {
                                AddCubesFrom(n, newVertices, cubes);
                            }
                        }
                    }
                }
            }
        }

        static bool TryMakeCubeFrom(int n, Vector3D origin, Vector3D a, Vector3D b, Vector3D c, out Cube cube)
        {
            var points = new[]
            {
                origin,
                origin + a,
                origin + b,
                origin + c,
                origin + a + b,
                origin + a + c,
                origin + b + c,
                origin + a + b + c
            };
            if (points.All(IsIntegral) && points.All(v => IsInBounds(n, v)))
            {
                cube = new Cube(points, new [] { a,b,c});
                return true;
            }
            else
            {
                cube = null;
                return false;
            }

        }

        public static bool IsIntegral(Vector3D v)
        {
            var isIntegral = IsIntegral(v.X) &&
                             IsIntegral(v.Y) &&
                             IsIntegral(v.Z);
            return isIntegral;
        }

        static bool IsIntegral(double d)
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

        
        static Cube[] GetCubes(int n)
        {
            var cubes = new List<Cube>();
            AddCubesFrom(n, new Vector3D[0], cubes);
            return cubes.Distinct().ToArray();
        }


    }
}
