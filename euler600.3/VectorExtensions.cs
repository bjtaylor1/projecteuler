using System;
using System.Windows;

namespace euler600
{
    public static class VectorExtensions
    {
        public static Vector Rotate(this Vector v, double radians)
        {
            var ca = Math.Cos(radians);
            var sa = Math.Sin(radians);
            Vector rotated = new Vector(ca * v.X - sa * v.Y, sa * v.X + ca * v.Y);
            return rotated;
        }
    }
}
