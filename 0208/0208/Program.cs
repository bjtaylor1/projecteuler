using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Threading;

namespace _0208
{
    class Pos
    {
        public int X1 { get; }
        public int X2 { get; }
        public int Y1 { get; }
        public int Y2 { get; }

        public Pos(int x1, int x2, int y1, int y2)
        {
            X1 = x1;
            X2 = x2;
            Y1 = y1;
            Y2 = y2;
        }

        public override bool Equals(object obj)
        {
            return obj is Pos pos &&
                   X1 == pos.X1 &&
                   X2 == pos.X2 &&
                   Y1 == pos.Y1 &&
                   Y2 == pos.Y2;
        }

        public override int GetHashCode()
        {
            var hashCode = 1982840186;
            hashCode = hashCode * -1521134295 + X1.GetHashCode();
            hashCode = hashCode * -1521134295 + X2.GetHashCode();
            hashCode = hashCode * -1521134295 + Y1.GetHashCode();
            hashCode = hashCode * -1521134295 + Y2.GetHashCode();
            return hashCode;
        }

        public bool IsZero()
        {
            return X1 == 0 && X2 == 0 && Y1 == 0 && Y2 == 0;

        }

        public static Pos operator+(Pos l, Pos r)
        {
            return new Pos(
                l.X1 + r.X1,
                l.X2 + r.X2,
                l.Y1 + r.Y1,
                l.Y2 + r.Y2);
        }

        public int Amplitude()
        {
            return Math.Abs(X1) + Math.Abs(X2) + Math.Abs(Y1) + Math.Abs(Y2);
        }

        public override string ToString()
        {
            return $"{X1}x1 + {X2}x2, {Y1}y1 + {Y2}y2";
        }
    }

    class Program
    {
        static Dictionary<int, (Pos L, Pos R)> moves = new Dictionary<int, (Pos L, Pos R)>
        {
            { 0, (new Pos(-1, 0, 1, 0), new Pos(1, 0, 1, 0)) },
            { -1, (new Pos(0, -1, -1, 1), new Pos(-1, 0, 1, 0)) },
            {-2, (new Pos(0,0,0, -2), new Pos(0, -1, -1, 1)) },
            {-3, (new Pos(0,1, -1, 1), new Pos(0,0,0,-2)) },
            {-4, (new Pos(1, 0, 1, 0), new Pos(0, 1, -1, 1)) },
            { 1, (new Pos(1, 0, 1, 0), new Pos(0, 1, -1, 1)) },
            {2, (new Pos(0, 1, -1, 1), new Pos(0, 0, 0, -2)) },
            {3, (new Pos(0, 0, 0, -2), new Pos(0, -1, -1, 1)) },
            {4, (new Pos(0, -1, -1, 1),new Pos(-1, 0, 1, 0)) }
        };

        
        static ConcurrentDictionary<(Pos pos, int facing, int left), long> cache = new ConcurrentDictionary<(Pos pos, int facing, int left), long>(
            EqualityComparers.ExpressionEqualityComparer<(Pos pos, int facing, int left)>.Create((a1, a2) =>
            a1.pos == a2.pos &&
            a1.facing == a2.facing &&
            a1.left == a2.left));

        static long W(int left)
        {
            return W(new Pos(0, 0, 0, 0), 0, left);
        }

        static long W(Pos pos, int facing, int left)
        {
            return cache.GetOrAdd((pos, facing, left), W);
        }

        static long total = 0;
        static long W((Pos pos, int facing, int left) p)
        {
            if (p.pos.IsZero() && p.left == 0)
            {
                if (++total % 1000 == 0)
                    Console.Write($"{total}\r");
                return 1;
            }
            if (p.pos.Amplitude() > p.left * 4)
            {
                return 0;
            }

            var facingL = p.facing - 1;
            if (facingL == -5) facingL = 0;
            var facingR = p.facing + 1;
            if (facingR == 5) facingR = 0;

            var movesThis = moves[p.facing];
            var retval = 0L;
            //Console.WriteLine($" {p.pos}");
            //var key = Console.ReadKey().Key;
            //if (key == ConsoleKey.LeftArrow)
            {
                //Console.Write("Moving L to ");
                retval += W(p.pos + movesThis.L, facingL, p.left - 1);
            }
            //else if (key == ConsoleKey.RightArrow)
            {
                //Console.Write("Moving R to ");
                retval += W(p.pos + movesThis.R, facingR, p.left - 1);
            }
            //else throw new InvalidOperationException();
            
            return retval;
        }
        static void Main(string[] args)
        {
            var sumL1 = moves.Where(m => m.Key <= 0).Aggregate(new Pos(0, 0, 0, 0), (c, p) => c + p.Value.L);
            var sumL2 = moves.Where(m => m.Key >= 0).Aggregate(new Pos(0, 0, 0, 0), (c, p) => c + p.Value.L);
            var sumR1 = moves.Where(m => m.Key <= 0).Aggregate(new Pos(0, 0, 0, 0), (c, p) => c + p.Value.R);
            var sumR2 = moves.Where(m => m.Key >= 0).Aggregate(new Pos(0, 0, 0, 0), (c, p) => c + p.Value.R);

            Console.WriteLine(W(25));
        }
    }


}
