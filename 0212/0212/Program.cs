using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using Mpir.NET;

namespace _0212
{
    static class Constants
    {
        public const int NMAX = 50000;
    }

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
            var sw = Stopwatch.StartNew();
            var cuboids = new List<Cuboid>();
            for (var n = 1; n <= Constants.NMAX; n++)
            {
                if (n % 100 == 0) Console.Write($"\r{n}");
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
            
            Console.WriteLine();
            mpz_t totalVolume = sortedCuboids.AsParallel().Aggregate(new mpz_t(0), (st,c) => 
            {
                if (i++ % 1000 == 0) Console.Write($"\r{i}");
                var intersectors = cuboidMasterSet.GetLowerIntersectors(c).ToArray();

                var subTotalVolume = c.Volume;

                int includeOrExclude = -1;
                var path = new List<int>();

                for (int count = 1; count <= intersectors.Length; count++)
                {
                    var partitions = GetPartitions(intersectors.Length - 1, count);
                    foreach (var partition in partitions)
                    {
                        var partitionShapes = new[] { c }.Concat(partition.Select(p => intersectors[p])).ToArray();
                        var intersection = Cuboid.Intersection(partitionShapes);
                        subTotalVolume += (includeOrExclude * intersection.Volume);
                    }
                    includeOrExclude *= -1;
                }
                return st + subTotalVolume;
            });
            sw.Stop();
            Console.WriteLine($"\n{totalVolume}\n{sw.Elapsed}");

        }
    }

    public class CuboidMasterSet : Cuboid, IIntersectionProvider
    {
        public IImmutableSet<IIntersectionProvider> Partitions { get; }
        public CuboidMasterSet(Cuboid[] cuboids) : this(cuboids,
            cuboids.Min(c => c.X),
            cuboids.Max(c => c.X1),
            cuboids.Min(c => c.Y),
            cuboids.Max(c => c.Y1),
            cuboids.Min(c => c.Z),
            cuboids.Max(c => c.Z1)
            )
            
        {
        }

        public CuboidMasterSet(Cuboid[] cuboids, mpz_t minX, mpz_t maxX, mpz_t minY, mpz_t maxY, mpz_t minZ, mpz_t maxZ)
            : base(cuboids.Min(c => c.N), 
                  minX,
                  minY, 
                  minZ, 
                  maxX, 
                  maxY, 
                  maxZ,
                  true)
        {
            var p = cuboids.Count() / 10;
            if (p > 5) p = 5;
            var cuboidPartitions = new List<Cuboid>[p+1,p+1,p+1];
            cuboids.ToImmutableArray();
            var cuboidSets = new List<Cuboid>();
            var rangeX = maxX - minX;
            var rangeY = maxY - minY;
            var rangeZ = maxZ - minZ;
            if (rangeX == 0) rangeX = 1;
            if (rangeY == 0) rangeY = 1;
            if (rangeZ == 0) rangeZ = 1;
            foreach (var c in cuboids)
            {
                int xp = (int)(((c.X - minX) * p) / rangeX);
                var yp = (int)(((c.Y - minY) * p) / rangeY);
                var zp = (int)(((c.Z - minZ) * p) / rangeZ);
                if (cuboidPartitions[xp, yp, zp] == null)
                {
                    cuboidPartitions[xp, yp, zp] = new List<Cuboid>();
                }
                cuboidPartitions[xp, yp, zp].Add(c);
            }

            var validPartitions = cuboidPartitions.Cast<List<Cuboid>>().Where(cp => cp != null).ToArray();
            if (cuboids.Count() < 100)
            {
                Partitions = validPartitions.Select(cp => (IIntersectionProvider)new CuboidSet(cp)).ToImmutableHashSet();
            }
            else
            {
                Partitions = validPartitions.Select(cp => (IIntersectionProvider)new CuboidMasterSet(cp.ToArray())).ToImmutableHashSet();
            }
        }

        public Cuboid[] GetLowerIntersectors(Cuboid cuboid)
        {
            var intersectingPartitions = Partitions.Where(p => p.MinN < cuboid.N && p.IntersectsWith(cuboid)).ToArray();
            var p5859 = Partitions.Where(p => p.MinN == 5859).FirstOrDefault();
            if(p5859 != null)
            {
                var iii = p5859.IntersectsWith(cuboid);
            }
            var intersectors = intersectingPartitions.SelectMany(p => p.GetLowerIntersectors(cuboid)).ToArray();
            return intersectors;
        }
    }

    public class CuboidSet : Cuboid, IIntersectionProvider
    {
        public IImmutableSet<Cuboid> Cuboids { get; }

        public CuboidSet(IList<Cuboid> cuboids)
            : base(cuboids.Min(c => c.N),
                  cuboids.Min(c => c.X),
                  cuboids.Min(c => c.Y),
                  cuboids.Min(c => c.Z),
                  cuboids.Max(c => c.X1),
                  cuboids.Max(c => c.Y1),
                  cuboids.Max(c => c.Z1), true)
        {
            Cuboids = cuboids.ToImmutableHashSet();
        }

        public Cuboid[] GetLowerIntersectors(Cuboid cuboid)
        {
            var interceptors = Cuboids.Where(c => c.N < cuboid.N && c.IntersectsWith(cuboid)).ToArray();
            return interceptors;
        }
    }

    public interface IIntersectionProvider
    {
        Cuboid[] GetLowerIntersectors(Cuboid cuboid);
        bool IntersectsWith(Cuboid cuboid);
        int MinN { get; }
    }

    public class Cuboid
    {
        public int N { get; }

        public virtual int MinN => N;
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

        public Cuboid(int n, mpz_t x, mpz_t y, mpz_t z, mpz_t dX, mpz_t dY, mpz_t dZ, bool isMax = false)
        {
            N = n;
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

        public static Cuboid Intersection(Cuboid[] cuboids)
        {
            Cuboid retval = new Cuboid(cuboids[0].N, cuboids[0].X, cuboids[0].Y, cuboids[0].Z, cuboids[0].DX, cuboids[0].DY, cuboids[0].DZ);
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

        public Cuboid Intersection(Cuboid other)
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
            var n = N < other.N ? N : other.N;
            var intersection = new Cuboid(n, x, y, z, dx, dy, dz);
            return intersection;
        }


        public bool IntersectsWith(Cuboid other)
        {
            var doesntIntersect = X1 <= other.X || other.X1 <= X
                                || Y1 <= other.Y || other.Y1 <= Y
                                || Z1 <= other.Z || other.Z1 <= Z;
            return !doesntIntersect;
        }

        public override bool Equals(object obj)
        {
            return obj is Cuboid cuboid &&
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
            return $"{N}: ({{{X},{Y},{Z})}} {{{DX},{DY},{DZ}}})";
        }

        public string Mathematica()
        {
            return $"Cuboid[{{{X}, {Y}, {Z}}}, {{{X1}, {Y1}, {Z1}}}]";
        }
    }

}
