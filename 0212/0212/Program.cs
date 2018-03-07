using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.Linq;
using Mpir.NET;

namespace _0212
{
    class Program
    {
        static ConcurrentDictionary<mpz_t, mpz_t> sequenceCache = new ConcurrentDictionary<mpz_t, mpz_t>();
        static mpz_t S(mpz_t k) => sequenceCache.GetOrAdd(k, MakeS);

        static ConcurrentDictionary<(int nMax, int count), int[][]> partitionsCache = new ConcurrentDictionary<(int nMax, int count), int[][]>();
        static int[][] GetPartitions(int nMax, int count) => partitionsCache.GetOrAdd((nMax, count), MakePartitions);
        static int[][] MakePartitions((int nMax, int count) p)
        {
            var current = new List<int>();
            var all = new List<int[]>();
            MakePartitions(0, p.nMax, p.count, current, all);
            return all.ToArray();
        }

        static void MakePartitions(int nMin, int nMax, int count, List<int> current, List<int[]> all)
        {
            for (int n = nMin; n <= nMax; n++)
            {
                current.Add(n);
                if (current.Count == count)
                    all.Add(current.ToArray());
                else
                    MakePartitions(n + 1, nMax, count, current, all);
                current.Remove(n);
            }
        }

        private static mpz_t MakeS(mpz_t k)
        {
            mpz_t s;
            if (k <= 55)
            {
                s = (100003 - 200003 * k + 300007 * k.Power(3)).Mod(1000000);
            }
            else
            {
                s = (S(k - 24) + S(k - 55)).Mod(1000000);
            }
            return s;
        }

        static void Main(string[] args)
        {
            var cuboids = new List<Cuboid>();
            var maxIntersectsWith = 0;
            for (var n = 1; n <= 100; n++)
            {
                if (n % 100 == 0) Console.Write($"\r{n}   {maxIntersectsWith}");
                var x = S(6 * n - 5).Mod(10000);
                var y = S(6 * n - 4).Mod(10000);
                var z = S(6 * n - 3).Mod(10000);
                var dx = 1 + S(6 * n - 2).Mod(399);
                var dy = 1 + S(6 * n - 1).Mod(399);
                var dz = 1 + S(6 * n).Mod(399);
                var cuboid = new Cuboid(n, x, y, z, dx, dy, dz);
                cuboids.Add(cuboid);
            }

            Cuboid[] sortedCuboids = cuboids.OrderBy(c => c.X).ThenBy(c => c.Y).ThenBy(c => c.Z).ToArray();
            var cuboidMasterSet = new CuboidMasterSet(sortedCuboids);

            int i = 0;
            mpz_t totalVolume = 0;

            foreach (var c in sortedCuboids)
            {
                if (i++ % 1000 == 0) Console.Write($"\r{i}");
                var intersectors = cuboidMasterSet.GetLowerIntersectors(c).ToArray();

                totalVolume += c.Volume;

                int includeOrExclude = -1;
                for (int count = 1; count <= intersectors.Length; count++)
                {
                    var partitions = GetPartitions(intersectors.Length - 1, count);
                    foreach (var partition in partitions)
                    {
                        var partitionShapes = new[] { c }.Concat(partition.Select(p => intersectors[p])).ToArray();
                        var intersection = Shape.Intersection(partitionShapes);
                        totalVolume += (includeOrExclude * intersection.Volume);
                    }
                    includeOrExclude *= -1;
                }

                if (intersectors.Length > maxIntersectsWith)
                {
                    maxIntersectsWith = intersectors.Length;
                    var mathStrings = new[] { c }.Concat(intersectors).Select(s => s.Mathematica());
                    Console.WriteLine($"\nGraphics3D[{{{string.Join("\n  , ", mathStrings)}}}]");
                    Console.WriteLine();
                }
            }
            Console.WriteLine($"\n{totalVolume}");

        }
    }

    public class CuboidMasterSet
    {
        public IImmutableSet<CuboidSet> Partitions { get; }
        private const int p = 10;
        public CuboidMasterSet(IEnumerable<Cuboid> cuboids)
        {
            var cuboidPartitions = new List<Cuboid>[p + 1, p + 1, p + 1];
            cuboids.ToImmutableArray();
            var cuboidSets = new List<Cuboid>();
            var minX = cuboids.Min(c => c.X);
            var maxX = cuboids.Max(c => c.X);
            var minY = cuboids.Min(c => c.Y);
            var maxY = cuboids.Max(c => c.Y);
            var minZ = cuboids.Min(c => c.Z);
            var maxZ = cuboids.Max(c => c.Z);

            foreach (var c in cuboids)
            {
                int xp = (int)(((c.X - minX) * p) / (maxX - minX));
                var yp = (int)(((c.Y - minY) * p) / (maxY - minY));
                var zp = (int)(((c.Z - minY) * p) / (maxZ - minZ));
                if (cuboidPartitions[xp, yp, zp] == null)
                {
                    cuboidPartitions[xp, yp, zp] = new List<Cuboid>();
                }
                cuboidPartitions[xp, yp, zp].Add(c);
            }
            Partitions = cuboidPartitions.Cast<List<Cuboid>>().Where(cp => cp != null)
                .Select(cp => new CuboidSet(cp))
                .ToImmutableHashSet();
        }

        public Cuboid[] GetLowerIntersectors(Cuboid cuboid)
        {
            var intersectingPartitions = Partitions.Where(p => p.MinN < cuboid.N && p.IntersectsWith(cuboid)).ToArray();
            var intersectors = intersectingPartitions.SelectMany(p => p.Cuboids.Where(c => c.N < cuboid.N && c.IntersectsWith(cuboid))).ToArray();
            return intersectors;
        }
    }

    public class CuboidSet : Shape
    {
        public IImmutableSet<Cuboid> Cuboids { get; }
        public int MinN { get; }
        public CuboidSet(IList<Cuboid> cuboids)
            : base(cuboids.Min(c => c.X),
                  cuboids.Min(c => c.Y),
                  cuboids.Min(c => c.Z),
                  cuboids.Max(c => c.X1),
                  cuboids.Max(c => c.Y1),
                  cuboids.Max(c => c.Z1), true)
        {
            Cuboids = cuboids.ToImmutableHashSet();
            MinN = cuboids.Min(c => c.N);
        }
    }

    public class Cuboid : Shape
    {
        public Cuboid(int n, mpz_t x, mpz_t y, mpz_t z, mpz_t dX, mpz_t dY, mpz_t dZ) : base(x, y, z, dX, dY, dZ)
        {
            N = n;
        }

        public int N { get; }
        public override string ToString()
        {
            return $"{N}: {base.ToString()}";
        }

        public string Mathematica()
        {
            return $"Cuboid[{{{X}, {Y}, {Z}}}, {{{X1}, {Y1}, {Z1}}}]";
        }
    }

    public class Shape
    {
        public mpz_t X { get; }
        public mpz_t Y { get; }
        public mpz_t Z { get; }
        public mpz_t DX { get; }
        public mpz_t DY { get; }
        public mpz_t DZ { get; }
        public mpz_t X1 => X + DX;
        public mpz_t Y1 => Y + DY;
        public mpz_t Z1 => Z + DZ;
        public mpz_t Volume => DX * DY * DZ;

        public Shape(mpz_t x, mpz_t y, mpz_t z, mpz_t dX, mpz_t dY, mpz_t dZ, bool isMax = false)
        {
            X = x;
            Y = y;
            Z = z;
            DX = dX;
            DY = dY;
            DZ = dZ;
            if (isMax)
            {
                DX -= X;
                DY -= Y;
                DZ -= Z;
            }
        }

        public static Shape Intersection(Shape[] cuboids)
        {
            Shape retval = new Shape(cuboids[0].X, cuboids[0].Y, cuboids[0].Z, cuboids[0].DX, cuboids[0].DY, cuboids[0].DZ);
            for (int i = 1; i < cuboids.Length; i++)
            {
                retval = retval.Intersection(cuboids[i]);
                if (retval.DX == 0 || retval.DY == 0 || retval.DZ == 0) break;
            }
            return retval;
        }

        private static mpz_t Min(mpz_t l, mpz_t r)
        {
            var minpos = l < r ? l : r;
            return minpos;
        }

        private static mpz_t Max(mpz_t l, mpz_t r)
        {
            var minpos = l > r ? l : r;
            return minpos;
        }

        public Shape Intersection(Shape other)
        {
            var x = Max(X, other.X);
            var y = Max(Y, other.Y);
            var z = Max(Z, other.Z);
            var x1 = Min(X1, other.X1);
            var y1 = Min(Y1, other.Y1);
            var z1 = Min(Z1, other.Z1);
            var dx = Max(x1 - x, 0);
            var dy = Max(y1 - y, 0);
            var dz = Max(z1 - z, 0);
            var intersection = new Shape(x, y, z, dx, dy, dz);
            return intersection;
        }


        public bool IntersectsWith(Shape other)
        {
            var doesntIntersect = X1 <= other.X || other.X1 <= X
                                || Y1 <= other.Y || other.Y1 <= Y
                                || Z1 <= other.Z || other.Z1 <= Z;
            return !doesntIntersect;
        }

        public override bool Equals(object obj)
        {
            return obj is Shape cuboid &&
                   X == cuboid.X &&
                   Y == cuboid.Y &&
                   Z == cuboid.Z &&
                   DX == cuboid.DX &&
                   DY == cuboid.DY &&
                   DZ == cuboid.DZ;
        }

        public override int GetHashCode()
        {
            unchecked
            {
                var hashCode = 236514578;
                hashCode = hashCode * -1521134295 + X.GetHashCode();
                hashCode = hashCode * -1521134295 + Y.GetHashCode();
                hashCode = hashCode * -1521134295 + Z.GetHashCode();
                hashCode = hashCode * -1521134295 + DX.GetHashCode();
                hashCode = hashCode * -1521134295 + DY.GetHashCode();
                hashCode = hashCode * -1521134295 + DZ.GetHashCode();
                return hashCode;
            }
        }

        public override string ToString()
        {
            return $"[{{{X},{Y},{Z})}} {{{DX},{DY},{DZ}}}]";
        }
    }

}
