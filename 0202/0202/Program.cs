using System;
using System.Collections.Generic;
using Mpir.NET;

namespace _0202
{
    class Program
    {
        static void Main(string[] args)
        {
            long crosses = 1000001;// 11; //  12017639147;
            long row = (crosses + 3) / 2;
            mpz_t row_t = row;
            long startx = 6 - 3 * (row % 2); //3 if row is odd, 6 if it's even.
            long tot = 0;
            long div = 2 - (row % 2); //2 for even rows, 1 for odd rows.
            long x;
            var patterns = new HashSet<int>();
            for (x = startx; x < row; x += 6)
            {
                Console.Write($"{x}\r");
                mpz_t gcd = mpz_t.Gcd(x /div, row_t);
                if (gcd == 1)
                {
                    //var pattern = GetPattern((int)x, (int)row);
                    //if (patterns.Add(pattern.GetHashCode()))
                    {
                        tot++;
                    }
                }
            }
            Console.WriteLine();
            Console.WriteLine(tot * 2);
        }

        static string[][] rowPatterns = new string[][]
        {
            new string[]{"B", "C", "A"},
            new string[]{"C", "A", "B"}
        };
        //static string[][] rowPatterns = new string[][]
        //{
        //    new string[]{"AC", "AB", "BC"},
        //    new string[]{"AB", "BC", "AC"}
        //};
        //static Dictionary<string, string> fillers = new Dictionary<string, string>
        //{
        //    {"ABAC", "BC" },
        //    {"ACBC", "AB" },
        //    {"ABBC", "AC" }
        //};

        static string GetPattern(int x, int row)
        {
            var crosses = new List<string>();
            for(long lrow = 1; lrow < row; lrow++)
            {
                var segment = (((lrow * x / row) + (lrow % 2)) / 2) %3;
                var pattern = rowPatterns[lrow % 2][segment];
                //if (crosses.Any())
                //{
                //    var fillerKey = string.Join("", new[] { crosses.Last(), pattern }.OrderBy(s => s));
                //    var filler = fillers[fillerKey];
                //    crosses.Add(filler);
                //}
                crosses.Add(pattern);
            }
            var returnValue = string.Join(" ", crosses);
            return returnValue;
        }
    }
}
