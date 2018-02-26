using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _0208
{
    class Program
    {
        static Dictionary<int, double[]> xMoves = new Dictionary<int, double[]>
        {
            {0,  new double[]{ Math.Cos(Math.PI * 2 / 5) - 1 , 1 - Math.Cos(Math.PI * 2 / 5)} },
            {1,  new double[]{ Math.Cos(Math.PI * 2 / 5) - Math.Cos(Math.PI * 4/ 5),  1 - Math.Cos(Math.PI * 2 / 5)}},
            {2, new double[]{0 , Math.Cos(Math.PI * 2 / 5) - Math.Cos(Math.PI * 4 / 5) } },
            {3, new double[]{ Math.Cos(Math.PI * 4 / 5) - Math.Cos(Math.PI * 2 / 5), 0 } },
            {4, new double[]{ Math.Cos(Math.PI * 2 / 5) - 1, Math.Cos(Math.PI * 4 / 5) - Math.Cos(Math.PI * 2 / 5) } }
        };
        static Dictionary<int, double[]> yMoves = new Dictionary<int, double[]>
        {
            { 0, new double[] { Math.Sin(Math.PI *2/5), Math.Sin(Math.PI *2/5) } },
            { 1, new double[] { Math.Sin(Math.PI * 4 / 5) - Math.Sin(Math.PI * 2 / 5), Math.Sin(Math.PI * 2 / 5) } },
            { 2, new double[]{ - 2 * Math.Sin(Math.PI * 4 / 5) , Math.Sin(Math.PI * 4 / 5) - Math.Sin(Math.PI * 2 / 5) } },
            { 3, new double[] { Math.Sin(Math.PI * 4 / 5) - Math.Sin(Math.PI * 2 / 5) , - 2 * Math.Sin(Math.PI * 4 / 5) } },
            {4, new double[] { Math.Sin(Math.PI * 2 / 5) , Math.Sin(Math.PI * 4 / 5) - Math.Sin(Math.PI * 2 / 5) } }
        };

        static double[] arcXDiffs =
        {
            0,
            1 - Math.Cos(Math.PI*2/5),
            Math.Cos(Math.PI*2/5) - Math.Cos(Math.PI*4/5),
            0,
            Math.Cos(Math.PI * 6/5) - Math.Cos(Math.PI * 8/5),
            Math.Cos(Math.PI * 2/5) - 1
        };
        static double[] arcYDiffs =
        {
            0,
            Math.Sin(Math.PI * 2/5),
            Math.Sin(Math.PI * 4/5) - Math.Sin(Math.PI * 2/5),
            Math.Sin(Math.PI * 6/5) - Math.Sin(Math.PI * 4/5),
            Math.Sin(Math.PI * 8/5) - Math.Sin(Math.PI * 6/5),
            Math.Sin(Math.PI * 2/5)
        };

        static bool Approx(double l, double r)
        {
            return Math.Abs(l - r) < 1e-9;
        }

        static ConcurrentDictionary<(double, double, int, long), long> cache = new ConcurrentDictionary<(double, double, int, long), long>(
            EqualityComparers.ExpressionEqualityComparer<(double x, double y, int pos, long left)>.Create((a1, a2) =>
            a1.pos == a2.pos &&
            Approx(a1.x, a2.x) &&
            Approx(a1.y, a2.y) &&
            a1.left == a2.left));

        static long W(long left)
        {
            return W(0, 0, 0, left);
        }

        static long W(double x, double y, int pos, long left)
        {
            return cache.GetOrAdd((x, y, pos, left), W);
        }

        static long W((double x, double y, int pos, long left) p)
        {
            if (Approx(p.x, 0) && Approx(p.y, 0) && p.left == 0)
            {
                return 1;
            }
            if (Math.Sqrt(p.x * p.x + p.y * p.y) > p.left * 2 * Math.PI)
            {
                return 0;
            }

            double[] xm = xMoves[Math.Abs(p.pos)].Select(d => d * Math.Sign(0.5 + p.pos)).ToArray();
            double[] ym = yMoves[Math.Abs(p.pos)];

            var posl = p.pos - 1;
            if (posl == -5) posl = 0;
            var xl = p.x + xm[0];
            var yl = p.y + ym[0];

            var posr = p.pos + 1;
            if (posr == 5) posr = 0;
            var xr = p.x + xm[1];
            var yr = p.y + ym[1];

            var l = W(xl, yl, posl, p.left - 1);
            var r = W(xr, yr, posr, p.left - 1);
            var retval = l + r;
            return retval;
        }
        static void Main(string[] args)
        {
            var sumx = arcXDiffs.Sum();
            var sumy = arcYDiffs.Sum();
            Console.WriteLine(W(5));
        }
    }
}
