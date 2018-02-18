using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Mpir.NET;

namespace _0408
{
    class Program
    {
        static ConcurrentDictionary<Point, mpz_t> cache = new ConcurrentDictionary<Point, mpz_t>();
        static mpz_t WaysOfGettingTo(Point p) => cache.GetOrAdd(p, MakeWaysOfGettingTo);
        const int N = 3;
        static mpz_t[,] ways = new mpz_t[N+1, N+1];
        static mpz_t[] wayss = new mpz_t[N + 1];
        static mpz_t[] totals = new mpz_t[N + 1];
        static mpz_t MakeWaysOfGettingTo(Point p)
        {
            if (p.X == 0) return 1;
            if (p.Y == 0) return 1;
            if (p.IsInadmissable()) return 0;
            mpz_t left = WaysOfGettingTo(p.Left());
            mpz_t down = WaysOfGettingTo(p.Down());
            mpz_t result = left + down;
            Console.WriteLine($"W({p}) = {result}   = {p.Left()}+{p.Down()}");
            return result;
        }

        static void Render()
        {
            for(long y = N; y >= 0; y--)
            {
                for(long x = 0; x <= N; x++)
                {
                    Console.Write((ways[x, y] ?? 0).ToString().PadLeft(3, ' '));
                }
                Console.WriteLine();
            }
            Console.WriteLine();
        }


        static void Main(string[] args)
        {
            ways[0, 0] = 0;
            for (int x = 1; x <= N; x++) ways[0, x] = ways[x, 0] = 1;
            
            for (int y = 1; y <= N; y++)
            {
                var yIsPerfectSquare = IsPerfectSquare(y);
                for (int x = y; x <= N; x++)
                {
                    if (yIsPerfectSquare && IsPerfectSquare(x) && IsPerfectSquare(x + y))
                        ways[x, y] = ways[y, x] = 0;
                    else
                        ways[x, y] = ways[y, x] = ways[x - 1, y] + ways[x, y - 1];
                }
            }
            //Console.WriteLine(ways[N, N].Mod(1000000007));
            //Console.WriteLine(WaysOfGettingTo(new Point(N,N)).Mod(1000000007));
        }

        static readonly ConcurrentDictionary<int, bool> isPerfectSquareCache = new ConcurrentDictionary<int, bool>();

        public static bool IsPerfectSquare(int n)
        {
            return isPerfectSquareCache.GetOrAdd(n, _n =>
            {
                int intsqrt = ((int)Math.Sqrt(n));
                return intsqrt * intsqrt == n;
            });
        }

        static void TestPoint(Point p)
        {
            Console.WriteLine($"{p}: {p.IsInadmissable()}");
        }

    }

    public class Point
    {
        public int X { get; }
        public int Y { get; }

        public Point(int x, int y)
        {
            X = x;
            Y = y;
        }

        public Point Left()
        {
            return new Point(X - 1, Y);
        }

        public Point Down()
        {
            return new Point(X, Y - 1);
        }

        public override bool Equals(object obj)
        {
            return obj is Point point &&
                   ((X == point.X &&
                   Y == point.Y)  ||
                   (X == point.Y && 
                   Y == point.X) );
        }

        public override int GetHashCode()
        {
            var hashCode = 1861411795;
            var xy = new[] { X, Y };
            Array.Sort(xy);
            hashCode = hashCode * -1521134295 + xy[0].GetHashCode();
            hashCode = hashCode * -1521134295 + xy[1].GetHashCode();
            return hashCode;
        }

        public bool IsInadmissable()
        {
            bool isInadmissable = X > 0 && Y > 0 &&
                IsPerfectSquare(X) &&
                IsPerfectSquare(Y) &&
                IsPerfectSquare(X + Y);
            return isInadmissable;
        }

        public override string ToString()
        {
            return $"{X},{Y}";
        }


        public static bool IsPerfectSquare(int n)
        {
            int intsqrt = ((int)Math.Sqrt(n));
            return intsqrt * intsqrt == n;
        }

    }
}
