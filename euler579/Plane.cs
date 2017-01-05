using System;
using System.Windows.Media.Media3D;

namespace euler579
{
    class Plane
    {
        public Vector3D V1 { get; set; }
        public Vector3D V2 { get; set; }
        private readonly string name;
        public int A { get; }
        public int B { get; }
        public int C { get; }
        public int D { get; }

        public override string ToString()
        {
            return $"{name}";
        }

        public Plane (Vector3D origin, Vector3D v1, Vector3D v2, string name)
        {
            this.name = name;

            //http://keisan.casio.com/has10/SpecExec.cgi?id=system/2006/1223596129
            var pointA = origin;
            var pointB = origin + v1;
            var pointC = origin + v2;
            A = ((int)Math.Round(pointB.Y) - (int)Math.Round(pointA.Y))*((int)Math.Round(pointC.Z) - (int)Math.Round(pointA.Z)) - ((int)Math.Round(pointC.Y) - (int)Math.Round(pointA.Y))*((int)Math.Round(pointB.Z) - (int)Math.Round(pointA.Z));
            B = ((int)Math.Round(pointB.Z) - (int)Math.Round(pointA.Z))*((int)Math.Round(pointC.X) - (int)Math.Round(pointA.X)) - ((int)Math.Round(pointC.Z) - (int)Math.Round(pointA.Z))*((int)Math.Round(pointB.X) - (int)Math.Round(pointA.X));
            C = ((int)Math.Round(pointB.X) - (int)Math.Round(pointA.X))*((int)Math.Round(pointC.Y) - (int)Math.Round(pointA.Y)) - ((int)Math.Round(pointC.X) - (int)Math.Round(pointA.X))*((int)Math.Round(pointB.Y) - (int)Math.Round(pointA.Y));
            D = -(A* (int)Math.Round(pointA.X) + B* (int)Math.Round(pointA.Y) + C* (int)Math.Round(pointA.Z));

        }

        // sing portion of http://forums.codeguru.com/showthread.php?378277-Distance-between-plane-and-a-point
        public int GetSide(Vector3D p)
        {
            var dist = A*p.X + B*p.Y + C*p.Z + D;
            
            var sign = Math.Sign(dist);
            if (sign != 0 && Math.Abs(dist) < 1e-6) throw new Exception("Point is very very close but sign is non-zero!");
            return sign;
        }
    }
}