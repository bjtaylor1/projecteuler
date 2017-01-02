using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Media3D;

namespace euler579
{
    public class VectorIntEqualityComparer : IEqualityComparer<Vector3D>
    {
        public static VectorIntEqualityComparer Instance { get; } = new VectorIntEqualityComparer();

        public bool Equals(Vector3D l, Vector3D r)
        {
            var equal = Math.Abs(l.X - r.X) < 1e-9 && 
                Math.Abs(l.Y - r.Y) < 1e-9 && 
                Math.Abs(l.Z - r.Z) < 1e-9;
            return equal;
        }

        public int GetHashCode(Vector3D obj)
        {
            unchecked
            {
                int hashCode = (int) obj.X;
                hashCode = (hashCode*397) ^ (int) obj.Y;
                hashCode = (hashCode*397) ^ (int) obj.Z;
                return hashCode;
            }
        }
    }
}
