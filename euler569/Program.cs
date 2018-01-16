using System;
using System.Collections.Generic;
using System.Diagnostics;
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
            const int maxCheck = 200;
            const int limit = 100; //2500000;
            var angles = new double[limit+1];
            mpz_t prime = 2;
            int totalCount = 0;
            var peaks = new Queue<Pos>();
            var prevPeak = new Pos(2, 2, 1);
            peaks.Enqueue(prevPeak);
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
                var xpos = prevPeak.X + next2Primes.Item1 + next2Primes.Item2;
                var ypos = prevPeak.Y - next2Primes.Item1 + next2Primes.Item2;
                var peak = new Pos(xpos, ypos, i);
                peaks.Enqueue(peak);
                double o = Math.Abs((double)prevPeak.Y - (double) peak.Y);
                double a = Math.Abs((double)prevPeak.X - (double) peak.X);
                angles[i] = o / a;
                int count = 0;
                double minAngle = double.MaxValue;
                var peaksArray = peaks.ToArray();
                var seen = new List<int>();
                for(int j = peaksArray.Length - 2; j >= 0; j--)
                {
                    var dy = peak.Y - peaksArray[j].Y;
                    double dx = Math.Abs((double)peak.X - (double) peaksArray[j].X);
                    double minProduct = minAngle * dx;
                    if (dy < minProduct)
                    {
                        minAngle =(double) dy / dx;
                        count++;
                        seen.Add(peaksArray[j].Id);
                    }
                }
                Console.WriteLine($"{i},{seen.Count},{string.Join(",", seen)}");
                totalCount += count;
                prevPeak = peak;
                while (peaks.Count > maxCheck) peaks.Dequeue();
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

        public mpz_t X { get; }
        public mpz_t Y { get; }
        public int Id { get; }
    }
}
