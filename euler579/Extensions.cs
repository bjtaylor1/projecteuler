using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Media3D;

namespace euler579
{
    public static class Extensions
    {
        public static bool IsAtRightAnglesTo(this VectorInt vn, VectorInt vector)
        {
            return Math.Abs(Math.Abs(VectorInt.AngleBetween(vn, vector)) - 90) < 1e-9;
        }

        public static string ToCsvString<T>(this IEnumerable<T> items, string separator = ", ")
        {
            return string.Join(separator, items.Select(t => t.ToString()));
        }
    }
}
