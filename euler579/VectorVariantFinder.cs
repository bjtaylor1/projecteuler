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
        public static Vector3D[] FindAllVariantsAtRightAnglesTo(Vector3D vector)
        {
            return FindAllVariants(vector, (vn, v) => Math.Abs(Math.Abs(Vector3D.AngleBetween(vn, v)) - 90) < 1e-9, ints => true);
        }

        public static Vector3D[] FindAllCombinationsOf(Vector3D v)
        {
/*
            var quants = new[]
            {
                new[] {1, 1, 1},
                new[] {1, 1, -1},
                new[] {1, -1, 1},
                new[] {1, -1, -1},
                new[] {-1, 1, 1},
                new[] {-1, 1, -1},
                new[] {-1, -1, 1},
                new[] {-1, -1, -1},
            };
*/
            var quants = Permutations.Of(new[] {-1, 1}, 3, true, false);
            var allPoints = quants.Select(q => new[] {(int)(q[0]*v.X), (int)(q[1]*v.Y), (int)(q[2]*v.Z)}).ToArray();
            var perms = allPoints.SelectMany(points => Permutations.Of(points, 3, false, false));
            var vectors = perms.Select(ints => new Vector3D(ints[0], ints[1], ints[2])).ToArray();
            return vectors;
        }

        public static Vector3D[] FindAllVariants(Vector3D v, Func<Vector3D, Vector3D, bool> predicate, Func<int[], bool> numbersPredicate  )
        {
            var points = new[] { (int)v.X, (int)v.Y, (int)v.Z};
            var pointsPerms = new[] {points};// Permutations.Of(points,3 , false, false);
            var possibleVectors = pointsPerms.SelectMany(p => PossibleVectors(v, predicate, numbersPredicate, p)).Distinct().ToArray();
            return possibleVectors;
        }

        private static Vector3D[] PossibleVectors(Vector3D v, Func<Vector3D, Vector3D, bool> predicate, Func<int[], bool> numbersPredicate, int[] points)
        {
            // must allow 0 in order to work with e.g. 4,4,7.
            var possibleQuantities = Permutations.Of(new[] { -1,0, 1 }, 3, true, false);
            var possibleNumbers = possibleQuantities.Select(qs => qs.Select((q, i) => q*points[i]).Sum()).OrderBy(n => n).Distinct().ToArray();
            var possibleVectors = Permutations.Of(possibleNumbers, 3, true, false)
                .Where(numbersPredicate)
                .Select(nums => new Vector3D(nums[0], nums[1], nums[2]))
                .Where(vn => Math.Abs(vn.LengthSquared - v.LengthSquared) < 1e-9 && predicate(vn, v))
                .ToArray();
            return possibleVectors;
        }
    }
}
