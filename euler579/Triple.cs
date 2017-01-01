using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Windows.Media.Media3D;
using euler579;
using NLog;

namespace euler579_findtriples
{
    public class Triple
    {
        public Triple(int[] sides, int square)
        {
            Sides = sides;
            Square = square;
            Dimensions = Sides.Count(s => s > 0);
            Vector = new Vector3D(Sides[0], Sides[1], Sides[2]);
        }

        public Vector3D Vector { get;  }

        public int[] Sides { get; }
        public int Square { get; }
        public int Dimensions { get; }

        public override string ToString()
        {
            return $"{string.Join(",", Sides.Select(s => s.ToString()))} => {Square} ({Dimensions}D)";
        }

        public Cube[] GetCubes(int n)
        {
            var cubes = new List<Cube>();
            var o = new Vector3D(0, 0, 0);
            var vs = VectorVariantFinder.FindAllVariants(Vector);
            var bs = vs.Where(v => Math.Abs(v.LengthSquared - Vector.LengthSquared) < 1e-9 
                        && Math.Abs(Math.Abs(Vector3D.AngleBetween(v, Vector)) - 90) < 1e-9);
            foreach (var b in bs)
            {
                var c = Vector3D.CrossProduct(Vector, b);
                c *= (Vector.Length / c.Length);
                Cube cube;
                if (Cube.TryMakeCubeFrom(o, Vector, b, c, out cube) && cube.Dimensions.All(d => d <= n) && !cubes.Contains(cube))
                {
                    cubes.Add(cube);
                }
            }
            return cubes.ToArray();
        }
    }
}