using System;
using System.Windows.Media.Media3D;

namespace euler579
{
    class Plane
    {
        public Vector3D Origin { get; set; }
        public Vector3D V1 { get; set; }
        public Vector3D V2 { get; set; }
        private readonly string name;
        public double A { get; }
        public double B { get; }
        public double C { get; }
        public double D { get; }

        public override string ToString()
        {
            return $"{name}";
        }

        public Plane (Vector3D origin, Vector3D v1, Vector3D v2, string name)
        {
            Origin = origin;
            V1 = v1;
            V2 = v2;
            this.name = name;
            var normal = Vector3D.CrossProduct(v1, v2);
            A  = normal.X;
            B = normal.Y;
            C = normal.Z;
            D = -Vector3D.DotProduct(origin, normal);
        }

        // sing portion of http://forums.codeguru.com/showthread.php?378277-Distance-between-plane-and-a-point
        public int GetSide(Vector3D p)
        {
            return Math.Sign(A*p.X + B*p.Y + C*p.Z + D);
        }

    }
}