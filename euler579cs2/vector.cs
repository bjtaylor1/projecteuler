using System;

namespace euler579cs2
{
    public class vector : vertex
    {
        public long gcd()
        {
            return new[] {x, y, z}.gcd();
        }

        public long length { get; }
        public vector(long x, long y, long z) : base(x, y, z)
        {
            long sumSquares = (x * x) + (y * y) + (z * z);
            length = (long)Math.Round(Math.Sqrt(sumSquares));
            if (length * length != sumSquares) throw new InvalidOperationException($"Vector length not integral for {x},{y},{z}");
        }

        public bool is_orthogonal_to(vector rhs)
        {
            double angleRads = Math.Acos((double)dot_product(rhs) / (length * rhs.length));
            double angleDegs = angleRads * 180 / Math.PI;
            bool is_orth = Math.Abs(angleDegs - 90) < 1e-9;
            return is_orth;
        }

        public vector cross_product(vector rhs)
        {

            long newx = y * rhs.z - z * rhs.y;
            long newy = z * rhs.x - x * rhs.z;
            long newz = x * rhs.y - y * rhs.x;

            vector n = new vector(newx, newy, newz);
            n = n / (n.length / rhs.length);
            return n;
        }

        public static vector operator /(vector lhs, long f)
        {
            long newx = lhs.x / f;
            long newy = lhs.y / f;
            long newz = lhs.z / f;
            return new vector(newx, newy, newz);
        }

        long dot_product(vector rhs)
        {
            return x * rhs.x + y * rhs.y + z * rhs.z;
        }
    }
}