using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace euler579
{
    public static class VectorVariantFinder
    {
        public static VectorInt[] FindAllVariantsAtRightAnglesTo(VectorInt vector)
        {
            var variantsAtRightAnglesTo = 
                FindAllVariants(vector, vn => vn.IsAtRightAnglesTo(vector), ints => true);

            if (!variantsAtRightAnglesTo.Any())
            {
                variantsAtRightAnglesTo = DatabaseHelper.Instance.GetAllPrimitivesOfSameSquare(vector).SelectMany(vAlt => 
                    FindAllVariants(vAlt, vn => vn.IsAtRightAnglesTo(vector), ints => true)).ToArray();
                
                if(!variantsAtRightAnglesTo.Any())
                    variantsAtRightAnglesTo = BruteForceCubeFinder.FindVectorsAtRightAnglesTo(vector);
            }
            return variantsAtRightAnglesTo;
        }

        public static VectorInt[] FindAllCombinationsOf(VectorInt v)
        {
            var quants = Permutations.Of(new[] {-1, 1}, 3, true, false);
            var allPoints = quants.Select(q => new[] {(int)(q[0]*v.X), (int)(q[1]*v.Y), (int)(q[2]*v.Z)}).ToArray();
            var perms = allPoints.SelectMany(points => Permutations.Of(points, 3, false, false));
            var vectors = perms.Select(ints => new VectorInt(ints[0], ints[1], ints[2])).ToArray();
            return vectors;
        }

        public static VectorInt[] FindAllVariants(VectorInt v, Func<VectorInt, bool> predicate, Func<int[], bool> numbersPredicate  )
        {
            var points = new[] { (int)v.X, (int)v.Y, (int)v.Z};
            var pointsPerms = new[] {points};// Permutations.Of(points,3 , false, false);
            var possibleVectors = pointsPerms.SelectMany(p => PossibleVectors(v, predicate, numbersPredicate, p)).Distinct().ToArray();
            return possibleVectors;
        }

        static readonly int[] factors = { -1,0, 1 };
        // must allow 0 in order to work with e.g. 4,4,7.
        static readonly int[][] possibleQuantities = Permutations.Of(factors, 3, true, false);

        private static VectorInt[] PossibleVectors(VectorInt v, Func<VectorInt, bool> predicate, Func<int[], bool> numbersPredicate, int[] points)
        {
            var possibleNumbers = possibleQuantities.Select(qs => qs.Select((q, i) => q*points[i]).Sum()).OrderBy(n => n).Distinct().ToArray();
            var possibleVectors = Permutations.Of(possibleNumbers, 3, true, false)
                .Where(numbersPredicate)
                .Select(nums => new VectorInt(nums[0], nums[1], nums[2]))
                .Where(vn => Math.Abs(vn.LengthSquared - v.LengthSquared) < 1e-9 && predicate(vn))
                .ToArray();
            return possibleVectors;
        }
    }
}
