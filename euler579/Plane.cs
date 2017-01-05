using System;
using System.Windows.Media.Media3D;

namespace euler579
{
    class Plane
    {
        public VectorInt V1 { get; set; }
        public VectorInt V2 { get; set; }
        private readonly string name;
        public int A { get; }
        public int B { get; }
        public int C { get; }
        public int D { get; }

        public override string ToString()
        {
            return $"{name}";
        }

        public Plane (VectorInt origin, VectorInt v1, VectorInt v2, string name)
        {
            this.name = name;

            //http://keisan.casio.com/has10/SpecExec.cgi?id=system/2006/1223596129
            var pointA = origin;
            var pointB = origin + v1;
            var pointC = origin + v2;
            A = (pointB.Y - pointA.Y)*(pointC.Z - pointA.Z) - (pointC.Y - pointA.Y)*(pointB.Z - pointA.Z);
            B = (pointB.Z - pointA.Z)*(pointC.X - pointA.X) - (pointC.Z - pointA.Z)*(pointB.X - pointA.X);
            C = (pointB.X - pointA.X)*(pointC.Y - pointA.Y) - (pointC.X - pointA.X)*(pointB.Y - pointA.Y);
            D = -(A* pointA.X + B* pointA.Y + C* pointA.Z);
        }

        // sing portion of http://forums.codeguru.com/showthread.php?378277-Distance-between-plane-and-a-point
        public int GetSide(VectorInt p)
        {
            var dist = A*p.X + B*p.Y + C*p.Z + D;
            
            var sign = Math.Sign(dist);
            if (sign != 0 && Math.Abs(dist) < 1e-6) throw new Exception("Point is very very close but sign is non-zero!");
            return sign;
        }
    }
}