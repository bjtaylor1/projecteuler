using System;
using System.Linq;

namespace euler579cs2
{
    public class cube : IEquatable<cube>
    {
        public bool Equals(cube other)
        {
            if (ReferenceEquals(null, other)) return false;
            if (ReferenceEquals(this, other)) return true;
            return vertices.SequenceEqual(other.vertices);
        }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            if (ReferenceEquals(this, obj)) return true;
            if (obj.GetType() != this.GetType()) return false;
            return Equals((cube) obj);
        }

        public override int GetHashCode()
        {
            unchecked
            {
                return vertices?.Aggregate(1, (i, v) => (i * 397) ^ v.GetHashCode()) ?? 0;
            }
        }

        public static bool operator ==(cube left, cube right)
        {
            return Equals(left, right);
        }

        public static bool operator !=(cube left, cube right)
        {
            return !Equals(left, right);
        }

        public vertex[] vertices { get; }

        private static readonly vertex O = new vertex(0,0,0);
        public cube(vector U, vector V, vector N, bool flipX, bool flipY, bool flipZ, int[] order)
        {
            var tempvertices = new[]
            {
                O,
                U,
                V,
                N,
                U + V,
                U + N,
                V + N,
                U + V + N
            };
            long minX = tempvertices.Min(v => v.x),
                minY = tempvertices.Min(v => v.y),
                minZ = tempvertices.Min(v => v.z);
            var alignedVertices = tempvertices.Select(v => new vertex(v.x - minX, v.y - minY, v.z - minZ)).ToArray();
            long maxX = alignedVertices.Max(v => v.x),
                maxY = alignedVertices.Max(v => v.y),
                maxZ = alignedVertices.Max(v => v.z);
            var initvertices = alignedVertices.Select(v =>
            {
                var xyz = new[]
                {
                    flipX ? v.x : maxX - v.x,
                    flipY ? v.y : maxY - v.y,
                    flipZ ? v.z : maxZ - v.z
                };
                return new vertex(xyz[0], xyz[1], xyz[2]);

            }).ToArray();

            if (initvertices.Distinct().Count() != 8) throw new InvalidOperationException("Cube does not have 8 distinct vertices");

            if (initvertices.Min(v => v.x) != 0 || initvertices.Min(v => v.y) != 0 || initvertices.Min(v => v.z != 0))
                throw new InvalidOperationException("Cube is not aligned");

            width = initvertices.Max(v => v.x);
            height = initvertices.Max(v => v.y);
            depth = initvertices.Max(v => v.z);
            vertices = initvertices.OrderBy(v => v.x).ThenBy(v => v.y).ThenBy(v => v.z).ToArray();

        }

        public long width { get;  }
        public long height { get;  }
        public long depth { get;  }

        public bool is_oversize()
        {
            return (width > Program.N) || (height > Program.N) || (depth > Program.N);
        }
    }
}