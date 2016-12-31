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

        private static readonly int[][] permutations = GetAllPermutations();
        private static readonly int[][] signPermutations = GetAllSignPermutations();

        private static int[][] GetAllPermutations()
        {
            var result = new List<int[]>();
            AddPermutation(new int [] {}, result);
            return result.Where(p => !p.SequenceEqual(new[] {0, 1, 2, 3, 4, 5})).ToArray();
        }

        private static int[][] GetAllSignPermutations()
        {
            var result = new List<int[]>();
            AddSignPermutation(new int [] {}, result);
            return result.ToArray();
        }

        static void AddPermutation(int[] p, List<int[]> result)
        {
            if (p.Length == 3)
                result.Add(p);
            else
            {
                for(int i = 0; i <= 2; i++)
                    if(!p.Contains(i)) AddPermutation(p.Concat(new [] {i}).ToArray(), result);
            }
        }

        static void AddSignPermutation(int[] p, List<int[]> result)
        {
            if (p.Length == 3)
                result.Add(p);
            else
            {
                for(int i = -1; i <= 1; i+=2)
                    AddSignPermutation(p.Concat(new [] {i}).ToArray(), result);
            }
        }

        static Vector3D[] FindAllVariants(Vector3D v)
        {
            var points = new[] {v.X, v.Y, v.Z};
            var vs = new List<Vector3D>();
            foreach (var p in permutations)
            {
                foreach (var s in signPermutations)
                {
                    var variant = new Vector3D(s[0] * points[p[0]], s[1] * points[p[1]], s[2] * points[p[2]]);
                    vs.Add(variant);
                }
            }
            return vs.ToArray();
        }

        static void Main(string[] args)
        {
            MakeCubeFromVector(new Vector3D(2,3,6));
            MakeCubeFromVector(new Vector3D(1, 6, 18));
        }

        private static void MakeCubeFromVector(Vector3D a)
        {
            var o = new Vector3D(0, 0, 0);
            var vs = FindAllVariants(a);
            var b = vs.First(v => Math.Abs(Math.Abs(Vector3D.AngleBetween(v, a)) - 90) < 1e-9);
            var c = Vector3D.CrossProduct(a, b);
            c *= (a.Length/c.Length);
            Cube cube;
            if (TryMakeCubeFrom(20, o, a, b, c, out cube, true))
            {
                LogManager.GetCurrentClassLogger().Info($"Inside/Surface/Total: {cube.LatticePointsInside} / {cube.LatticePointsSurface} / {cube.LatticePoints}");
            }
        }

        const int MINSIDE = 7;
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
                                if (sideA.LengthSquared + 1e-9 >= MINSIDE*MINSIDE && 
                                    sideA.X > 1e-9 && sideA.Y > 1e-9 && sideA.Z > 1e-9 &&
                                    sideB.X > 1e-9 && sideB.Y > 1e-9 && sideB.Z > 1e-9 &&
                                    Math.Abs(sideA.LengthSquared - sideB.LengthSquared) < 1e-9 && Math.Abs(Vector3D.AngleBetween(sideA, sideB) - 90) < 1e-9)
                                {
                                    var crossProduct = Vector3D.CrossProduct(sideA, sideB);
                                    //make it the same length, and we have a choice of plus or minus
                                    var sideC1 = crossProduct*(sideA.Length/crossProduct.Length);
                                    var sideC2 = crossProduct*-(sideA.Length/crossProduct.Length);
                                    Cube cube;
                                    foreach (var sideC in new[] {sideC1,sideC2})
                                    {
                                        if (TryMakeCubeFrom(n, v1, sideA, sideB, sideC, out cube) && !cubes.Any(c => (int)c.A.LengthSquared == (int)cube.A.LengthSquared))
                                        {
                                            LogManager.GetCurrentClassLogger().Info($"Side: {cube.A.Length}, A/B/C: {cube.A} / {cube.B} / {cube.C}, Inside/Surface/Total: {cube.LatticePointsInside}/{cube.LatticePointsSurface}/{cube.LatticePoints}");
                                            cubes.Add(cube);
                                        }
                                    }
                                    
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

        static bool TryMakeCubeFrom(int n, Vector3D origin, Vector3D a, Vector3D b, Vector3D c, out Cube cube, bool skipBoundsCheck = false)
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
            if (points.All(IsIntegral) && points.All(v => skipBoundsCheck || IsInBounds(n, v)))
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

        
        static Cube[] GetCubes(int n)
        {
            var cubes = new List<Cube>();
            AddCubesFrom(n, new Vector3D[0], cubes);
            return cubes.Distinct().ToArray();
        }


    }
}
