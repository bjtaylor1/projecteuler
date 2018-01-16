using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.Linq;
using Mpir.NET;

namespace euler569
{
    class Program
    {
        static void Main(string[] args)
        {
            const int limit = 100; //2500000;
            mpz_t prime = 2;
            int totalCount = 0;
            var peaks = new Dictionary<int,Pos>();
            peaks[0] = new Pos(0, 0, 0);
            peaks[1] = new Pos(2, 2, 1);
            Stopwatch sw = Stopwatch.StartNew();
            for(int i = 2; i <= limit; i++)
            {
                if (i % 10000 == 0)
                {
                    var velocity = (1000000000d) / sw.ElapsedTicks;
                    Console.Write($"{(double)i / limit:P1}, velocity={velocity:0.00000}\r");
                    sw.Restart();
                }
                var next2Primes = (prime = prime.NextPrimeGMP(), prime = prime.NextPrimeGMP());
                var xpos = peaks[i-1].X + next2Primes.Item1 + next2Primes.Item2;
                var ypos = peaks[i-1].Y - next2Primes.Item1 + next2Primes.Item2;
                peaks[i] = new Pos(xpos, ypos, i);
            }
            Console.WriteLine("\nMade peaks");
            for (int i = 1; i < limit;)
            {
                var next = peaks.Where(p => p.Key > i)
                    .OrderByDescending(p => Fraction.AngleBetween(p.Value, peaks[i]))
                    .First();
                peaks[i].Next = next.Key;
                next.Value.Previous = i;
                i = next.Key;
            }
            peaks[limit].Next = limit + 1;

            int lowerLimit = 1;
            for (int i = 2; i <= limit; i++)
            {
                if (i % 10000 == 0)
                {
                    var velocity = (1000000000d) / sw.ElapsedTicks;
                    Console.Write($"{(double)i / limit:P1}, velocity={velocity:0.00000}\r");
                    sw.Restart();
                }

                Fraction minAngle = Fraction.AngleBetween(peaks[i], peaks[i-1]);
                int count = 1;
                for (int j = i - 2; j >= lowerLimit; j--)
                {
                    var angle = Fraction.AngleBetween(peaks[i], peaks[j]);
                    if (angle.CompareTo(minAngle) < 0)
                    {
                        minAngle = angle;
                        count++;
                    }
                }
                if (peaks[i].Previous > lowerLimit)
                {
                    lowerLimit = peaks[i].Previous;
                    peaks = peaks.Where(p => p.Key >= lowerLimit).ToDictionary(k => k.Key, k => k.Value);
                }


                totalCount += count;
            }
            Console.WriteLine($"\n{totalCount}");
        }
    }

    public class Pos
    {
        public override string ToString()
        {
            return $"{Id}: {X}, {Y}";
        }

        public Pos(mpz_t x, mpz_t y, int id)
        {
            X = x;
            Y = y;
            Id = id;
        }
        public int Previous { get; set; }
        public mpz_t X { get; }
        public mpz_t Y { get; }
        public int Id { get; }
        public int Next { get; set; }
    }

    public class Fraction : IComparable<Fraction>
    {
        public mpz_t Top { get; }
        public mpz_t Bottom { get; }

        public Fraction(mpz_t top, mpz_t bottom)
        {
            Top = top;
            Bottom = bottom;
        }

        public int CompareTo(Fraction other)
        {
            if (ReferenceEquals(this, other)) return 0;
            if (ReferenceEquals(null, other)) return 1;

            return (Top * other.Bottom).CompareTo(other.Top * Bottom);
        }

        public static Fraction AngleBetween(Pos p1, Pos p2)
        {
            return new Fraction(p1.Y - p2.Y, p1.X - p2.X);
        }
    }
}
