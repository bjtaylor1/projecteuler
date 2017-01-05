using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace euler579
{
    public struct VectorInt
    {
        int _x, _y, _z;
        public int X { get { return _x; } }
        public int Y { get { return _y; } }
        public int Z { get { return _z; } }

        public VectorInt(int x, int y, int z)
        {
            _x = x;
            _y = y;
            _z = z;

        }

        public int LengthSquared
        {
            get
            {
                return _x* _x +_y * _y + _z * _z;
            }
        }

        public int Length
        {
            get
            {
                var sqrt = Math.Sqrt(LengthSquared);
                if(!Numerics.IsIntegral(sqrt)) throw new InvalidOperationException("Length is not integral");
                return (int) sqrt;
            }
        }

        private double LengthDouble //MUST be private
        {
            get
            {
                return Math.Sqrt(LengthSquared);
            }
        }

        public static VectorInt CrossProduct(VectorInt vector1, VectorInt vector2)
        {
            VectorInt result;
            result._x = vector1._y * vector2._z - vector1._z * vector2._y;
            result._y = vector1._z * vector2._x - vector1._x * vector2._z;
            result._z = vector1._x * vector2._y - vector1._y * vector2._x;
            return result;
        }

        public static double AngleBetween(VectorInt vector1, VectorInt vector2)
        {
            var rads = DotProduct(vector1, vector2) >= 0.0 ? 
                2.0 * Math.Asin((vector1 - vector2).LengthDouble / 2.0) : 
                Math.PI - 2.0 * Math.Asin((-vector1 - vector2).LengthDouble / 2.0);
            var degs = rads*180/Math.PI;
            return degs;
        }

        public static double DotProduct(VectorInt vector1, VectorInt vector2)
        {
            return DotProduct(ref vector1, ref vector2);
        }

        internal static double DotProduct(ref VectorInt vector1, ref VectorInt vector2)
        {
            return vector1._x * vector2._x + vector1._y * vector2._y + vector1._z * vector2._z;
        }

        public static VectorInt operator -(VectorInt vector)
        {
            return new VectorInt(-vector._x, -vector._y, -vector._z);
        }

        public static VectorInt operator -(VectorInt vector1, VectorInt vector2)
        {
            return new VectorInt(vector1._x - vector2._x, vector1._y - vector2._y, vector1._z - vector2._z);
        }

        public static VectorInt operator +(VectorInt vector1, VectorInt vector2)
        {
            return new VectorInt(vector1._x + vector2._x, vector1._y + vector2._y, vector1._z + vector2._z);
        }

        public static VectorInt operator *(VectorInt vector1, double d)
        {
            var newX = Numerics.ToIntSafe(vector1._x * d);
            var newY = Numerics.ToIntSafe(vector1._y * d);
            var newZ = Numerics.ToIntSafe(vector1._z * d);
            return new VectorInt(newX, newY, newZ);
        }

        public static bool TryMultiply(VectorInt vector1, double factor, out VectorInt result)
        {
            var newX = (vector1._x * factor);
            var newY = (vector1._y * factor);
            var newZ = (vector1._z * factor);
            if (Numerics.IsIntegral(newX) && Numerics.IsIntegral(newY) && Numerics.IsIntegral(newZ))
            {
                result = new VectorInt((int) Math.Round(newX), (int) Math.Round(newY), (int) Math.Round(newZ));
                return true;
            }
            else
            {
                result = default(VectorInt);
                return false;
            }
        }
    }
}
