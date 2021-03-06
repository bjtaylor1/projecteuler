using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Media.Media3D;
using NLog;

namespace euler579
{
    public class Cube : IEquatable<Cube>
    {
        public int NumDimensions { get; }
        public Vector3D[] Definitions { get; }
        public ulong LatticePointsInside { get { return latticePoints.Value.Item1; } }
        public ulong LatticePointsSurface { get { return latticePoints.Value.Item2; } }

        public ulong LatticePoints
        {
            get
            {
                //if (NumDimensions == 0) return (ulong) Math.Round(Math.Pow(Math.Round(A.Length + 1), 3));
                return LatticePointsInside + LatticePointsSurface;
            }
        }

        public bool Regular
        {
            get { return Numerics.IsIntegral(Math.Pow(LatticePoints, 1.0/3)); }
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
        private static readonly Vector3D[] axes = {xAxis, yAxis, zAxis};
        private Lazy<Tuple<ulong, ulong>> latticePoints;
        private Lazy<Vector3D> uniqueA;
        public Vector3D MinBounds { get; }
        public Vector3D MaxBounds { get; }
        private static readonly Vector3D ORIGIN = new Vector3D(0, 0, 0);
        private List<Tuple<Vector3D, string[]>> pointsByFace;
        private Plane firstNonParallelFace;
        private Plane front;
        private Plane back;
        private Plane left;
        private Plane right;
        private Plane bottom;
        private Plane top;
        private PlanePair[] oppositeFaces;
        private Plane[] allFaces;

        public Cube(Vector3D[] vertices, Vector3D[] definitions = null)
        {
            MinBounds = new Vector3D(vertices.Min(v => v.X), vertices.Min(v => v.Y), vertices.Min(v => v.Z));
            if (!Equals(MinBounds, ORIGIN))
            {
                vertices = vertices.Select(v => v - MinBounds).ToArray();
                MinBounds = ORIGIN;
            }
            pointsByFace = new List<Tuple<Vector3D, string[]>>();
            MaxBounds = new Vector3D(vertices.Max(v => v.X), vertices.Max(v => v.Y), vertices.Max(v => v.Z));
            if (definitions == null) definitions = vertices.Skip(1).Take(3).Select(v => v - vertices[0]).ToArray();
            Definitions = definitions;
            if (vertices.Length != 8) throw new InvalidOperationException("Sanity check failed.");
            OrderedVertices = vertices.OrderBy(v => v.X).ThenBy(v => v.Y).ThenBy(v => v.Z).ToArray();
            Vertices = vertices.ToArray();
            latticePoints = new Lazy<Tuple<ulong, ulong>>(CalculateLatticePoints);
            uniqueA = new Lazy<Vector3D>(CalculateUniqueA);
            Width = (int)Math.Abs(Math.Round(MinBounds.X, 0) - (int)Math.Round(MaxBounds.X, 0));
            Height = (int)Math.Abs(Math.Round(MinBounds.Y, 0) - (int)Math.Round(MaxBounds.Y, 0));
            Depth = (int)Math.Abs(Math.Round(MinBounds.Z, 0) - (int)Math.Round(MaxBounds.Z, 0));
            Dimensions = new[] {Width, Height, Depth};
            var numAxesParallel = axes.Count(a => definitions.Any(d => IsParallelToAxis(d,a)));
            NumDimensions = 3 - numAxesParallel;

            front = new Plane(O, A, B, "front");
            back = new Plane(O + C, A, B, "back");
            left = new Plane(O, B, C, "left");
            right = new Plane(O + A, B, C, "right");
            bottom = new Plane(O, A, C, "bottom");
            top = new Plane(O + B, A, C, "top");

            oppositeFaces = new[]
            {
                new PlanePair(front,back),
                new PlanePair(left,right),
                new PlanePair(bottom, top)
            };
            allFaces = new[] { front, back, left, right, bottom, top };

        }

        public int[][] GetDuplicateDefinitionPoints()
        {
            var allDefinitionPoint = Definitions.Select(v =>
            {
                var ints = new[] {(int) Math.Round(Math.Abs(v.X)), (int)Math.Round(Math.Abs(v.Y)), (int) Math.Round(Math.Abs(v.Z))};
                Array.Sort(ints);
                return ints;
            }).ToArray();
            var distinctDefinitionsPoints = allDefinitionPoint.Distinct(IntArrayEqualityComparer.Instance).ToArray();
            var exceptA = distinctDefinitionsPoints.Except(new[] {allDefinitionPoint[0]}, IntArrayEqualityComparer.Instance).ToArray();
            return exceptA;
        }

        public int[] Dimensions { get;}
        public int Depth { get;  }
        public int Height { get; }
        public int Width { get;  }
        public int? RepeatabilityCombinations { get; set; }
        public Cube[] Variants { get; set; }

        private Vector3D CalculateUniqueA()
        {
            var points = new[] {A.X, A.Y, A.Z}.Select(Math.Abs).ToArray();
            Array.Sort(points);
            var result = new Vector3D(points[0], points[1], points[2]);
            return result;
        }

        public ulong GetRepeatability(int n)
        {
            var repeatability = (long)(n + 1 - MaxBounds.X)*
                    (n + 1 - MaxBounds.Y)*
                    (n + 1 - MaxBounds.Z)
                    ;
            return (ulong)repeatability;
        }

        public ulong GetCombinations()
        {
            if (Variants == null) throw new InvalidOperationException("Variants have not been set");
            return (ulong) (Variants.Length) + 1;

/*
            var distinction = new[] {(int)A.X, (int)A.Y, (int)A.Z}.Where(i => i != 0).Distinct().Count();
            if (distinction == 1) return 1;
            else return (int) Math.Pow(2, distinction - 1);
*/


            /*var pointsA = new[] {(int) A.X, (int) A.Y, (int) A.Z};
            if (pointsA.Count(p => p == 0) == 2) return 1;
            else
            {
                var distinctCount = pointsA.Distinct().Count();
                if (distinctCount == 2) return 4; //e.g. 1,2,2
                else if (distinctCount == 3) return 6; //e.g. 1,2,2
                else throw new InvalidOperationException("Sanity check failed.");
            }*/
        }

        public int[] GetCountsByFace()
        {
            return allFaces.Select(f => pointsByFace.Count(p => p.Item2.SequenceEqual(new [] {f.ToString()}))).ToArray();
        }

        private Tuple<ulong,ulong> CalculateLatticePoints()
        {
            firstNonParallelFace = allFaces.FirstOrDefault(p => new[] {p.A, p.B, p.C}.Count(i => i != 0) >= 2);
            
            ulong inside = 0;
            ulong surface = 0;
            pointsByFace.Clear();
            if(MinBounds.X < 0 || MinBounds.Y < 0 || MinBounds.Z < 0) throw new InvalidOperationException("Should be aligned");
            for (int x = 0; x <= (int)Math.Ceiling(MaxBounds.X); x++)
            {
                for (int y = 0; y <= (int)Math.Ceiling(MaxBounds.Y); y++)
                {
                    for(int z= 0; z <= (int)Math.Ceiling(MaxBounds.Z); z++)
                    {
                        var v = new Vector3D(x,y,z);
                        if (oppositeFaces.All(pp => pp.IsBetweenOrOn(v)))
                        {
                            var facesOn = allFaces.Where(p => p.GetSide(v) == 0).ToArray();
                            if (facesOn.Any())
                            {
                                surface++;
                            }
                            else
                            {
                                inside++;
                            }
                            pointsByFace.Add(new Tuple<Vector3D, string[]>(v, facesOn.Select(f => f.ToString()).OrderBy(fn => fn).ToArray()));
                        }
                    }
                }
            }
            return Tuple.Create(inside, surface);
        }


        private static bool IsParallelToAxis(Vector3D vector3D, Vector3D axis)
        {
            var angle = Vector3D.AngleBetween(vector3D, axis);
            var result = (int) Math.Round(Math.Abs(angle), 0)%180 == 0;
            return result;
        }

        public override string ToString()
        {
            return $"A = {A} B = {B} C = {C}";
        }

        public Vector3D[] GetOrderedDefinitions()
        {
            var defs = Definitions.Select(d => d.X < 0 ? d*-1 : d)
                .OrderBy(d => d.X).ThenBy(d => d.Y).ThenBy(d => d.Z)
                .ToArray();
            return defs;
        }

        public bool Equals(Cube other)
        {
            if (ReferenceEquals(null, other)) return false;
            if (ReferenceEquals(this, other)) return true;
            var sequenceEqual = OrderedVertices.SequenceEqual(other.OrderedVertices, VectorIntEqualityComparer.Instance);
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

        public static bool TryMakeCubeFrom(Vector3D origin, Vector3D a, Vector3D b, Vector3D c, out Cube cube)
        {
            
            var points = new[]
            {
                origin,
                origin + a,
                origin + b,
                origin + c,
                origin + a + b,
                origin + a + c,
                origin + b + c,
                origin + a + b + c
            };
            if (points.All(Program.IsIntegral))
            {
                cube = new Cube(points, new [] { a,b,c});
                return true;
            }
            else
            {
                cube = null;
                return false;
            }

        }

        public string GetWpfSpec()
        {
            var order = new[] { 3, 5, 6, 7, 0, 1, 2, 4 };

            var verticesInWpfOrder = order.Select(i => Vertices[i]).ToArray();
            var wpfSpec = string.Join("   ", verticesInWpfOrder.Select(v => $"{(int) v.X} {(int) v.Y} {(int) v.Z}"));
            return wpfSpec;
        }

        public int GetPointsOnEdges()
        {
            return pointsByFace.Count(p => p.Item2.Length == 2);
        }
    }
}