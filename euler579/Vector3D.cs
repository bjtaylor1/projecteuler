﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Media3D;
using MS.Internal.Media3D;

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

        public static VectorInt CrossProduct(VectorInt vector1, VectorInt vector2)
        {
            Vector3D v1;
            Vector3D v2;
            
            VectorInt result;
            result._x = vector1._y * vector2._z - vector1._z * vector2._y;
            result._y = vector1._z * vector2._x - vector1._x * vector2._z;
            result._z = vector1._x * vector2._y - vector1._y * vector2._x;
            return result;
        }

        public static double AngleBetween(VectorInt vector1, VectorInt vector2)
        {
            var rads = DotProduct(vector1, vector2) >= 0.0 ? 
                2.0 * Math.Asin((vector1 - vector2).Length / 2.0) : 
                Math.PI - 2.0 * Math.Asin((-vector1 - vector2).Length / 2.0);
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

        public static Vector3D operator -(VectorInt vector1, VectorInt vector2)
        {
            return new Vector3D(vector1._x - vector2._x, vector1._y - vector2._y, vector1._z - vector2._z);
        }

        public static Vector3D operator +(VectorInt vector1, VectorInt vector2)
        {
            return new Vector3D(vector1._x + vector2._x, vector1._y + vector2._y, vector1._z + vector2._z);
        }


    }
}
