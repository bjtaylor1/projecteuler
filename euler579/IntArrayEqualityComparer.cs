using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace euler579
{
    public class IntArrayEqualityComparer : IEqualityComparer<int[]>
    {
        public static IntArrayEqualityComparer Instance { get; } = new IntArrayEqualityComparer();
        public bool Equals(int[] x, int[] y)
        {
            return x.SequenceEqual(y);
        }

        public int GetHashCode(int[] obj)
        {
            unchecked
            {
                return obj.Aggregate(397, (i, n) => (i*397) ^ n);
            }
        }
    }
}
