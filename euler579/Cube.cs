using System;
using System.Linq;
using System.Windows.Media.Media3D;

namespace euler579
{
    internal class Cube : IEquatable<Cube>
    {
        public Vector3D[] Definitions { get; }
        private readonly Lazy<bool> isStraight;
        public bool IsStraight { get { return isStraight.Value; } }
        public int LatticePointsInside { get { return latticePoints.Value.Item1; } }
        public int LatticePointsSurface { get { return latticePoints.Value.Item2; } }
        public int LatticePoints { get { return LatticePointsInside + LatticePointsSurface; } }

        public bool Regular
        {
            get { return Program.IsIntegral(Math.Pow(LatticePoints, 1.0/3)); }
        }

        public Vector3D[] Vertices { get;  }
        public Vector3D[] OrderedVertices { get; }
        public Vector3D A { get { return Definitions[0]; } }

        public Vector3D UniqueA
        {
            get { return uniqueA.Value; }
        }

        public Vector3D B { get { return Definitions[1]; } }
        public Vector3D C { get { return Definitions[2]; } }
        public Vector3D O { get { return Vertices[0]; } }

        private static readonly Vector3D xAxis = new Vector3D(1, 0, 0);
        private static readonly Vector3D yAxis = new Vector3D(0, 1, 0);
        private static readonly Vector3D zAxis = new Vector3D(0, 0, 1);
        private Lazy<Tuple<int,int>> latticePoints;
        private Lazy<Vector3D> uniqueA;

        public Cube(Vector3D[] vertices, Vector3D[] definitions = null)
        {
            if (definitions == null) definitions = vertices.Skip(1).Take(3).Select(v => v - vertices[0]).ToArray();
            Definitions = definitions;
            if (vertices.Length != 8) throw new InvalidOperationException("Sanity check failed.");
            OrderedVertices = vertices.OrderBy(v => v.X).ThenBy(v => v.Y).ThenBy(v => v.Z).ToArray();
            Vertices = vertices.ToArray();
            isStraight = new Lazy<bool>(CalculateIsStraight);
            latticePoints = new Lazy<Tuple<int,int>>(CalculateLatticePoints);
            uniqueA = new Lazy<Vector3D>(CalculateUniqueA);
        }

        private Vector3D CalculateUniqueA()
        {
            var points = new[] {A.X, A.Y, A.Z}.Select(Math.Abs).ToArray();
            Array.Sort(points);
            var result = new Vector3D(points[0], points[1], points[2]);
            return result;
        }

        private Tuple<int, int> CalculateLatticePoints()
        {
            var front = new Plane(O, A, B, "front");
            var back = new Plane(O+C, A, B, "back");
            var left = new Plane(O, B, C, "left");
            var right = new Plane(O + A, B, C, "right");
            var bottom = new Plane(O, A, C, "bottom");
            var top = new Plane(O + B, A, C, "top");

            var oppositeFaces = new[]
            {
                new PlanePair(front,back), 
                new PlanePair(left,right), 
                new PlanePair(bottom, top)
            };
            var allFaces = new[] {front, back, left, right, bottom, top};
            
            int inside = 0;
            int surface = 0;
            var minBounds = new Vector3D(Vertices.Min(v => v.X), Vertices.Min(v => v.Y), Vertices.Min(v => v.Z));
            var maxBounds = new Vector3D(Vertices.Max(v => v.X), Vertices.Max(v => v.Y), Vertices.Max(v => v.Z));

            for (int x = (int)Math.Round(minBounds.X,0); x <= (int)Math.Round(maxBounds.X,0); x++)
            {
                for (int y = (int) Math.Round(minBounds.Y,0); y <= (int)Math.Round(maxBounds.Y,0); y++)
                {
                    for(int z= (int)Math.Round(minBounds.Z, 0); z <= (int)Math.Round(maxBounds.Z,0); z++)
                    {
                        var v = new Vector3D(x,y,z);
                        if (oppositeFaces.All(pp => pp.IsBetweenOrOn(v)))
                        {
                            if (allFaces.Any(p => p.GetSide(v) == 0))
                                surface++;
                            else
                                inside++;
                        }
                    }
                }
            }
            return Tuple.Create(inside, surface);
        }

        private bool CalculateIsStraight()
        {
            var straight = Definitions.All(p =>
            {
                var result = 
                    IsParallelToAxis(p, xAxis) || 
                    IsParallelToAxis(p, yAxis) || 
                    IsParallelToAxis(p, zAxis);
                return result;
            });
            return straight;
        }

        private static bool IsParallelToAxis(Vector3D vector3D, Vector3D axis)
        {
            var angleBetweenXAxis = Vector3D.AngleBetween(vector3D, axis);
            var result = (int) Math.Round(Math.Abs(angleBetweenXAxis), 0)%90 == 0;
            return result;
        }

        public override string ToString()
        {
            return string.Join("   ", OrderedVertices.Select(o => $"{{{o.X:0},{o.Y:0},{o.Z:0}}}"));
        }

        public bool Equals(Cube other)
        {
            if (ReferenceEquals(null, other)) return false;
            if (ReferenceEquals(this, other)) return true;
            var sequenceEqual = OrderedVertices.SequenceEqual(other.OrderedVertices);
            return sequenceEqual;
        }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            if (ReferenceEquals(this, obj)) return true;
            if (obj.GetType() != this.GetType()) return false;
            return Equals((Cube) obj);
        }

        public override int GetHashCode()
        {
            unchecked
            {
                return OrderedVertices?.Aggregate(0, (i, v) => i*367 ^ ((int)v.X*419) ^ ((int)v.Y*293) ^ ((int)v.Z*137)) ?? 0;
            }
        }

        public static bool operator ==(Cube left, Cube right)
        {
            return Equals(left, right);
        }

        public static bool operator !=(Cube left, Cube right)
        {
            return !Equals(left, right);
        }
    }
}