using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Media3D;

namespace euler579
{
    public static class VectorVariantFinder
    {
        private static readonly int[][] permutations = GetAllPermutations();
        private static readonly int[][] signPermutations = GetAllSignPermutations();

        private static int[][] GetAllPermutations()
        {
            var result = new List<int[]>();
            AddPermutation(new int[] { }, result);
            return result.ToArray();
        }

        private static int[][] GetAllSignPermutations()
        {
            var result = new List<int[]>();
            AddSignPermutation(new int[] { }, result);
            return result.ToArray();
        }

        static void AddPermutation(int[] p, List<int[]> result)
        {
            if (p.Length == 3)
                result.Add(p);
            else
            {
                for (int i = 0; i <= 3; i++)
                    if (!p.Contains(i)) AddPermutation(p.Concat(new[] { i }).ToArray(), result);
            }
        }

        static void AddSignPermutation(int[] p, List<int[]> result)
        {
            if (p.Length == 3)
                result.Add(p);
            else
            {
                for (int i = -1; i <= 1; i += 2)
                    AddSignPermutation(p.Concat(new[] { i }).ToArray(), result);
            }
        }

        public static Vector3D[] FindAllVariants(Vector3D v)
        {
            var points = new[] { v.X, v.Y, v.Z, 0 };
            var vs = new List<Vector3D>();
            foreach (var p1 in permutations)
            {
                foreach (var p2 in permutations)
                {
                    foreach (var s in signPermutations)
                    {
                        var a = points[p1[0]] + s[0] * points[p2[0]];
                        var b = points[p1[1]] + s[1] * points[p2[1]];
                        var c = points[p1[2]] + s[1] * points[p2[2]];
                        var variant = new Vector3D(a, b, c);
                        vs.Add(variant);
                    }
                }
            }
            return vs.Distinct().ToArray();
        }

    }
}
