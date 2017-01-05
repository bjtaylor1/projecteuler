using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Media3D;
using NLog;

namespace euler579
{
    class BruteForceCubeFinder
    {
        public static VectorInt[] FindVectorsAtRightAnglesTo(VectorInt v)
        {
            var vectors = new List<VectorInt>();
            var n = (int)v.Length;
            for (int x = -n; x <= n; x++)
            {
                for (int y = -n; y <= n; y++)
                {
                    for (int z = -n; z <= n; z++)
                    {
                        if (!(x == 0 && y == 0 && z == 0))
                        {
                            var vr = new VectorInt(x, y, z);
                            if (Math.Abs(vr.LengthSquared - v.LengthSquared) < 1e-9 &&
                                Math.Abs(Math.Abs(VectorInt.AngleBetween(vr, v)) - 90) < 1e-9)
                            {
                                vectors.Add(vr);
                            }
                        }
                    }
                }
            }
            return vectors.Distinct().ToArray();
        }


        public static void AddCubesFrom(int n, VectorInt[] vertices, List<Cube> cubes)
        {
            System.Windows.Media.Media3D.Geometry3D g;
            for (int x = 0; x <= n; x++)
            {
                for (int y = 0; y <= n; y++)
                {
                    for (int z = 0; z <= n; z++)
                    {
                        var v = new VectorInt(x, y, z);

                        if (!vertices.Contains(v))
                        {
                            var newVertices = vertices.Concat(new[] { v }).ToArray();
                            if (newVertices.Length == 3)
                            {
                                var v0 = newVertices[0];
                                var v1 = newVertices[1]; //the origin of the cube
                                var v2 = newVertices[2];
                                var sideA = v0 - v1;
                                var sideB = v2 - v1;
                                if (Math.Abs(sideA.LengthSquared - sideB.LengthSquared) < 1e-9 && Math.Abs(VectorInt.AngleBetween(sideA, sideB) - 90) < 1e-9)
                                {
                                    var crossProduct = VectorInt.CrossProduct(sideA, sideB);
                                    //make it the same length, and we have a choice of plus or minus
                                    var sideC1 = crossProduct * (sideA.Length / crossProduct.Length);
                                    var sideC2 = crossProduct * -(sideA.Length / crossProduct.Length);
                                    Cube cube;
                                    foreach (var sideC in new[] { sideC1, sideC2 })
                                    {
                                        throw new NotImplementedException("No bounds check...");
                                        if (Cube.TryMakeCubeFrom(v1, sideA, sideB, sideC, out cube) && !cubes.Any(c => (int)c.A.LengthSquared == (int)cube.A.LengthSquared))
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
    }
}
