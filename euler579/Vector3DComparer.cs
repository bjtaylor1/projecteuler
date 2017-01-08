using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Media3D;

namespace euler579
{
    class Vector3DComparer : IComparer<Vector3D>
    {
        public static Vector3DComparer Instance { get; } = new Vector3DComparer();
        public int Compare(Vector3D lhs, Vector3D rhs)
        {
            var cs = new Func<int>[]
            {
                () => lhs.X.CompareTo(rhs.X),
                () => lhs.Y.CompareTo(rhs.Y),
                () => lhs.Z.CompareTo(rhs.Z)
            };
            foreach (var c in cs)
            {
                var res = c();
                if (res != 0) return res;
            }
            return 0;
        }
    }
}
