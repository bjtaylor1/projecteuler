using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace euler600
{
    public class Vector : IEquatable<Vector>
    {
        public double X, Y;
        public Vector(double x, double y)
        {
            X = x;
            Y = y;
        }

        public override bool Equals(object obj)
        {
            if (!(obj is Vector)) return false;
            var rhs = (Vector)obj;
            return Equals(rhs);
        }

        public override int GetHashCode()
        {
            return ToString().GetHashCode();
        }

        public override string ToString()
        {
            return $"{X:0.000}, {Y:0.000}";
        }

        public bool Equals(Vector rhs)
        {
            return Math.Abs(X - rhs.X) < 1e-10 && Math.Abs(Y - rhs.Y) < 1e-10;
        }

        public static Vector operator +(Vector lhs, Vector rhs)
        {
            return new Vector(lhs.X + rhs.X, lhs.Y + rhs.Y);
        }

        public static Vector operator-(Vector v)
        {
            return new Vector(-v.X, -v.Y);
        }
    }
}
