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
                for (int i = -1; i <= 1; i += 1)
                    AddSignPermutation(p.Concat(new[] { i }).ToArray(), result);
            }
        }

        public static Vector3D[] FindAllVariantsAtRightAnglesTo(Vector3D v)
        {
            var points = new[] { (int)v.X, (int)v.Y, (int)v.Z};
            var possibleQuantities = Permutations.Of(new[] {-1, 0, 1}, 3, true);
            var possibleNumbers = possibleQuantities.Select(qs => qs.Select((q, i) => q*points[i]).Sum()).OrderBy(n => n).Distinct().ToArray();
            var possibleVectors = Permutations.Of(possibleNumbers, 3, true)
                .Select(nums => new Vector3D(nums[0], nums[1], nums[2]))
                .Where(vn => Math.Abs(vn.LengthSquared - v.LengthSquared) < 1e-9 &&
                    Math.Abs(Math.Abs(Vector3D.AngleBetween(vn, v)) - 90) < 1e-9)
                .ToArray();

            return possibleVectors;
        }

    }
}
