using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Mpir.NET;

namespace euler569
{
    class Program
    {
        static void Main(string[] args)
        {
            const int limit = 100; //2500000
            var peaks = new Pos[limit+1];
            var angles = new double[limit+1];
            peaks[1] = new Pos(2, 2);
            mpz_t prime = 2;
            int totalCount = 0;
            for(int i = 2; i <= limit; i++)
            {
                var next2primes = (prime = prime.NextPrimeGMP(), prime = prime.NextPrimeGMP());
                var xpos = peaks[i - 1].X + next2primes.Item1 + next2primes.Item2;
                var ypos = peaks[i - 1].Y - next2primes.Item1 + next2primes.Item2;
                peaks[i] = new Pos(xpos, ypos);
                angles[i] = peaks[i].AngleTo(peaks[i - 1]);
                int count = 0;
                double minAngle = double.MaxValue;
                var anglesToThis = peaks.Skip(1).Take(i - 1).Select(p => p.AngleTo(peaks[i])).ToArray();
                for(int j = i-1; j >= 1; j--)
                {
                    double angle = peaks[j].AngleTo(peaks[i]);
                    if(angle < minAngle)
                    {
                        minAngle = angle;
                        count++;
                    }
                }
                totalCount += count;
            }
        }
    }

    public class Pos
    {
        public Pos(mpz_t x, mpz_t y)
        {
            X = x;
            Y = y;
        }

        public mpz_t X { get; }
        public mpz_t Y { get; }

        public double AngleTo(Pos other)
        {
            double o = Math.Abs((double)other.Y - (double)Y);
            double a = Math.Abs((double)other.X - (double)X);
            var tan = o / a;
            double angle = Math.Atan(tan) * 180 / Math.PI;
            return angle;
        }
    }
}
